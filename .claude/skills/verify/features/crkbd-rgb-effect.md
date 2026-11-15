# crkbd RGB idle animation

With idle animation on, the crkbd shows the active effect while the user types. After the idle timeout with no key press, it shows the idle effect. A key press shows the active effect again. `RGB_ATG`, `RGB_PST` and `RGB_PCT` choose the effects. `TYPING`, `GAMING` and `WASD` apply a profile. `RGB_IDL` turns idle animation on and off. `RGB_RST` applies the typing profile of the theme. The terms are in `GLOSSARY.md`.

## Sub-features

- `idle-timeout`: the idle effect shows after the idle timeout, not before.
- `wake`: a key press shows the active effect, and the timeout starts again.
- `cycle`: the cycle order of `RGB_ATG`, `RGB_PST` and `RGB_PCT`, with idle animation on and off.
- `profile`: typing and gaming profiles store both effects and the timeout, with idle animation on and off.
- `reset`: after reset, the current effect is the active effect of the typing profile, for each theme.
- `invariant`: with idle animation on, the current effect is the active effect or the idle effect.
- `oled`, `layer-indicator`, `split-sync`: not host-tested. They need the keyboard.

## How to get to it (user POV)

- On the keyboard, use the RGB keys on the UTIL layer, or select a base layer there. Then wait, or press a key.

## Driving it with run-test.sh

Preconditions:

- `qmk_firmware/tests` has no `rpbaptist_crkbd_rgb_effect` directory.
- Source: `keyboards/crkbd/keymaps/rpbaptist/rgb_effect.c`. It talks to the RGB engine through `rgb_effect_engine.h`. The firmware links `rgb_effect_engine_qmk.c`. The test links a fake engine in `tests/crkbd_rgb_effect/test_rgb_effect.cpp` and moves the harness timer with `advance_time`.

- **All behaviours.** Run `.claude/skills/verify/scripts/run-test.sh crkbd_rgb_effect`. Output ends with `[  PASSED  ] 18 tests.` and `status=0`.
- **Test catches defects.** In `rgb_effect.c`, add `show(RGB_MATRIX_SOLID_REACTIVE_SIMPLE);` at the end of `rgb_effect_reset`. Run the test. `ResetStartsOnActiveEffectForEachTheme`, `ResetSetsSpeedOfActiveEffect` and `InvariantHoldsAfterEachOperation` fail. Restore the file.
- **Firmware also builds.** Run `.claude/skills/verify/scripts/build-all.sh crkbd`, and `qmk compile -kb crkbd/rev1 -km rpbaptist -e RGB_MATRIX_ENABLE=no`. Both show `OK`.
- **Proof.** Quote the gtest result line and give the evidence path.

## Gotchas

- The test config defines `RGB_MATRIX_ENABLE` only while it includes the keymap `config.h`, to get the effect settings. Then it undefines it, so the harness builds no RGB matrix code.
- The test includes `rgb_matrix.h` for the effect enum. It needs `RGB_MATRIX_LED_COUNT` from the test config and the rgb_matrix include dirs from `test.mk`.
- The test build uses the default theme. The reset test gives each theme's typing profile as data.
