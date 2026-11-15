# Corne keyboard layout

This is my highly personalized CRKBD layout. I use Colemak DH as main typing layer. I use this keyboard and layout in my daily programming job and for everything at home, including gaming.

Many thanks to foostan for the keyboard, all QMK contributors and drashna specifically for his work on this keyboard.

## Layers

### COLEMAKDH

Main typing layer. I really love having SHIFT available on thumbs. I am a left thumb space bar person, so I put SPACE there and ENTER on right. I use CTRL key combos lot in my text editor and desktop environment. That's why I have two of them. I also found them to be a great position for the square brackets. (`[` and `]`). These become parenthesis (`(` and `)`) on either of the layers.

Holding SHIFT while tapping BACKSPACE will output DEL. Holding both SHIFT keys will output SHIFT+DEL.

I don't know about you but "lower" and "raise" don't mean that much to me. I named my layers `SYM`, short for symbols and `NAV`, short for navigation. Holding the key will activate the layer for as long as it is held, double tapping it will switch to it. Single tapping it once will switch to main layer again.

Holding TAB will access `NUMPAD` layer which features a numpad. This is lockable by going to `UTIL` layer and pressing the same key.

### SYM

Includes a number row, the symbols normally on SHIFT and numbers and a variety of other symbols used mostly for programming.

### NAV

This is where I access F keys, arrow keys, page up, down, home, end and F keys. I also added general tab forward and backward keys. Tab close and window close. I primarily use these in my browser, terminal and text editor.

### UTIL

Short for utility. This is accessed by pressing both `SYM` and `NAV` at the same time. It has a software `QK_BOOT` key, control for RGB and switching the main layer. More on the RGB below.

### NUMPAD"

This gives access to a numpad on the right half with some additional symbols I find useful when using the numpad. The left half has media control.

### GAMING

Sometimes I switch to type in Discord, Steam, or in game chat, so I think it's more important to keep typing functional than put WASD on where I want movement to be. This means I rebind my keys in all games. It's worth it to me.

I put movement on FRST (would be ESDF on QWERTY), because it lines up more comfortably with the columnar staggered layout of the Corne. It also gives more easy access to all keys on left half.

Most notable here is that there are no tap and hold differences on the left hand side. The left thumb keys are ALT, `SFT_EXT` and SPACE. `SFT_EXT` sends SHIFT immediately on press, so there is no delay when you hold it. Double tap it to activate `GAMING_EXT` for the next key only: the second tap sends no SHIFT. Tap and hold to keep `GAMING_EXT` on while held. Press it once more to cancel and return to the base layer. An unused one-shot layer turns off after 1 second. This is a different layer than `SYM`.

### WASD

This is functionally the same as the `GAMING` layer, but uses traditional WASD layout. This is done for games where I don't need to type and want to just jump in without rebinding everything.

### GAMING_EXT

This is the gaming extended layer. It is a one-shot layer: double tap `SFT_EXT` and the next key comes from this layer. Num row is now moved to the home row. The bottom row has `H I J K L` and the inner left thumb has `M`, the same letters as on my Iris pad. The right thumb gives an ENTER key which does not switch. This is good when a game requires you to hold ENTER.

To reach `UTIL` from a gaming layer, first hold `NAV`, then double tap `SFT_EXT`.

## RGB

I took a big inspiration from Drashna's RGB configuration and tweaked it.

These keys work only with `RGB_MATRIX_ENABLE = yes`. If you build with RGB matrix off, the keys stay on the `UTIL` layer but do nothing.

- `RGB_UND`: Turns the layer indicator on and off. Each layer has its own color.
- `RGB_IDL`: Turns idle animation on and off. With idle animation on, the active effect shows while you type. After the idle timeout, the idle effect shows. The gaming profile (`GAMING`, `WASD`) uses a shorter timeout and `RGB_MATRIX_RAINBOW_PINWHEELS` as the idle effect.
   When you turn idle animation off, the idle effect shows.
   While the idle effect shows, the layer indicator is hidden.
- `RGB_ATG`: Cycles through three active effects: `RGB_MATRIX_SOLID_REACTIVE_SIMPLE` -> `RGB_MATRIX_TYPING_HEATMAP` -> `RGB_MATRIX_SOLID_REACTIVE_MULTINEXUS`
- `RGB_PST`: Cycles through three idle effects: `RGB_MATRIX_SOLID_COLOR` -> `RGB_MATRIX_BREATHING` -> `RGB_MATRIX_CYCLE_ALL`
- `RGB_PCT`: Cycles through two, more colorful, idle effects: `RGB_MATRIX_RAINBOW_PINWHEELS` -> `RGB_MATRIX_CYCLE_LEFT_RIGHT`

## Timings

`DEBOUNCE 20` with `asym_eager_defer_pk`. A press sends at once. A release
sends only after the switch stays open for 20 ms, so a bounce cannot send a
false release and press.

We do not use `sym_eager_pk`: on this hot-swap board it needs 35 ms to stop
chatter, which makes each tap at least 35 ms long.

If chatter comes back, check the hot-swap sockets of the keys that chatter
before you raise `DEBOUNCE`.

## Building

`qmk flash -bl dfu -kb crkbd -km rpbaptist`

## Notes

I use several OLED slave side images, depending on the keycaps I am using. These also determine the default LED color and underglow.

Provide the theme like this:

`THEME=laser qmk flash -bl dfu -kb crkbd -km rpbaptist`

The themes are the `THEME` values in `rules.mk`. The default is pulse.
