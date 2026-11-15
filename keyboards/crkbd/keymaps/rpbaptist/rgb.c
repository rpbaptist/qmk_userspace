// Copyright 2023 Your Name (@rpbaptist)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "rpbaptist.h"

uint32_t transport_user_config = 0;

void user_config_sync(uint8_t initiator2target_buffer_size, const void* initiator2target_buffer, uint8_t target2initiator_buffer_size, void* target2initiator_buffer) {
    if (initiator2target_buffer_size == sizeof(transport_user_config)) {
        memcpy(&transport_user_config, initiator2target_buffer, initiator2target_buffer_size);
    }
}

void rgb_matrix_layer_helper(uint8_t hue, uint8_t sat, uint8_t val, uint8_t led_min, uint8_t led_max) {
    hsv_t hsv = {hue, sat, val};
    if (hsv.v > rgb_matrix_get_val()) {
        hsv.v = rgb_matrix_get_val();
    }

    rgb_t rgb = hsv_to_rgb(hsv);
    for (uint8_t i = led_min; i < led_max; i++) {
        if (HAS_FLAGS(g_led_config.flags[i], LED_FLAG_UNDERGLOW)) {
            rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
        }
    }
}

static const hsv_t PROGMEM layer_colors[] = {
    [_COLEMAKDH]  = {THEME_HSV},
    [_GAMING]     = {HSV_RED},
    [_WASD]       = {HSV_RED},
    [_GAMING_EXT] = {HSV_PURPLE},
    [_NUMPAD]     = {HSV_CORAL},
    [_SYM]        = {HSV_YELLOW},
    [_NAV]        = {HSV_SPRINGGREEN},
    [_UTIL]       = {HSV_PINK},
};
_Static_assert(ARRAY_SIZE(layer_colors) == _UTIL + 1, "layer_colors needs one row for each layer");

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    if (rgb_effect_layer_indicator() && !rgb_effect_is_idle()) {
        hsv_t color;
        memcpy_P(&color, &layer_colors[get_highest_layer(layer_state | default_layer_state)], sizeof(color));
        rgb_matrix_layer_helper(color.h, color.s, color.v, led_min, led_max);
    }
    return false;
}

static void rgb_matrix_set_defaults(void) {
    rgb_matrix_enable_noeeprom();
    rgb_matrix_sethsv_noeeprom(THEME_HSV);
    rgb_effect_reset(&rgb_effect_typing_profile);
}

void user_transport_sync(void) {
    // Keep track of the last state, so that we can tell if we need to propagate to slave
    static uint32_t last_config = 0, last_sync = 0;

    // Check if the state values are different
    // or if sync timer elapsed
    if (transport_user_config != last_config || (timer_elapsed32(last_sync) > 250)) {
        last_config = transport_user_config;

        if (transaction_rpc_send(USER_CONFIG_SYNC, sizeof(transport_user_config), &transport_user_config)) {
            last_sync = timer_read32();
        }
    }
}

void user_transport_update(void) {
    if (is_keyboard_master()) {
        transport_user_config = rgb_effect_state();
        user_transport_sync();
    } else {
        rgb_effect_set_state(transport_user_config);
    }
}

void housekeeping_task_user(void) {
    // Only master changes the RGB effect.
    // RGB_MATRIX_SPLIT copies the effect to the slave half.
    if (is_keyboard_master()) {
        rgb_effect_tick();
    }

    // Update config to slave
    user_transport_update();
}

void keyboard_post_init_user_rgb_matrix(void) {
    rgb_matrix_set_defaults();

    // Register user config sync
    transaction_register_rpc(USER_CONFIG_SYNC, user_config_sync);
}

bool process_record_user_rgb_matrix(uint16_t keycode, keyrecord_t* record) {
    rgb_effect_wake();

    switch (keycode) {
        case RGB_RST:
            if (record->event.pressed) {
                rgb_matrix_set_defaults();
            }
            break;
        case RGB_UND: // Toggle the layer indicator
            if (record->event.pressed) {
                rgb_effect_toggle_layer_indicator();
            }
            break;
        case RGB_IDL: // Toggle idle animation
            if (record->event.pressed) {
                rgb_effect_toggle_idle_animation();
            }
            break;
        case RGB_ATG:
            if (record->event.pressed) {
                rgb_effect_cycle_active();
            }
            break;
        case RGB_PST:
            if (record->event.pressed) {
                rgb_effect_cycle_idle_simple();
            }
            break;
        case RGB_PCT:
            if (record->event.pressed) {
                rgb_effect_cycle_idle_color();
            }
            break;
    }
    return true;
}
