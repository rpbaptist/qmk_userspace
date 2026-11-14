---
name: verify
description: Prove changes to this QMK userspace (crkbd, Iris, CU7, Planck, DZ60 keymaps) work. Compiles each keymap and records size and warnings, and runs host-side gtest journeys for keymap logic (crkbd SFT_EXT) in the QMK test harness. Use after any change under keyboards/ or tests/, before you commit, and when the user asks if a keymap builds or behaves correctly.
---

# Verify qmk_userspace

The user touches a physical keyboard. An agent cannot press keys, so this skill proves changes on two surfaces that an agent can drive:

1. **Build:** compile the keymap with the real firmware. Proves that it compiles with no warnings and fits in flash.
2. **Host test:** run keymap logic in QMK's gtest harness (`qmk_firmware/tests/test_common`). The harness gives key presses with real timing to the QMK core and records each HID report. This proves the behaviour of the key.

The third surface is the keyboard. It needs the user. Flash only when the user tells you to. Refer to [features/hardware-flash.md](features/hardware-flash.md).

Read [features/README.md](features/README.md) for the feature map. Each feature file is the recipe for that feature.

## Layout

- `qmk_firmware`: `qmk config -ro user.qmk_home` (now `~/code/qmk_firmware`, QMK 0.34.x). The overlay is set: `user.overlay_dir` = this repo.
- Keymaps: `keyboards/<kb>/keymaps/<km>/`.
- Host tests: `tests/<name>/` in this repo, with `test.mk`, `config.h` and `test_*.cpp`. The names cannot contain `-`.
- Helpers: `.claude/skills/verify/scripts/`. All are executable. Run them from any directory in the repo.
- Evidence: `~/.local/share/doc/qmk_userspace/verify/<UTC time>-<label>/`. Set `VERIFY_EVIDENCE_ROOT` to change it. Cleanup never deletes it.

## Launch

There is no server. Launch is a compile.

```bash
.claude/skills/verify/scripts/build-all.sh          # all maintained keymaps
.claude/skills/verify/scripts/build-all.sh crkbd    # targets that contain "crkbd"
STALE=1 .claude/skills/verify/scripts/build-all.sh  # also the stale keymaps
```

Ready: each line starts with `OK`, shows `warnings=0`, and shows the firmware size. The exit code is 0 only if all selected targets compile. The script also writes the `.hex` to the repo root, which `.gitignore` ignores.

To change the target list, edit `targets` in `build-all.sh`. The board revisions come from the keymap READMEs (`crkbd/rev1`, `keebio/iris/rev2`).

## Doctor

Run this first, and again when something looks wrong. It changes nothing.

```bash
qmk userspace-doctor | tail -3     # must say "Userspace enabled: True"
git -C "$(qmk config -ro user.qmk_home | cut -d= -f2 | sed 's/ (.*)$//')" describe --tags
ls "$(qmk config -ro user.qmk_home | cut -d= -f2 | sed 's/ (.*)$//')"/tests | grep rpbaptist_ || echo "no stale scaffolding"
lsusb | grep 4653:0001              # Corne is connected (only for hardware work)
```

If `rpbaptist_*` directories exist in `qmk_firmware/tests`, a test run is active or was interrupted. Do not start a second run. Run `cleanup.sh` only if no other agent is running a test.

## Drive

### Build

`build-all.sh` (above). For one keymap with a theme or option: `THEME=laser qmk compile -kb crkbd/rev1 -km rpbaptist`.

### Host test

```bash
.claude/skills/verify/scripts/run-test.sh crkbd_sft_ext                 # all tests
.claude/skills/verify/scripts/run-test.sh crkbd_sft_ext 'SftExt.Hold*'  # gtest filter
```

The script copies `tests/<name>` to `qmk_firmware/tests/rpbaptist_<name>`, because QMK finds tests only as real files there. It runs `make test:rpbaptist_<name>`, writes evidence, and removes the copy on exit. Only one run of a given test at a time: the script stops if the copy already exists.

To add a host test for new logic:

- Put the logic in its own `.c` file that does not need RGB, OLED or the `keymaps` array, as `sft_ext.c` does. The harness cannot link `keymap.c`, `rgb.c` or `oled.c`.
- Copy `tests/crkbd_sft_ext/`. In `test.mk`, add the `.c` file to `SRC`. Keep `QMK_KEYBOARD_H` and the full-path header defines.
- Do not add `-I<keymap dir>` to `OPT_DEFS`. The harness then finds the real `keymap.c` in place of its stub and the build fails.
- In the test keymap, map every position that a test presses on each layer that can be active. Use `KC_TRNS` where the real keymap has `_______`. A missing key makes the fixture print `no key is mapped for layer N`.
- Define `process_record_user` in the test so that it calls the logic, in an `extern "C"` block.

## Evidence

Each run writes a new directory:

- `build-all.sh`: `summary.txt` (one line for each target), one full compile log for each target, `commit`, `dirty` (`git status --short`).
- `run-test.sh`: `build.log`, `test.log` (gtest output), `result.xml` (JUnit), `commit`.

Proof standards:

- Quote the `summary.txt` line or the gtest `[  PASSED  ] N tests.` line, with the evidence path.
- A host test proves behaviour only if it fails when the logic is wrong. When you add or change a test, break the logic one time (for example, change a condition), run the test, see it fail, and restore the file with `git checkout -- <file>`. Record which tests failed.
- A host test asserts HID reports and layer state. It does not cover RGB, OLED, split transport, or matrix and debounce timing. Do not claim that it does. Those need the hardware.
- If the working tree is dirty, `dirty` in the build evidence shows it. State it when you report.
- For size changes, compare the free bytes with a build of the base commit.

## Cleanup

```bash
.claude/skills/verify/scripts/cleanup.sh
```

It removes only `qmk_firmware/tests/rpbaptist_*` (scaffolding from an interrupted `run-test.sh`) and shows `git status` of `qmk_firmware/tests`, which must be empty. It keeps the evidence. Build output in `<checkout>/.build` is cache; leave it. `lib.sh` and `mise.toml` set it with `MAKEFLAGS=BUILD_DIR=...`. Run cleanup after each failed or interrupted run.

## Known state

- `planck/rev6:rpbaptist` and `dz60:marina` do not compile on current QMK (`KC_LBRACKET` and other old keycode names; `RGBLIGHT_LED_COUNT` redefined). `build-all.sh` skips them unless `STALE=1`. The Planck revision is a guess.
- `qmk.json` has an empty `build_targets`, so GitHub CI builds nothing.
