// Copyright 2023 Your Name (@rpbaptist)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "rpbaptist.h"

#ifdef OLED_ENABLE
#    define LABEL(text) .label = text,
#else
#    define LABEL(text)
#endif

#ifdef RGB_MATRIX_ENABLE
#    define COLOR(...) .color = {__VA_ARGS__},
#else
#    define COLOR(...)
#endif

const layer_display_t layer_display[] PROGMEM = {
    [_COLEMAKDH]  = {LABEL("TYPE ") COLOR(THEME_HSV)},
    [_GAMING]     = {LABEL("GAME ") COLOR(HSV_RED)},
    [_WASD]       = {LABEL("WASD ") COLOR(HSV_RED)},
    [_GAMING_EXT] = {LABEL("Ext  ") COLOR(HSV_PURPLE)},
    [_NUMPAD]     = {LABEL("Num  ") COLOR(HSV_CORAL)},
    [_SYM]        = {LABEL("Sym  ") COLOR(HSV_YELLOW)},
    [_NAV]        = {LABEL("Nav  ") COLOR(HSV_SPRINGGREEN)},
    [_UTIL]       = {LABEL("Util ") COLOR(HSV_PINK)},
};
_Static_assert(ARRAY_SIZE(layer_display) == _UTIL + 1, "layer_display needs one row for each layer");
