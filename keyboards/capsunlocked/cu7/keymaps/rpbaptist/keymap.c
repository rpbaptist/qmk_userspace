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

#define STEAM LSFT(KC_TAB)

void keyboard_post_init_user(void) {
  rgblight_setrgb(RGB_CYAN);
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_GAME]  = LAYOUT(
        KC_ENTER,
        KC_GRV, TO(_MEDIA), TT(_UTIL),
        RPLAY, SGIF, STEAM),
    [_MEDIA] = LAYOUT(
        KC_MUTE,
        KC_MSTP, TO(_GAME), _______,
        KC_MPRV, KC_MPLY, KC_MNXT),
    [_UTIL]  = LAYOUT(
        UG_TOGG,
        QK_BOOT, _______, _______,
        XXXXXXX, XXXXXXX, XXXXXXX),
};

bool encoder_update_user(uint8_t index, bool clockwise) {
    switch (get_highest_layer(layer_state)) {
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
            break;
        case _MEDIA:
            if (clockwise) {
                tap_code_delay(KC_VOLU, 10);
            } else {
                tap_code_delay(KC_VOLD, 10);
            }
            break;
        case _UTIL:
            if (clockwise) {
                rgblight_increase_hue();
            } else {
                rgblight_decrease_hue();
            }
            break;
    }
    return false;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    uint8_t current_layer = get_highest_layer(state);

    switch (current_layer) {
        case _GAME:
            rgblight_setrgb(RGB_CYAN);
            break;
        case _MEDIA:
            rgblight_setrgb(RGB_GREEN);
            break;
        case _UTIL:
            rgblight_setrgb(RGB_RED);
            break;
    }
    return state;
}
