# Iris gamepad

I use the left half of an Iris rev2 alone as a gaming pad. The right half is
not connected. My daily driver is a crkbd, so this keymap uses the letter
positions and the debounce of my crkbd gaming layers. I can play on either
keyboard without remapping keys in games.

## Hardware

- Iris rev2 PCB, left half only
- QMK DFU bootloader (the stock rev2 target expects Caterina)
- 18 underglow LEDs, 9 per half (the stock rev2 target expects 12)

The firmware is still a split build. When the right half is missing, the
master marks it as disconnected after one failed transfer and tries the link
again only once per minute. This stops serial timeouts from stalling the
matrix scan.

## Layers

The inner key is the key above the thumb row. ▽ means that the key from the
layer below is used.

### COLEMAK

Default layer. The letters are in the same positions as my crkbd `GAMING`
layer, so the key bindings I use in games work on both keyboards. Movement is
on FRST (ESDF on QWERTY), which also gives easy access to the keys around it.

```
┌──────┬──────┬──────┬──────┬──────┬──────┐
│ ESC  │  1   │  2   │  3   │  4   │  5   │
├──────┼──────┼──────┼──────┼──────┼──────┤
│  `   │  Q   │  W   │  F   │  P   │  B   │
├──────┼──────┼──────┼──────┼──────┼──────┤
│ TAB  │  A   │  R   │  S   │  T   │  G   │
├──────┼──────┼──────┼──────┼──────┼──────┼──────┐
│ CTRL │  Z   │  X   │  C   │  D   │  V   │SHIFT │
└──────┴──────┴──────┴─┬────┴─┬────┴─┬────┴─┬────┘
                       │ ALT  │ EXT  │SPACE │
                       └──────┴──────┴──────┘
```

### QWERTY

Same as the crkbd `WASD` layer: QWERTY, moved one column to the right so
WASD sits under the same fingers as FRST. I use this for games where I want to
start without rebinding everything. All keys that are not letters are the
same as on `COLEMAK`.

```
┌──────┬──────┬──────┬──────┬──────┬──────┐
│ ESC  │  1   │  2   │  3   │  4   │  5   │
├──────┼──────┼──────┼──────┼──────┼──────┤
│  `   │  T   │  Q   │  W   │  E   │  R   │
├──────┼──────┼──────┼──────┼──────┼──────┤
│ TAB  │  G   │  A   │  S   │  D   │  F   │
├──────┼──────┼──────┼──────┼──────┼──────┼──────┐
│ CTRL │  B   │  Z   │  X   │  C   │  V   │SHIFT │
└──────┴──────┴──────┴─┬────┴─┬────┴─┬────┴─┬────┘
                       │ ALT  │ EXT  │SPACE │
                       └──────┴──────┴──────┘
```

To toggle it, hold `EXT` and press the top-left key (`T_QWERT`). QWERTY is
not saved: the pad always starts in Colemak.

### GAMING_EXT

Hold `EXT` (second thumb key). It is a layer key only: a tap sends nothing.

```
┌──────┬──────┬──────┬──────┬──────┬──────┐
│T_QWRT│  6   │  7   │  8   │  9   │  0   │
├──────┼──────┼──────┼──────┼──────┼──────┤
│ GUI  │  F1  │  F2  │  F3  │  F4  │  F5  │
├──────┼──────┼──────┼──────┼──────┼──────┤
│ CAPS │  F6  │  F7  │  F8  │  F9  │ F10  │
├──────┼──────┼──────┼──────┼──────┼──────┼──────┐
│ ENT  │  H   │  I   │  J   │  K   │  L   │  M   │
└──────┴──────┴──────┴─┬────┴─┬────┴─┬────┴─┬────┘
                       │  ▽   │ [EXT]│  ▽   │
                       └──────┴──────┴──────┘
```

## RGB

The underglow shows the active layer:

- Cyan: `COLEMAK`
- Red: `QWERTY`
- Purple: `GAMING_EXT`

## Timings

Same as the crkbd: `DEBOUNCE 35` with `sym_eager_pk`. A low debounce value
let switch bounce toggle QWERTY on and off again. The keymap also sets
`TAPPING_TERM 140` with `PERMISSIVE_HOLD`, but no key uses tap-hold now.

## Building

`qmk flash -kb keebio/iris/rev2 -km rpbaptist`

Press the reset button on the PCB once when the flash command waits for the
bootloader.
