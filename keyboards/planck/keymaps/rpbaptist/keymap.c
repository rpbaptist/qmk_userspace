/* Copyright 2015-2021 Jack Humbert
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "rpbaptist.h"


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

[_COLEMAKDH] = LAYOUT_planck_grid(
     KC_ESC, KC_Q,    KC_W,    KC_F,    KC_P,    KC_B,    KC_J,    KC_L,    KC_U,    KC_Y,    KC_SCLN, BSP_DEL,
    TAB_NUM, KC_A,    KC_R,    KC_S,    KC_T,    KC_G,    KC_M,    KC_N,    KC_E,    KC_I,    KC_O,    KC_QUOT,
    LCTL_BR, KC_Z,    KC_X,    KC_C,    KC_D,    KC_V,    KC_K,    KC_H,    KC_COMM, KC_DOT,  KC_SLSH, RCTL_BR ,
    XXXXXXX, KC_LSFT, KC_LGUI, KC_LALT, T_SYM,   SFT_SPC, SFT_ENT, T_NAV,   KC_RGUI, KC_RALT, KC_RSFT, XXXXXXX
),

[_GAMING] = LAYOUT_planck_grid(
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
     KC_TAB, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    KC_LCTL, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_RCTL,
    XXXXXXX, GAME_XT, _______,  KC_LALT, KC_LSFT, KC_SPC,  KC_ENT, _______, _______, _______, _______, _______
),

[_WASD] = LAYOUT_planck_grid(
    _______, _______, _______, KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    _______,
    _______, _______, KC_S,    KC_D,    KC_F,    _______, KC_H,    KC_J,    KC_K,    KC_L,    _______, _______,
    _______, _______, _______, _______, KC_V,    KC_B,    KC_N,    KC_M,    _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
),

[_GAMING_EXT] = LAYOUT_planck_grid(
     KC_GRV, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_DEL,
    _______,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0, _______,
    KC_CAPS, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    XXXXXXX, _______, _______,  _______, KC_LSFT, KC_SPC,  KC_ENT, _______, _______, _______, _______, _______
),

[_NUMPAD] = LAYOUT_planck_grid(
    _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  KC_EUR,    KC_7,    KC_8,    KC_9, _______, _______,
      S_NUM, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_UNDS,    KC_4,    KC_5,    KC_6, KC_MINS, KC_PLUS,
    KC_LCTL, XXXXXXX, XXXXXXX, XXXXXXX, KC_PSCR, KC_NLCK,  KC_EQL,    KC_1,    KC_2,    KC_3, KC_SLSH, KC_ASTR,
    XXXXXXX, XXXXXXX, _______, _______, _______, _______,  KC_ENT,    KC_0,  KC_DOT, _______, _______, _______
),

[_SYM] = LAYOUT_planck_grid(
     KC_GRV,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0, _______,
    KC_TILD, KC_EXLM,   KC_AT, KC_HASH,  KC_DLR, KC_PERC, KC_CIRC, KC_AMPR, KC_ASTR, KC_UNDS, KC_MINS, KC_PLUS,
    LCT_PRN, XXXXXXX, XXXXXXX, XXXXXXX,  KC_EUR, XXXXXXX,  KC_EQL, KC_PIPE,   KC_LT,   KC_GT, KC_EXLM, RCT_PRN,
    _______, _______, _______, _______, S_SYM,   _______, _______, _______, _______, _______, _______, _______
),

[_NAV] = LAYOUT_planck_grid(
    KC_GRV,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5, KC_PGUP,  KC_HOME,   KC_UP,  KC_END,  KC_INS, KC_BSPC,
    KC_TILD,  KC_F6,   KC_F7,   KC_F8,   KC_F9,  KC_F10, KC_PGDN,  KC_LEFT, KC_DOWN, KC_RGHT, XXXXXXX, KC_RSFT,
    LCT_PRN,  KC_F11,  KC_F12,  KC_F13, KC_PSCR, KC_CAPS, WIN_CLS, TAB_BCK, TAB_CLS, TAB_FWD, KC_BSLS, RCT_PRN,
    _______, _______, _______, _______, _______, _______, _______,   S_NAV, _______, _______, _______, _______
),

[_UTIL] = LAYOUT_planck_grid(
      RESET, XXXXXXX, KC_MPRV, KC_VOLU, KC_MNXT,  TYPING, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    LCK_NMP, XXXXXXX, KC_MSTP, KC_VOLD, KC_MPLY,  GAMING, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    EEP_RST, KC_SLEP, XXXXXXX, KC_MUTE, XXXXXXX,    WASD, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______, _______, _______
)

};

layer_state_t layer_state_set_user(layer_state_t state) {
    switch (get_highest_layer(default_layer_state)) {
        case _COLEMAKDH:
            state = update_tri_layer_state(state, _SYM, _NAV, _UTIL);
            break;
        case _GAMING:
        case _WASD:
            state = update_tri_layer_state(state, _GAMING_EXT, _NAV, _UTIL);
            break;
    }
    return state;
}

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case SFT_ENT:
            return TAPPING_TERM - 20;
        default:
            return TAPPING_TERM;
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    static uint8_t saved_mods   = 0;
    uint16_t       temp_keycode = keycode;

#if defined(RGB_MATRIX_ENABLE) || defined(OLED_DRIVER_ENABLE)
    idle_timer = sync_timer_read32();
#endif

#ifdef RGB_MATRIX_ENABLE
    process_record_user_rgb_matrix(temp_keycode, record);
#endif

    // Filter out the actual keycode from MT and LT keys.
    if ((keycode >= QK_MOD_TAP && keycode <= QK_MOD_TAP_MAX) || (keycode >= QK_LAYER_TAP && keycode <= QK_LAYER_TAP_MAX)) {
        temp_keycode &= 0xFF;
    }

    switch (temp_keycode) {
        case BSP_DEL:
            if (record->event.pressed) {
                saved_mods = get_mods() & MOD_MASK_SHIFT;

                if (saved_mods == MOD_MASK_SHIFT) {  // Both shifts pressed
                    register_code(KC_DEL);
                } else if (saved_mods) {   // One shift pressed
                    del_mods(saved_mods);  // Remove any Shifts present
                    register_code(KC_DEL);
                    add_mods(saved_mods);  // Add shifts again
                } else {
                    register_code(KC_BSPC);
                }
            } else {
                unregister_code(KC_DEL);
                unregister_code(KC_BSPC);
            }
            return false;
        case TYPING:
            if (record->event.pressed) {
#ifdef RGB_MATRIX_ENABLE
                rgb_matrix_set_typing_defaults();
#endif
                default_layer_set(1U << _COLEMAKDH);
            }
            return true;
        case GAMING:
            if (record->event.pressed) {
#ifdef RGB_MATRIX_ENABLE
                rgb_matrix_set_gaming_defaults();
#endif
                default_layer_set(1U << _GAMING);
            }
            return true;
        case WASD:
            if (record->event.pressed) {
#ifdef RGB_MATRIX_ENABLE
                rgb_matrix_set_gaming_defaults();
#endif
                default_layer_set(1U << _WASD);
            }
            return true;
    }
    return true;
}

bool encoder_update_user(uint8_t index, bool clockwise) {
    if (clockwise) {
        tap_code(KC_VOLU);
    } else {
        tap_code(KC_VOLD);
    }
    return true;
}

