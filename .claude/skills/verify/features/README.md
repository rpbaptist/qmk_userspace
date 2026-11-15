# qmk_userspace verification map

This directory is the maintained source to verify the user-facing behaviour of the keymaps in this userspace. Read this index before you drive anything. Then use the matching feature file as the recipe.

## Baseline preconditions

- `qmk userspace-doctor` reports `Userspace enabled: True`.
- `qmk_firmware/tests` has no `rpbaptist_*` directory.
- The working tree contains the change under test. Evidence records the commit and dirty files.
- Hardware work only: the keyboard is connected (`lsusb | grep 4653:0001` for the Corne), and the user agreed to flash.

## Driving conventions

- Use the helpers in `.claude/skills/verify/scripts/`. Do not run `make test:` by hand, because the helper stages and removes the scaffolding.
- Use the real board revision: `crkbd/rev1`, `keebio/iris/rev2`.
- Use the real keymap timings. Host tests include the keymap `config.h`, so `TAPPING_TERM` and `ONESHOT_TIMEOUT` are the same as on the keyboard.

## Proof and skip reporting

- Build proof: the `summary.txt` line (`OK`, `warnings=0`, size) and the evidence path.
- Behaviour proof: the gtest result line, the evidence path, and a record that the test failed when the logic was broken.
- Hardware proof: what the user saw, in their words, and the firmware commit that you flashed.
- If a feature has no host test, report it as "built, not verified". Do not report a build as behaviour proof.
- Report a skipped target with its reason (for example, stale keymap).

## Feature entry contract

Each feature file starts with an H1 title and one paragraph that describes the user-visible behaviour. Then it has exactly four H2 sections in this order:

1. `Sub-features` lists short IDs, with one line for each behaviour.
2. `How to get to it (user POV)` lists each way the user reaches it.
3. `Driving it with <harness>` starts with `Preconditions:`, and uses labelled bullets that pair each action with an exact command and an observable result.
4. `Gotchas` lists traps that can waste or invalidate a run.

## Features

- [Firmware build](./firmware-build.md): each keymap compiles with no warnings and fits in flash.
- [crkbd SFT_EXT key](./crkbd-sft-ext.md): gaming thumb key, SHIFT on hold, one-shot GAMING_EXT on double tap. Host-tested.
- [crkbd BSP_DEL key](./crkbd-bsp-del.md): SHIFT + BACKSPACE gives DEL. Host-tested.
- [crkbd layers](./crkbd-layers.md): TYPING, GAMING and WASD base layers, tri-layer UTIL, and NUMPAD lock. Host-tested, except `rgb-indicator` and the OLED.
- [crkbd RGB idle animation](./crkbd-rgb-effect.md): active and idle effects, idle timeout, profiles, cycle keys. Host-tested, except the OLED, the layer indicator and split sync.
- [Hardware flash](./hardware-flash.md): put a build on the keyboard and get the user's check.
