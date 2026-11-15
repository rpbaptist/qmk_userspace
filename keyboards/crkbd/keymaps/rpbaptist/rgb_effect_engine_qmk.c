// Copyright 2026 Richard Baptist (@rpbaptist)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "rgb_effect_engine.h"

#include "rgb_matrix.h"

void rgb_effect_engine_set(uint8_t effect, uint8_t speed) {
    rgb_matrix_config.speed = speed;
    rgb_matrix_mode_noeeprom(effect);
}

uint8_t rgb_effect_engine_current(void) {
    return rgb_matrix_get_mode();
}
