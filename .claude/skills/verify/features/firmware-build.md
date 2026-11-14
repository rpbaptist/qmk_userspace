# Firmware build

Each maintained keymap compiles against the local `qmk_firmware` with no warnings (QMK builds with `-Werror`), and the firmware fits in flash. The crkbd has the least free space, so a change there can fail on size.

## Sub-features

- `build-ok`: the keymap compiles.
- `build-clean`: the compile log has no warnings.
- `build-size`: the firmware fits, and the free bytes are known.
- `build-theme`: crkbd compiles with each `THEME` (godspeed, godspeed_ares, pulse, laser, milkshake, awaken).

## How to get to it (user POV)

- The user runs `qmk compile` or `qmk flash` for a keymap.
- GitHub CI runs on push, but `qmk.json` has no `build_targets`, so CI builds nothing now.

## Driving it with build-all.sh

Preconditions:

- `qmk userspace-doctor` reports `Userspace enabled: True`.

- **All maintained keymaps.** Run `.claude/skills/verify/scripts/build-all.sh`. It builds crkbd, Iris and CU7 (`rpbaptist`). Each line shows `OK`, `warnings=0` and the size. The exit code is 0.
- **One keyboard.** Run `.claude/skills/verify/scripts/build-all.sh crkbd`. Only the crkbd line shows.
- **Size change.** Run `build-all.sh crkbd` on the change, then on the base commit in a worktree. Compare the free bytes. Do not use an old number as the baseline; build the base commit.
- **Themes.** For each theme, run `THEME=<theme> qmk --no-color compile -kb crkbd/rev1 -km rpbaptist`. Each shows `firmware size is fine`. `awaken` is the largest (2162 bytes free at `1da4a70`).
- **Proof.** Quote the `summary.txt` lines and give the evidence path.

## Gotchas

- `planck/rev6:rpbaptist` and `dz60:marina` fail on current QMK. `build-all.sh` skips them unless `STALE=1`. Do not count those failures against a change in other keymaps.
- Each compile writes a `.hex` to the repo root. `.gitignore` ignores it. Do not commit it.
- `qmk compile` without `--no-color` puts ANSI codes in the log, which stops the warning count.
- `THEME` changes only the crkbd. Other keymaps ignore it. An unknown `THEME` name gives no error; the build uses the default theme.
- The skill is not tracked (the global `~/.gitignore` ignores `.claude`), so a worktree has no `.claude/skills/verify`. In a worktree, run the helpers by their path in the main checkout, for example `~/code/qmk_userspace/.claude/skills/verify/scripts/build-all.sh crkbd`. `lib.sh` uses the checkout of the current directory.
- Near the flash limit, the size line says `approaching the maximum` in place of `is fine`. `build-all.sh` still shows `OK`; read the free bytes.
- `capsunlocked/cu7:default` is the stock keymap. `build-all.sh` does not build it.
