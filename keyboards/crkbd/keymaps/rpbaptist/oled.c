// Copyright 2023 Your Name (@rpbaptist)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "rpbaptist.h"

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    if (is_keyboard_master()) {
        return OLED_ROTATION_270;
    } else {
        return OLED_ROTATION_180;
    }
}

static const char PROGMEM newline[] = "\n";

static void write_flag(bool on, const char *label) {
    oled_write_P(on ? label : newline, false);
}

void render_status(void) {
    oled_write_P(layer_display[get_highest_layer(default_layer_state)].label, false);
    oled_write_P(newline, false);
    uint8_t layer = get_highest_layer(layer_state);
    oled_write_P(layer < _GAMING_EXT ? PSTR("     ") : layer_display[layer].label, false);
    oled_write_P(newline, false);

    uint8_t modifiers = get_mods();

    write_flag(modifiers & MOD_MASK_SHIFT, PSTR("SHIFT"));
    write_flag(modifiers & MOD_MASK_CTRL, PSTR("CTRL "));
    write_flag(modifiers & MOD_MASK_ALT, PSTR("ALT  "));
    write_flag(modifiers & MOD_MASK_GUI, PSTR("SUPER"));

    oled_write_P(newline, false);

    led_t led_state = host_keyboard_led_state();
    oled_write_P(PSTR("Mode:"), false);
    write_flag(led_state.num_lock, PSTR(" NUM "));
    write_flag(led_state.caps_lock, PSTR(" CAPS"));

#ifdef RGB_MATRIX_ENABLE
    oled_write_P(PSTR("\n\n"), false);

    bool rgb_on = rgb_matrix_config.enable;
    write_flag(rgb_on, rgb_effect_label_upper());
    write_flag(rgb_on, rgb_effect_label_lower());
#endif
}

bool oled_task_user(void) {
    if (is_keyboard_master()) {
        render_status(); // Renders the current keyboard state (layer, lock, caps, scroll, etc)
    } else {
        oled_render_logo();
#ifdef RGB_MATRIX_ENABLE
        if (rgb_effect_is_idle()) {
            oled_scroll_left(); // Turns on scrolling
        } else {
            oled_scroll_off();
        }
#endif
    }
    return false;
}
