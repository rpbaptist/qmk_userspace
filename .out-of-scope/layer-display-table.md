# One layer table for crkbd OLED names and underglow colours

The crkbd `rpbaptist` keymap keeps two tables indexed by layer: the OLED
layer labels in `oled.c` (`layer_labels`, `base_labels`), and the layer
indicator colours in `rgb.c` (`layer_colors`). We do not merge them into one
table.

## Why this is out of scope

The two tables do not cover the same set of layers, so they are not
duplicates that must stay in step:

- The OLED name line reads `layer_state` only. Layer 0 shows blank, and
  other layers show `Unkn`. The base layer (TYPE, GAME, WASD) is a separate
  line that reads `default_layer_state`.
- The indicator reads `layer_state | default_layer_state`. `_GAMING` and
  `_WASD` show red, and `_COLEMAKDH` shows `THEME_HSV`.

One table indexed by layer needs placeholder names for the base layers and a
separate rule for the base layer line. Each table is local and easy to read. The gain is
small, and the original request calls it speculative.

## When to reconsider

Reconsider when a new momentary layer is added, or when the OLED name and
the indicator colour of a layer drift apart. If a new base layer is added,
decide its OLED label and colour together with the base layer table from #27.

## Prior requests

- #28: "Use one layer table for crkbd OLED names and underglow colours"
