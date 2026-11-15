#!/usr/bin/env bash
# Compile every keymap in this userspace and record size and warnings.
# Usage: build-all.sh [keyboard filter, e.g. crkbd] [qmk compile args, e.g. -e RGB_MATRIX_ENABLE=no]
set -uo pipefail
source "$(dirname "$0")/lib.sh"

# keymap directory -> build target. Add a line when you add a keymap.
targets=(
    "crkbd/rev1:rpbaptist"
    "keebio/iris/rev2:rpbaptist"
    "capsunlocked/cu7:rpbaptist"
)
# Stale: these fail on current QMK (old keycode names, RGBLIGHT_LED_COUNT
# redefined). build-all.sh skips them unless STALE=1.
stale=(
    "planck/rev6:rpbaptist"
    "dz60:marina"
)
[[ ${STALE:-0} == 1 ]] && targets+=("${stale[@]}")

ev=$(new_evidence_dir build)
git -C "$USERSPACE" rev-parse HEAD >"$ev/commit"
git -C "$USERSPACE" status --short >"$ev/dirty"
echo "THEME=${THEME:-} args=${*:2}" >"$ev/options"
fail=0
for t in "${targets[@]}"; do
    [[ -z ${1:-} || $t == *"$1"* ]] || continue
    kb=${t%%:*} km=${t##*:}
    log="$ev/${kb//\//_}-$km.log"
    if qmk --no-color compile -kb "$kb" -km "$km" "${@:2}" >"$log" 2>&1; then r=OK; else r=FAIL; fail=1; fi
    size=$(grep -o 'firmware size is .*' "$log" | head -1)
    warn=$(grep -c 'warning:' "$log")
    printf '%-4s %-28s warnings=%s %s\n' "$r" "$t" "$warn" "${size:-}" | tee -a "$ev/summary.txt"
done
echo "evidence=$ev"
exit "$fail"
