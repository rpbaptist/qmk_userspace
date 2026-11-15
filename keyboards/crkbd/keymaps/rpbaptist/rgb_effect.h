// Copyright 2026 Richard Baptist (@rpbaptist)
// SPDX-License-Identifier: GPL-2.0-or-later

// Idle animation for the crkbd RGB matrix. Owns the active effect, the idle
// effect, the idle timeout, the profiles and the layer indicator flag. The
// terms are in GLOSSARY.md.

#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    uint8_t active_effect;
    uint8_t idle_effect;
    uint8_t idle_timeout_s;
    bool    force_layer_indicator; // true turns the layer indicator on; false leaves it as it is
} rgb_effect_profile_t;

extern const rgb_effect_profile_t rgb_effect_typing_profile;
extern const rgb_effect_profile_t rgb_effect_gaming_profile;

// Turns idle animation and the layer indicator on, then applies the profile.
void rgb_effect_reset(const rgb_effect_profile_t *typing);
void rgb_effect_set_profile(const rgb_effect_profile_t *profile);

// RGB_ATG, RGB_PST and RGB_PCT.
void rgb_effect_cycle_active(void);
void rgb_effect_cycle_idle_simple(void);
void rgb_effect_cycle_idle_color(void);

void rgb_effect_toggle_idle_animation(void);
void rgb_effect_toggle_layer_indicator(void);

// Call on each key event.
void rgb_effect_wake(void);
// Call from the housekeeping task of the master half.
void rgb_effect_tick(void);

bool    rgb_effect_is_idle(void);
bool    rgb_effect_idle_animation(void);
bool    rgb_effect_layer_indicator(void);
uint8_t rgb_effect_active_effect(void);
uint8_t rgb_effect_idle_effect(void);

// The two OLED lines, as PROGMEM strings: the active and idle effect labels,
// or a blank line and the current effect label when idle animation is off.
const char *rgb_effect_label_upper(void);
const char *rgb_effect_label_lower(void);

// The raw state word, for split sync only.
uint32_t rgb_effect_state(void);
void     rgb_effect_set_state(uint32_t state);
