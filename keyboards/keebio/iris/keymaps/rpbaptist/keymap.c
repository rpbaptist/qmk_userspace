// Copyright 2026 Richard Baptist (@rpbaptist)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

enum layer_names {
    _COLEMAK,
    _QWERTY,
    _GAMING_EXT,
};

#define EXT MO(_GAMING_EXT)
#define T_QWERT TG(_QWERTY)

// Only the left half is connected. The right half stays empty.
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_COLEMAK] = LAYOUT(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐
      KC_ESC,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //├────────┼────────┼────────┼────────┼────────┼────────┤
      KC_GRV,    KC_Q,    KC_W,    KC_F,    KC_P,    KC_B,       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //├────────┼────────┼────────┼────────┼────────┼────────┤
      KC_TAB,    KC_A,    KC_R,    KC_S,    KC_T,    KC_G,       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐
      KC_LCTL,   KC_Z,    KC_X,    KC_C,    KC_D,    KC_V,     EXT,       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //└────────┴────────┴────────┴───┬────┴───┬────┴───┬────┴───┬────┘
                                     KC_LALT, KC_LSFT,  KC_SPC,       XXXXXXX, XXXXXXX, XXXXXXX
  //                               └────────┴────────┴────────┘
  ),

  // Toggled on top of _COLEMAK. Letters shift one column right, so WASD
  // sits under the same fingers as on the crkbd _WASD layer. Keys that
  // match _COLEMAK are transparent, except letters.
  [_QWERTY] = LAYOUT(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐
     _______, _______, _______, _______, _______, _______,       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //├────────┼────────┼────────┼────────┼────────┼────────┤
     _______,    KC_T,    KC_Q,    KC_W,    KC_E,    KC_R,       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //├────────┼────────┼────────┼────────┼────────┼────────┤
     _______,    KC_G,    KC_A,    KC_S,    KC_D,    KC_F,       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐
     _______,    KC_B,    KC_Z,    KC_X,    KC_C,    KC_V, _______,       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //└────────┴────────┴────────┴───┬────┴───┬────┴───┬────┴───┬────┘
                                     _______, _______, _______,       XXXXXXX, XXXXXXX, XXXXXXX
  //                               └────────┴────────┴────────┘
  ),

  [_GAMING_EXT] = LAYOUT(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐
      T_QWERT,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //├────────┼────────┼────────┼────────┼────────┼────────┤
      KC_LGUI,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //├────────┼────────┼────────┼────────┼────────┼────────┤
      KC_CAPS,   KC_F6,   KC_F7,   KC_F8,   KC_F9,  KC_F10,       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐
       KC_ENT,    KC_H,    KC_I,    KC_J,    KC_K,    KC_L,    KC_M,       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //└────────┴────────┴────────┴───┬────┴───┬────┴───┬────┴───┬────┘
                                     _______, _______, _______,       XXXXXXX, XXXXXXX, XXXXXXX
  //                               └────────┴────────┴────────┘
  ),
};

#if defined(RGBLIGHT_ENABLE) || defined(RGB_MATRIX_ENABLE)
static void set_layer_color(layer_state_t state) {
    uint8_t hue;
    switch (get_highest_layer(state)) {
        case _GAMING_EXT:
            hue = 191; // purple
            break;
        case _QWERTY:
            hue = 0; // red
            break;
        default:
            hue = 128; // cyan
            break;
    }
#    if defined(RGBLIGHT_ENABLE)
    rgblight_sethsv_noeeprom(hue, 255, rgblight_get_val());
#    else
    rgb_matrix_sethsv_noeeprom(hue, 255, rgb_matrix_get_val());
#    endif
}

void keyboard_post_init_user(void) {
#    if defined(RGBLIGHT_ENABLE)
    rgblight_enable_noeeprom();
    rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
#    else
    rgb_matrix_enable_noeeprom();
    rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
#    endif
    set_layer_color(layer_state);
}


layer_state_t layer_state_set_user(layer_state_t state) {
    set_layer_color(state);
    return state;
}
#endif
