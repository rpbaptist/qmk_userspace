// Copyright 2026 Richard Baptist (@rpbaptist)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

// Left half is used alone as a gaming pad. Mark the missing right half as
// disconnected after one failed transfer, and retry the link only once per
// minute, so serial timeouts do not stall the matrix scan.
#define SPLIT_MAX_CONNECTION_ERRORS 1
#define SPLIT_CONNECTION_CHECK_TIMEOUT 60000

// Same timings as the crkbd keymap.
#define TAPPING_TERM 140
#define PERMISSIVE_HOLD
#define TAPPING_TOGGLE 2
#define NO_ACTION_ONESHOT

#define DEBOUNCE 35

// Rev2 has no handedness pin. The left half is always the USB side.
#define MASTER_LEFT

// This build has 18 underglow LEDs, 9 per half (stock rev2 expects 12).
#undef RGBLIGHT_LED_COUNT
#define RGBLIGHT_LED_COUNT 18
#undef RGBLED_SPLIT
#define RGBLED_SPLIT { 9, 9 }
#define RGBLIGHT_LIMIT_VAL 225
