# Iris gamepad

I use the left half of an Iris rev2 alone as a gaming pad. The right half is
not connected. My daily driver is a crkbd, so this keymap copies the gaming
layers and the key timings of my crkbd keymap. I can play on either keyboard
without remapping keys in games.

## Hardware

- Iris rev2 PCB, left half only
- QMK DFU bootloader (the stock rev2 target expects Caterina)
- 18 underglow LEDs, 9 per half (the stock rev2 target expects 12)

The firmware is still a split build. When the right half is missing, the
master marks it as disconnected after one failed transfer and tries the link
again only once per minute. This stops serial timeouts from stalling the
matrix scan.

## Layers

### COLEMAK

Default layer. The letters are in the same positions as my crkbd `GAMING`
layer. I type in Discord, Steam or in-game chat, so typing must keep working.
This means I rebind keys in all games. Movement is on FRST (ESDF on QWERTY),
which also gives easy access to the keys around it.

ESC is to the left of `1`. Grave (and tilde with SHIFT) is in the old ESC
position.

### QWERTY

Same as the crkbd `WASD` layer: QWERTY, moved one column to the right so
WASD sits under the same fingers as FRST. I use this for games where I do not
type and want to start without rebinding everything.

To toggle it, hold `EXTALT` and press the top-left key. QWERTY is not saved:
the pad always starts in Colemak.

### GAMING_EXT

Hold the left thumb key `EXTALT` (tap for ALT). This layer has `6`-`0` on the
number row, F1-F11, ALT, CAPS LOCK, `[ ] \` and `J M`. The right thumb key is
an ENTER that does not change layers, for games where you must hold ENTER.

## RGB

The underglow shows the active layer:

- Cyan: `COLEMAK`
- Red: `QWERTY`
- Purple: `GAMING_EXT`

## Timings

Same as the crkbd: `TAPPING_TERM 140` with `PERMISSIVE_HOLD`, and
`DEBOUNCE 35` with `sym_eager_pk`. A low debounce value let switch bounce
toggle QWERTY on and off again.

## Building

`qmk flash -kb keebio/iris/rev2 -km rpbaptist`

Press the reset button on the PCB once when the flash command waits for the
bootloader.
