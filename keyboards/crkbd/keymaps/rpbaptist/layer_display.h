// Copyright 2023 Your Name (@rpbaptist)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "quantum.h"

// One row for each layer. The OLED shows the label and the underglow shows the colour.
typedef struct {
#ifdef OLED_ENABLE
    char label[6];
#endif
#ifdef RGB_MATRIX_ENABLE
    hsv_t color;
#endif
} layer_display_t;

extern const layer_display_t layer_display[] PROGMEM;
