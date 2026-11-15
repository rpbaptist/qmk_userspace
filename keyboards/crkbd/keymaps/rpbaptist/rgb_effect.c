// Copyright 2026 Richard Baptist (@rpbaptist)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "rgb_effect.h"

#include "keyboard.h"
#include "progmem.h"
#include "rgb_effect_engine.h"
#include "rgb_matrix.h"

// Fields do not cross a byte boundary, which keeps the AVR code small.
typedef union {
    uint32_t raw;
    struct {
        uint8_t active_effect : 4;
        uint8_t idle_effect : 4;
        bool    layer_indicator : 1;
        bool    idle_animation : 1;
        uint8_t idle_timeout_s;
    };
} user_config_t;

_Static_assert(sizeof(user_config_t) == sizeof(uint32_t), "The state word must fit in 32 bits");
_Static_assert(RGB_MATRIX_EFFECT_MAX <= 16, "An RGB effect must fit in 4 bits");
_Static_assert(IDLE_TIMEOUT % 1000 == 0 && IDLE_TIMEOUT / 1000 <= UINT8_MAX, "The idle timeout must be whole seconds, at most 255");
_Static_assert(GAMING_IDLE_TIMEOUT % 1000 == 0 && GAMING_IDLE_TIMEOUT / 1000 <= UINT8_MAX, "The idle timeout must be whole seconds, at most 255");

static user_config_t user_config;

const rgb_effect_profile_t rgb_effect_typing_profile = {RGB_MATRIX_TYPING_ACTIVE, RGB_MATRIX_TYPING_PASSIVE, IDLE_TIMEOUT / 1000, false};
const rgb_effect_profile_t rgb_effect_gaming_profile = {RGB_MATRIX_SOLID_REACTIVE_MULTINEXUS, RGB_MATRIX_RAINBOW_PINWHEELS, GAMING_IDLE_TIMEOUT / 1000, true};

static uint8_t speed_for(uint8_t effect) {
    switch (effect) {
        case RGB_MATRIX_SOLID_REACTIVE_SIMPLE:
        case RGB_MATRIX_SOLID_REACTIVE_MULTINEXUS:
            return RGB_MATRIX_ANIMATION_SPEED_FAST;
        case RGB_MATRIX_BREATHING:
        case RGB_MATRIX_CYCLE_LEFT_RIGHT:
        case RGB_MATRIX_RAINBOW_PINWHEELS:
            return RGB_MATRIX_ANIMATION_SPEED_SLOW;
        case RGB_MATRIX_CYCLE_ALL:
            return RGB_MATRIX_ANIMATION_SPEED_SLOWER;
        default:
            return RGB_MATRIX_ANIMATION_SPEED_MEDIUM;
    }
}

static bool is_active_effect(uint8_t effect) {
    return effect == RGB_MATRIX_SOLID_REACTIVE_SIMPLE || effect == RGB_MATRIX_SOLID_REACTIVE_MULTINEXUS || effect == RGB_MATRIX_TYPING_HEATMAP;
}

static void show(uint8_t effect) {
    rgb_effect_engine_set(effect, speed_for(effect));
}

// Stores the effect in its slot. Shows it when it is an active effect, when
// idle animation is off, or when it replaces the idle effect on display.
static void select_effect(uint8_t effect) {
    bool show_now;
    if (is_active_effect(effect)) {
        show_now                  = true;
        user_config.active_effect = effect;
    } else {
        show_now                = !user_config.idle_animation || rgb_effect_is_idle();
        user_config.idle_effect = effect;
    }
    if (show_now) {
        show(effect);
    }
}

// The cycle keys start from the current effect when idle animation is off.
static uint8_t cycle_origin(uint8_t stored) {
    return user_config.idle_animation ? stored : rgb_effect_engine_current();
}

void rgb_effect_set_profile(const rgb_effect_profile_t *profile) {
    user_config.active_effect = profile->active_effect;
    user_config.idle_effect   = profile->idle_effect;
    user_config.idle_timeout_s = profile->idle_timeout_s;
    if (profile->force_layer_indicator) {
        user_config.layer_indicator = true;
    }
    show(profile->active_effect);
}

void rgb_effect_reset(const rgb_effect_profile_t *typing) {
    user_config.layer_indicator = true;
    user_config.idle_animation  = true;
    rgb_effect_set_profile(typing);
}

void rgb_effect_cycle_active(void) {
    switch (cycle_origin(user_config.active_effect)) {
        case RGB_MATRIX_SOLID_REACTIVE_SIMPLE:
            select_effect(RGB_MATRIX_TYPING_HEATMAP);
            break;
        case RGB_MATRIX_TYPING_HEATMAP:
            select_effect(RGB_MATRIX_SOLID_REACTIVE_MULTINEXUS);
            break;
        default:
            select_effect(RGB_MATRIX_SOLID_REACTIVE_SIMPLE);
            break;
    }
}

void rgb_effect_cycle_idle_simple(void) {
    switch (cycle_origin(user_config.idle_effect)) {
        case RGB_MATRIX_SOLID_COLOR:
            select_effect(RGB_MATRIX_BREATHING);
            break;
        case RGB_MATRIX_BREATHING:
            select_effect(RGB_MATRIX_CYCLE_ALL);
            break;
        default:
            select_effect(RGB_MATRIX_SOLID_COLOR);
            break;
    }
}

void rgb_effect_cycle_idle_color(void) {
    switch (cycle_origin(user_config.idle_effect)) {
        case RGB_MATRIX_RAINBOW_PINWHEELS:
            select_effect(RGB_MATRIX_CYCLE_LEFT_RIGHT);
            break;
        default:
            select_effect(RGB_MATRIX_RAINBOW_PINWHEELS);
            break;
    }
}

void rgb_effect_toggle_idle_animation(void) {
    user_config.idle_animation ^= 1;
    show(user_config.idle_animation ? user_config.active_effect : user_config.idle_effect);
}

void rgb_effect_toggle_layer_indicator(void) {
    user_config.layer_indicator ^= 1;
}

void rgb_effect_wake(void) {
    if (rgb_effect_is_idle()) {
        show(user_config.active_effect);
    }
}

void rgb_effect_tick(void) {
    if (user_config.idle_animation && rgb_effect_engine_current() == user_config.active_effect && last_input_activity_elapsed() > user_config.idle_timeout_s * 1000UL) {
        show(user_config.idle_effect);
    }
}

bool rgb_effect_is_idle(void) {
    return user_config.idle_animation && rgb_effect_engine_current() == user_config.idle_effect;
}

bool rgb_effect_idle_animation(void) {
    return user_config.idle_animation;
}

bool rgb_effect_layer_indicator(void) {
    return user_config.layer_indicator;
}

uint8_t rgb_effect_active_effect(void) {
    return user_config.active_effect;
}

uint8_t rgb_effect_idle_effect(void) {
    return user_config.idle_effect;
}

static const char *label(uint8_t effect) {
    switch (effect) {
        case RGB_MATRIX_TYPING_HEATMAP:
            return PSTR("Heat ");
        case RGB_MATRIX_SOLID_REACTIVE_MULTINEXUS:
            return PSTR("Nexus");
        case RGB_MATRIX_SOLID_REACTIVE_SIMPLE:
            return PSTR("Ease ");
        case RGB_MATRIX_SOLID_COLOR:
            return PSTR("Solid");
        case RGB_MATRIX_BREATHING:
            return PSTR("Fade ");
        case RGB_MATRIX_CYCLE_ALL:
            return PSTR("Cycle");
        case RGB_MATRIX_RAINBOW_PINWHEELS:
            return PSTR("Wheel");
        case RGB_MATRIX_CYCLE_LEFT_RIGHT:
            return PSTR("Wave ");
        default:
            return PSTR("");
    }
}

const char *rgb_effect_label_upper(void) {
    return user_config.idle_animation ? label(user_config.active_effect) : PSTR("\n");
}

const char *rgb_effect_label_lower(void) {
    return label(user_config.idle_animation ? user_config.idle_effect : rgb_effect_engine_current());
}

uint32_t rgb_effect_state(void) {
    return user_config.raw;
}

void rgb_effect_set_state(uint32_t state) {
    user_config.raw = state;
}
