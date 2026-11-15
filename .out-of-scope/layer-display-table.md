# One layer table for crkbd OLED names and underglow colours

The crkbd `rpbaptist` keymap keeps two switches on the layers: the OLED
layer name in `oled.c` `render_status`, and the layer indicator colour in
`rgb.c` `rgb_matrix_indicators_advanced_user`. We do not merge them into one
table indexed by layer.

## Why this is out of scope

The two switches do not cover the same set of layers, so they are not
duplicates that must stay in step:

- The OLED name line reads `layer_state` only. Layer 0 shows blank, and
  other layers show `Unkn`. The base layer (TYPE, GAME, WASD) is a separate
  line that reads `default_layer_state`.
- The indicator reads `layer_state | default_layer_state`. `_GAMING` and
  `_WASD` show red, and `_COLEMAKDH` shows `THEME_HSV` through the default.

A table indexed by layer needs placeholder names for the base layers and a
separate rule for the base layer line. Thus "a new layer needs one table
entry" is not fully true. Each switch is local and easy to read. The gain is
small, and the original request calls it speculative.

## When to reconsider

Reconsider when a new momentary layer is added, or when the OLED name and
the indicator colour of a layer drift apart. If a new base layer is added,
decide its OLED label and colour together with the base layer table from #27.

## Prior requests

- #28: "Use one layer table for crkbd OLED names and underglow colours"
