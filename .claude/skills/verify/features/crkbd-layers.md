# crkbd layers

The crkbd has three base layers: TYPING (Colemak-DH), GAMING (Colemak-DH with plain modifiers) and WASD (QWERTY; on the left half, T, G and B move to the first letter column and the other letters move one column to the right). The user selects them on the UTIL layer. UTIL is a tri-layer: SYM + NAV on TYPING, or GAMING_EXT + NAV on GAMING and WASD. `LCK_NMP` on UTIL locks NUMPAD on and off. The OLED and the underglow show the active layer.

## Sub-features

- `base-select`: TYPING, GAMING and WASD keys on UTIL change the base layer. The OLED shows `TYPE`, `GAME` or `WASD`.
- `tri-typing`: SYM + NAV gives UTIL on TYPING.
- `tri-gaming`: GAMING_EXT + NAV gives UTIL on GAMING and WASD.
- `numpad-lock`: `LCK_NMP` locks NUMPAD on. A second tap unlocks it.
- `rgb-indicator`: the layer indicator follows the layer (yellow SYM, spring green NAV, pink UTIL, coral NUMPAD, purple GAMING_EXT, red GAMING and WASD).
- `boot-default`: after a power cycle, the base layer is TYPING, with RGB on and with RGB off.

## How to get to it (user POV)

- On TYPING, hold SYM and NAV (the two middle thumbs, between ALT/GUI and the SHIFT thumbs) to reach UTIL. Press `TYPING`, `GAMING` or `WASD` in the right column of the left half.
- On GAMING and WASD, the left middle thumb is `SFT_EXT`. Tap it, hold it (GAMING_EXT on), then hold NAV to reach UTIL.

## Driving it with run-test.sh and the keyboard

Preconditions:

- `qmk_firmware/tests` has no `rpbaptist_crkbd_keys` directory.
- Source: `keyboards/crkbd/keymaps/rpbaptist/key_behaviour.c`. The `base_layers` table has one row for each base layer: select key, tri-layer lower layer and RGB profile. The hooks `process_record_user`, `layer_state_set_user` and `keyboard_post_init_user` read it. `layer_display.c` has one row for each layer: the OLED label and the layer indicator color. `oled.c` and `rgb.c` read it.
- Test: `tests/crkbd_keys/test_layers.cpp`.
- Hardware steps only: the firmware under test is on the keyboard (refer to [hardware-flash.md](./hardware-flash.md)).
- Hardware steps only: the underglow layer indicator is on (`RGB_UND` toggles it).

- **Host test.** Run `.claude/skills/verify/scripts/run-test.sh crkbd_keys 'Layers.*'`. Output ends with `[  PASSED  ] 11 tests.` and `status=0`.

  | Sub-feature | Test |
  | --- | --- |
  | `base-select` | `TypingSelectsColemakBase`, `GamingSelectsGamingBase`, `WasdSelectsWasdBase` |
  | `tri-typing` | `SymAndNavGiveUtilOnTyping`, `GamingExtAndNavGiveNoUtilOnTyping` |
  | `tri-gaming` | `GamingExtAndNavGiveUtilOnGaming`, `GamingExtAndNavGiveUtilOnWasd`, `SymAndNavGiveNoUtilOnGaming`, `SymAndNavGiveNoUtilOnWasd` |
  | `boot-default` | `BootSelectsColemakBase` |
  | `numpad-lock` | `LckNmpTogglesNumpadLock` |

- **Test catches defects.** Make these changes in `key_behaviour.c`:
  - Change `.tri_lower` of the `_COLEMAKDH` row to `_GAMING_EXT`.
  - Change `.tri_lower` of the `_GAMING` row and of the `_WASD` row to `_SYM`.
  - In `keyboard_post_init_user`, change `base_layers[0]` to `base_layers[1]`.

  Run the host test. All 7 tri-layer and boot tests fail (the `tri-typing`, `tri-gaming` and `boot-default` rows of the table above). Restore with `git checkout -- keyboards/crkbd/keymaps/rpbaptist/key_behaviour.c`.
- **Build.** Run `.claude/skills/verify/scripts/build-all.sh crkbd`. Shows `OK`.
- **Select GAMING.** Ask the user to hold SYM + NAV and press `GAMING`. The OLED shows `GAME` and the underglow is red.
- **Tri-layer in GAMING.** Ask the user to hold `SFT_EXT` after a tap (GAMING_EXT on), then hold NAV. The OLED shows `Util` and the underglow is pink.
- **Back to TYPING.** Ask the user to press `TYPING` on UTIL. The OLED shows `TYPE`.
- **Boot default.** Ask the user to unplug and connect the keyboard. The OLED shows `TYPE`.
- **Proof.** Record what the user saw, in their words, and the flashed commit.

## Gotchas

- The host test does not cover `rgb-indicator` or the OLED. The host build has no RGB matrix and no OLED. `BootSelectsColemakBase` calls `keyboard_post_init_user` directly. It does not do a power cycle.
- During RGB idle, the underglow shows the animation, not the layer indicator. Press a key first.
- The OLED that shows layers is the master half (the half with the USB cable).
