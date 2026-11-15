// Copyright 2026 Richard Baptist (@rpbaptist)
// SPDX-License-Identifier: GPL-2.0-or-later

// Link-time seam between rgb_effect and the RGB engine. rgb_effect_engine_qmk.c
// calls QMK rgb_matrix. The host tests link a fake.

#pragma once

#include <stdint.h>

void    rgb_effect_engine_set(uint8_t effect, uint8_t speed);
uint8_t rgb_effect_engine_current(void);
