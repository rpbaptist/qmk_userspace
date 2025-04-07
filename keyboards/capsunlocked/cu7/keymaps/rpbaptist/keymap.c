/*
Copyright 2021 CapsUnlocked

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

#include QMK_KEYBOARD_H

enum layer_names {
    _GAME,
    _MEDIA,
    _UTIL,
};

bool     alt_tab_active       = false;
bool     alt_shift_tab_active = false;
uint16_t alt_tab_timer        = 0;

// ALT TAB Encoder Timer
void matrix_scan_user(void) {
    if (alt_tab_active) {
        if (timer_elapsed(alt_tab_timer) > 1000) {
            unregister_code(KC_LEFT_ALT);
            unregister_code(KC_LEFT_SHIFT);
            alt_tab_active       = false;
            alt_shift_tab_active = false;
        }
    }
};

#define RPLAY LCTL(LSFT(KC_S))
#define SGIF LCTL(LSFT(KC_J))
#define SHOT LCTL(LSFT(KC_I))

#define GAME DF(_GAME)
#define MEDIA DF(_MEDIA)
#define UTIL DF(_UTIL)

#define STEAM LSFT(KC_TAB)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_GAME]  = LAYOUT(RPLAY, GAME, MEDIA, UTIL, SHOT, SGIF, STEAM),
    [_MEDIA] = LAYOUT(KC_MUTE, _______, _______, _______, KC_MPRV, KC_MPLY, KC_MNXT),
    [_UTIL]  = LAYOUT(UG_TOGG, QK_BOOT, UG_SATU, UG_VALU, UG_NEXT, UG_SATD, UG_VALD),
};

bool encoder_update_user(uint8_t index, bool clockwise) {
    switch (get_highest_layer(default_layer_state)) {
        case _MEDIA:
            if (clockwise) {
                tap_code_delay(KC_VOLU, 10);
            } else {
                tap_code_delay(KC_VOLD, 10);
            }
        case _UTIL:
            if (clockwise) {
                rgblight_increase_hue();
            } else {
                rgblight_decrease_hue();
            }
        case _GAME:
            if (clockwise) {
                if (!alt_tab_active) {
                    alt_tab_active = true;
                    unregister_code(KC_LEFT_SHIFT);
                    register_code(KC_LEFT_ALT);
                }
                alt_tab_timer = timer_read();
                tap_code(KC_TAB);
            } else {
                if (!alt_shift_tab_active) {
                    alt_shift_tab_active = true;
                    register_code(KC_LEFT_ALT);
                    register_code(KC_LEFT_SHIFT);
                }
                alt_tab_timer = timer_read();
                tap_code(KC_TAB);
            }
    }
    return false;
}
