// Copyright 2023 Your Name (@rpbaptist)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#define NO_ACTION_ONESHOT
#define NO_USB_STARTUP_CHECK

#define LAYER_STATE_8BIT // 8 layer limit

#define T_UTIL TT(_UTIL)

#define TAPPING_TERM 140
#define PERMISSIVE_HOLD
#define TAPPING_TERM_PER_KEY

#define TAP_CODE_DELAY 10
#define TAPPING_TOGGLE 2

#define RGB_DISABLE_WHEN_USB_SUSPENDED // turn off effects when suspended

#define RGBLIGHT_LAYERS

#define THEME_HSV 123, 255, 125 // default to PULSE

#undef PRODUCT
#define PRODUCT "CU7"
