# Shared paths for the verify helpers. Source it; do not run it.
# The checkout or worktree of the current directory. If the current directory
# is not in a userspace checkout, use the checkout that holds this script.
USERSPACE=$(git rev-parse --show-toplevel 2>/dev/null) && [[ -f $USERSPACE/qmk.json ]] \
    || USERSPACE=$(git -C "$(dirname "${BASH_SOURCE[0]}")" rev-parse --show-toplevel)
QMK_HOME=$(qmk config -ro user.qmk_home | cut -d= -f2 | sed "s/ (.*)$//")
[[ -f $QMK_HOME/quantum/quantum.h ]] || { echo "Bad QMK home: $QMK_HOME" >&2; exit 2; }
# Each checkout builds in its own .build, also without the mise prompt hook.
# Same value as mise.toml.
BUILD_DIR="$USERSPACE/.build"
export MAKEFLAGS="${MAKEFLAGS:+$MAKEFLAGS }BUILD_DIR=$BUILD_DIR"
EVIDENCE_ROOT="${VERIFY_EVIDENCE_ROOT:-$HOME/.local/share/doc/qmk_userspace/verify}"

# The random suffix keeps runs that start in the same second apart.
new_evidence_dir() { # $1 = label
    mkdir -p "$EVIDENCE_ROOT"
    mktemp -d "$EVIDENCE_ROOT/$(date -u +%Y%m%dT%H%M%SZ)-$1-XXXX"
}
