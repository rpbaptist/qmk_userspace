/*
This is the c configuration file for the keymap

Copyright 2012 Jun Wako <wakojun@gmail.com>
Copyright 2015 Jack Humbert

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

/* Select hand configuration */
#define EE_HANDS

#define NO_USB_STARTUP_CHECK
#define RGB_MATRIX_SLEEP

#define LAYER_STATE_8BIT // 8 layer limit

#define SPLIT_LAYER_STATE_ENABLE

#define DEBOUNCE 20

#ifdef RGB_MATRIX_ENABLE
#    define SPLIT_TRANSACTION_IDS_USER USER_CONFIG_SYNC

#    define IDLE_TIMEOUT 60000 * 4
#    define GAMING_IDLE_TIMEOUT 10000
#    define RGB_MATRIX_TYPING_ACTIVE RGB_MATRIX_SOLID_REACTIVE_SIMPLE
#    define RGB_MATRIX_TYPING_IDLE RGB_MATRIX_CYCLE_ALL

#    define RGB_MATRIX_HUE_STEP 6
#    define RGB_MATRIX_SAT_STEP 8
#    define RGB_MATRIX_VAL_STEP 10
#    define RGB_MATRIX_SPD_STEP 10

#    define ENABLE_RGB_MATRIX_BREATHING
#    define ENABLE_RGB_MATRIX_CYCLE_ALL
#    define ENABLE_RGB_MATRIX_CYCLE_LEFT_RIGHT
#    define ENABLE_RGB_MATRIX_RAINBOW_PINWHEELS

#    define RGB_MATRIX_FRAMEBUFFER_EFFECTS
#    define ENABLE_RGB_MATRIX_TYPING_HEATMAP

#    define RGB_MATRIX_KEYPRESSES // reacts to keypresses

#    define ENABLE_RGB_MATRIX_SOLID_REACTIVE_SIMPLE
#    define ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTINEXUS

#    define RGB_MATRIX_ANIMATION_SPEED_FAST UINT8_MAX / 2
#    define RGB_MATRIX_ANIMATION_SPEED_MEDIUM UINT8_MAX / 4
#    define RGB_MATRIX_ANIMATION_SPEED_SLOW UINT8_MAX / 8
#    define RGB_MATRIX_ANIMATION_SPEED_SLOWER UINT8_MAX / 24

#    if defined(THEME_GODSPEED)
#        define THEME_HSV 132, 255, 125
#    elif defined(THEME_GODSPEED_ARES)
#        define THEME_HSV HSV_ORANGE
#    elif defined(THEME_LASER)
#        undef RGB_MATRIX_TYPING_ACTIVE
#        undef RGB_MATRIX_TYPING_IDLE
#        define THEME_HSV HSV_PURPLE
#        define RGB_MATRIX_TYPING_ACTIVE RGB_MATRIX_SOLID_REACTIVE_SIMPLE
#        define RGB_MATRIX_TYPING_IDLE RGB_MATRIX_CYCLE_LEFT_RIGHT
#    elif defined(THEME_MILKSHAKE)
#        undef RGB_MATRIX_TYPING_ACTIVE
#        undef RGB_MATRIX_TYPING_IDLE
#        define THEME_HSV 30, 255, 218
#        define RGB_MATRIX_TYPING_ACTIVE RGB_MATRIX_TYPING_HEATMAP
#        define RGB_MATRIX_TYPING_IDLE RGB_MATRIX_CYCLE_LEFT_RIGHT
#    elif defined(THEME_AWAKEN)
#        define THEME_HSV 170, 255, 180
#    else
#        define THEME_HSV 123, 255, 125 // default to PULSE
#    endif
#endif

#define TAPPING_TERM 140
#define PERMISSIVE_HOLD
#define TAPPING_TERM_PER_KEY

#define TAPPING_TOGGLE 2
#define ONESHOT_TIMEOUT 1000 // Cancel a one-shot GAMING_EXT that is not used

#ifdef OLED_ENABLE
#    define SPLIT_OLED_ENABLE
#    define OLED_TIMEOUT 60000 * 10
#    define OLED_FONT_H "glcdfont_multi.c"
#endif

#undef PRODUCT
#define PRODUCT "Corne Keyboard"
