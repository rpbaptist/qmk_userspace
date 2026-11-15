#!/usr/bin/env bash
# Build and run one host-side gtest from <userspace>/tests/<name> in the QMK
# test harness. Usage: run-test.sh <name> [gtest filter]
set -euo pipefail
source "$(dirname "$0")/lib.sh"

name=${1:?usage: run-test.sh <name> [gtest filter]}
filter=${2:-*}
src="$USERSPACE/tests/$name"
[[ -f $src/test.mk ]] || { echo "No test at $src" >&2; exit 2; }
[[ $name != *-* ]] || { echo "QMK test names cannot contain '-'" >&2; exit 2; }

# QMK finds tests only as real files under qmk_firmware/tests. This copy is
# verification scaffolding; the trap removes it.
stage="$QMK_HOME/tests/rpbaptist_$name"
[[ ! -e $stage ]] || { echo "$stage exists: another run is active, or stale. Run cleanup.sh." >&2; exit 3; }
trap 'rm -rf "$stage"' EXIT
cp -r "$src" "$stage"

ev=$(new_evidence_dir "test-$name")
elf="$BUILD_DIR/test/rpbaptist_$name.elf"
# A failed build leaves the old .elf. Delete it, so that only a new build runs.
rm -f "$elf"
set +e
make -C "$QMK_HOME" "test:rpbaptist_$name" QMK_USERSPACE="$USERSPACE" COLOR=false \
    GTEST_FILTER="$filter" >"$ev/build.log" 2>&1
status=$?
if [[ -x $elf ]]; then
    "$elf" --gtest_color=no --gtest_filter="$filter" --gtest_output="xml:$ev/result.xml" >"$ev/test.log" 2>&1
    status=$?
fi
set -e
git -C "$USERSPACE" rev-parse HEAD >"$ev/commit"
echo "status=$status evidence=$ev"
tail -n 3 "$ev/test.log" 2>/dev/null || tail -n 20 "$ev/build.log"
exit "$status"
