# Hardware flash

Flashing puts a build on the user's keyboard, so the user can check the behaviour that the host cannot show: RGB, OLED, split halves, debounce and the full keymap. It replaces the firmware that the user types with. Do it only when the user tells you to.

## Sub-features

- `flash-crkbd`: flash each half of the Corne (QMK DFU, `EE_HANDS`).
- `flash-iris`: flash the left half of the Iris rev2 (QMK DFU).
- `user-check`: the user does a feature journey and reports the result.

## How to get to it (user POV)

- The user runs `qmk flash`, then presses the reset button on each half when QMK waits for the bootloader.

## Driving it with qmk flash and the user

Preconditions:

- The user agreed to flash this build now.
- `build-all.sh <kb>` shows `OK` for the same commit.
- `lsusb | grep 4653:0001` shows the Corne (before the flash).

- **Flash Corne.** Run `qmk flash -bl dfu -kb crkbd/rev1 -km rpbaptist` (add `THEME=<theme>` before it if the user uses one). Output waits for the bootloader. Ask the user to press reset on one half. Output shows the DFU write and `Validating... Success`. Repeat for the other half.
- **Flash Iris.** Run `qmk flash -kb keebio/iris/rev2 -km rpbaptist`. Ask the user to press reset on the left half.
- **Device back.** Run `lsusb | grep 4653:0001`. The Corne shows again.
- **User check.** Give the user the steps from the feature file (for example [crkbd-layers.md](./crkbd-layers.md)). Record their answer.
- **Proof.** The flash log, the commit (`git rev-parse HEAD`), and the user's answer.

## Gotchas

- `EE_HANDS`: each half has its handedness in EEPROM. A flash does not change it, but `EE_CLR` does. After `EE_CLR`, flash the correct half with `qmk flash -bl dfu-split-left` or `-bl dfu-split-right` (plus `-kb crkbd/rev1 -km rpbaptist`).
- The Iris rev2 target expects Caterina and 12 LEDs, but this board has QMK DFU and 18 LEDs. Refer to the Iris README before you flash.
- `4653:0001` is the Corne in normal firmware. While `qmk flash` waits, the half is in the DFU bootloader, which has a different USB ID, so `lsusb | grep 4653:0001` shows nothing then.
- You cannot press reset. If the user is not there, stop and report "built, not flashed".
