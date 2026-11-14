# crkbd BSP_DEL key

`BSP_DEL` is the top right key on the TYPING layer. A tap sends BACKSPACE. With one SHIFT held, it sends DEL without SHIFT. With both SHIFT keys held, it sends SHIFT + DEL.

## Sub-features

- `bsp`: a tap sends BACKSPACE.
- `del-one-shift`: one SHIFT held sends DEL, and the host sees no SHIFT.
- `del-two-shift`: both SHIFT keys held send SHIFT + DEL.

## How to get to it (user POV)

- On the TYPING layer, press the top right key. Hold a SHIFT thumb key (`SFT_SPC` or `SFT_ENT`) for DEL.

## Driving it with the user and the keyboard

Preconditions:

- The firmware under test is on the keyboard (refer to [hardware-flash.md](./hardware-flash.md)).
- The base layer is TYPING. The OLED on the master half shows `TYPE`.

- **Build.** Run `.claude/skills/verify/scripts/build-all.sh crkbd`. Shows `OK`.
- **Backspace.** Ask the user to type `abc` in a text field and tap the key. The field shows `ab`.
- **Delete.** Ask the user to put the cursor before `b`, hold one SHIFT thumb, and tap the key. The field shows `a` and no text is selected.
- **Two SHIFT keys.** Ask the user to hold the two SHIFT thumbs and tap the key. The result depends on the application (SHIFT + DEL is cut in some).
- **Proof.** Record what the user saw, in their words, and the flashed commit.

## Gotchas

- There is no host test. The logic is in `process_record_user` in `keymap.c`, which the harness cannot link. To host-test it, move it to its own file first, as `sft_ext.c` does.
- The SHIFT thumbs are mod-tap keys. The user must hold them longer than the tapping term (`SFT_SPC`: `TAPPING_TERM`; `SFT_ENT`: `TAPPING_TERM - 20`, from `get_tapping_term` in `keymap.c`), or press the key during the hold (`PERMISSIVE_HOLD`).
