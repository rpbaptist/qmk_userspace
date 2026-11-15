// Copyright 2026 Richard Baptist (@rpbaptist)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "quantum.h"

// One row for each layer: the OLED label and the layer indicator color.
typedef struct {
#ifdef OLED_ENABLE
    char label[6];
#endif
#ifdef RGB_MATRIX_ENABLE
    hsv_t color;
#endif
} layer_display_t;

extern const layer_display_t layer_display[] PROGMEM;
