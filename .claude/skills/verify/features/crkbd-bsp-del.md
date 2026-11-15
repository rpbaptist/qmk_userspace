# crkbd BSP_DEL key

`BSP_DEL` is the top right key on the TYPING layer. A tap sends BACKSPACE. With one SHIFT held, it sends DEL without SHIFT. With both SHIFT keys held, it sends SHIFT + DEL. GAMING and WASD have a plain BACKSPACE at that position, and GAMING_EXT has DEL.

## Sub-features

- `bsp`: a tap sends BACKSPACE.
- `del-one-shift`: one SHIFT held sends DEL, and the host sees no SHIFT.
- `del-two-shift`: both SHIFT keys held send SHIFT + DEL.

## How to get to it (user POV)

- On the TYPING layer, press the top right key. Hold a SHIFT thumb key (`SFT_SPC` or `SFT_ENT`) for DEL.

## Driving it with run-test.sh and the keyboard

Preconditions:

- `qmk_firmware/tests` has no `rpbaptist_crkbd_keys` directory.
- Source: `BSP_DEL` in `process_record_user` in `keyboards/crkbd/keymaps/rpbaptist/key_behaviour.c`. Test: `tests/crkbd_keys/test_bsp_del.cpp`.
- Hardware steps only: the firmware under test is on the keyboard (refer to [hardware-flash.md](./hardware-flash.md)).
- Hardware steps only: the OLED on the master half shows `TYPE`.

- **Host test.** Run `.claude/skills/verify/scripts/run-test.sh crkbd_keys 'BspDel.*'`. Output shows `status=0` and ends with `[  PASSED  ] 3 tests.`

  | Sub-feature | Test |
  | --- | --- |
  | `bsp` | `TapSendsBackspace` |
  | `del-one-shift` | `OneShiftSendsDeleteWithoutShift` |
  | `del-two-shift` | `TwoShiftsSendShiftDelete` |

- **Test catches defects.** Comment out `del_mods(saved_mods);` in `key_behaviour.c`. Run the host test. `OneShiftSendsDeleteWithoutShift` fails. Restore with `git checkout -- keyboards/crkbd/keymaps/rpbaptist/key_behaviour.c`.
- **Build.** Run `.claude/skills/verify/scripts/build-all.sh crkbd`. Shows `OK`.
- **Backspace.** Ask the user to type `abc` in a text field and tap the key. The field shows `ab`.
- **Delete.** Ask the user to put the cursor before `b`, hold one SHIFT thumb, and tap the key. The field shows `a` and no text is selected.
- **Two SHIFT keys.** Ask the user to hold the two SHIFT thumbs and tap the key. The result depends on the application (SHIFT + DEL is cut in some).
- **Proof.** Record what the user saw, in their words, and the flashed commit.

## Gotchas

- The host test uses plain `KC_LSFT` and `KC_RSFT`, not the mod-tap SHIFT thumbs. The mod-tap timing needs the hardware.
- The SHIFT thumbs are mod-tap keys. The user must hold them longer than the tapping term (`SFT_SPC`: `TAPPING_TERM`; `SFT_ENT`: `TAPPING_TERM - 20`, from `get_tapping_term` in `key_behaviour.c`), or press the key during the hold (`PERMISSIVE_HOLD`).
