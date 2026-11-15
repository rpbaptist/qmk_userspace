# crkbd SFT_EXT key

`SFT_EXT` is the middle left thumb key (between ALT and SPACE) on the GAMING and WASD layers. A press sends SHIFT at once. A tap, then a second press within `TAPPING_TERM`, sends no SHIFT and turns on `GAMING_EXT` for the next key, or while the second press is held. A press while `GAMING_EXT` is on cancels it and sends SHIFT. An unused `GAMING_EXT` turns off after `ONESHOT_TIMEOUT`.

## Sub-features

- `sft-hold`: a press sends SHIFT at once, and keys pressed while it is held are shifted.
- `ext-oneshot`: a double tap sends no SHIFT, and the next key comes from `GAMING_EXT`. The layer then turns off.
- `ext-hold`: a tap, then a held second press, keeps `GAMING_EXT` on for each key until release.
- `ext-cancel`: a press while the one-shot layer is on cancels it and sends SHIFT.
- `ext-timeout`: an unused one-shot layer turns off after `ONESHOT_TIMEOUT`.
- `ext-slow`: a second tap after `TAPPING_TERM` is a normal SHIFT tap.
- `ext-interrupt`: a shifted key between taps stops the double tap.

## How to get to it (user POV)

- On the keyboard, select GAMING or WASD on the UTIL layer. Then use the middle left thumb key.

## Driving it with run-test.sh

Preconditions:

- `qmk_firmware/tests` has no `rpbaptist_crkbd_keys` directory.
- Source: `process_sft_ext` in `keyboards/crkbd/keymaps/rpbaptist/key_behaviour.c`. Test: `tests/crkbd_keys/test_sft_ext.cpp`. The tests go through the real `process_record_user`. Each test name in `SftExt.*` is the same as a sub-feature above.

- **All behaviours.** Run `.claude/skills/verify/scripts/run-test.sh crkbd_keys 'SftExt.*'`. Output ends with `[  PASSED  ] 7 tests.` and `status=0`.
- **One behaviour.** Run `.claude/skills/verify/scripts/run-test.sh crkbd_keys 'SftExt.UnusedExtTimesOut'`. One test runs.
- **Test catches defects.** Change `if (!interrupted) {` in `key_behaviour.c` to `if (false) {`. Run the test. `DoubleTapSkipsShiftAndArmsOneShotExt`, `PressWhileExtActiveCancelsAndSendsShift` and `UnusedExtTimesOut` fail (3 tests). `TapThenHoldKeepsExtWhileHeld` passes, because its key press sets `interrupted`. Restore with `git checkout -- keyboards/crkbd/keymaps/rpbaptist/key_behaviour.c`.
- **Firmware also builds.** Run `.claude/skills/verify/scripts/build-all.sh crkbd`. Shows `OK` and `warnings=0`.
- **Proof.** Quote the gtest result line and give the evidence path (`test.log`, `result.xml`).

## Gotchas

- The test keymap has only the positions that the tests press. If you add a test that presses another position, map it on each layer that can be active. Otherwise the fixture prints `no key is mapped for layer N`.
- The test uses `TAPPING_TERM` and `ONESHOT_TIMEOUT` from the keymap `config.h`. If those change, the tests follow. Do not hardcode milliseconds.
- The harness does not have debounce. On the keyboard, `DEBOUNCE 20` (`asym_eager_defer_pk`) makes very fast taps longer.
