# crkbd layers

The crkbd has three base layers: TYPING (Colemak-DH), GAMING (Colemak-DH with plain modifiers) and WASD (QWERTY positions shifted one column). The user selects them on the UTIL layer. UTIL is a tri-layer: SYM + NAV on TYPING, or GAMING_EXT + NAV on GAMING and WASD. The OLED and the underglow show the active layer.

## Sub-features

- `base-select`: TYPING, GAMING and WASD keys on UTIL change the base layer. The OLED shows `TYPE`, `GAME` or `WASD`.
- `tri-typing`: SYM + NAV gives UTIL on TYPING.
- `tri-gaming`: GAMING_EXT + NAV gives UTIL on GAMING and WASD.
- `rgb-indicator`: the underglow colour follows the layer (yellow SYM, spring green NAV, pink UTIL, coral NUMPAD, purple GAMING_EXT, red GAMING and WASD).
- `boot-default`: after a power cycle, the base layer is TYPING.

## How to get to it (user POV)

- Hold SYM and NAV (the two middle thumbs, between ALT/GUI and the SHIFT thumbs) to reach UTIL. Press `TYPING`, `GAMING` or `WASD` in the right column of the left half.

## Driving it with the user and the keyboard

Preconditions:

- The firmware under test is on the keyboard (refer to [hardware-flash.md](./hardware-flash.md)).
- The underglow layer indicator is on (`RGB_UND` toggles it).

- **Build.** Run `.claude/skills/verify/scripts/build-all.sh crkbd`. Shows `OK`.
- **Select GAMING.** Ask the user to hold SYM + NAV and press `GAMING`. The OLED shows `GAME` and the underglow is red.
- **Tri-layer in GAMING.** Ask the user to hold `SFT_EXT` after a tap (GAMING_EXT on), then hold NAV. The OLED shows `Util` and the underglow is pink.
- **Back to TYPING.** Ask the user to press `TYPING` on UTIL. The OLED shows `TYPE`.
- **Boot default.** Ask the user to unplug and connect the keyboard. The OLED shows `TYPE`.
- **Proof.** Record what the user saw, in their words, and the flashed commit.

## Gotchas

- There is no host test. `layer_state_set_user` and the base-layer keys are in `keymap.c`, which the harness cannot link.
- During RGB idle, the underglow shows the animation, not the layer colour. Press a key first.
- The OLED that shows layers is the master half (the half with the USB cable).
