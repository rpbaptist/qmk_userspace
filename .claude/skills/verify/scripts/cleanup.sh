#!/usr/bin/env bash
# Remove scaffolding that an interrupted run-test.sh left in qmk_firmware/tests.
# Keeps the evidence directory.
set -euo pipefail
source "$(dirname "$0")/lib.sh"
for d in "$QMK_HOME"/tests/rpbaptist_*; do
    [[ -e $d ]] || continue
    echo "remove $d"
    rm -rf "$d"
done
git -C "$QMK_HOME" status --short tests
