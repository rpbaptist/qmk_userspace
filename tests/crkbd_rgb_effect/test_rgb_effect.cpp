// Copyright 2026 Richard Baptist (@rpbaptist)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "gtest/gtest.h"

extern "C" {
#include "rgb_matrix.h" // effect enum, with the keymap ENABLE_* defines
#include CRKBD_RGB_EFFECT_H
#include CRKBD_RGB_EFFECT_ENGINE_H

void set_time(uint32_t t);
void advance_time(uint32_t ms);
void last_matrix_activity_trigger(void);

// Fake RGB engine.
static uint8_t fake_effect;
static uint8_t fake_speed;

void rgb_effect_engine_set(uint8_t effect, uint8_t speed) {
    fake_effect = effect;
    fake_speed  = speed;
}

uint8_t rgb_effect_engine_current(void) {
    return fake_effect;
}
}

// Typing profiles of each theme in config.h.
static const rgb_effect_profile_t default_typing   = {RGB_MATRIX_SOLID_REACTIVE_SIMPLE, RGB_MATRIX_CYCLE_ALL, IDLE_TIMEOUT / 1000, false};
static const rgb_effect_profile_t laser_typing     = {RGB_MATRIX_SOLID_REACTIVE_SIMPLE, RGB_MATRIX_CYCLE_LEFT_RIGHT, IDLE_TIMEOUT / 1000, false};
static const rgb_effect_profile_t milkshake_typing = {RGB_MATRIX_TYPING_HEATMAP, RGB_MATRIX_CYCLE_LEFT_RIGHT, IDLE_TIMEOUT / 1000, false};

class RgbEffect : public testing::Test {
   protected:
    void SetUp() override {
        fake_effect = RGB_MATRIX_NONE;
        fake_speed  = 0;
        set_time(1000);
        rgb_effect_set_state(0);
        rgb_effect_reset(&rgb_effect_typing_profile);
        key_press();
    }

    // A key event: the input activity timer restarts and the effect wakes.
    void key_press() {
        last_matrix_activity_trigger();
        rgb_effect_wake();
    }

    void wait_and_tick(uint32_t ms) {
        advance_time(ms);
        rgb_effect_tick();
    }

    void go_idle() {
        wait_and_tick(IDLE_TIMEOUT + 1);
    }

    static void expect_invariant() {
        if (rgb_effect_idle_animation()) {
            EXPECT_TRUE(fake_effect == rgb_effect_active_effect() || fake_effect == rgb_effect_idle_effect())
                << "current " << +fake_effect << " active " << +rgb_effect_active_effect() << " idle " << +rgb_effect_idle_effect();
        }
    }
};

TEST_F(RgbEffect, TypingProfileUsesConfig) {
    EXPECT_EQ(rgb_effect_typing_profile.active_effect, RGB_MATRIX_TYPING_ACTIVE);
    EXPECT_EQ(rgb_effect_typing_profile.idle_effect, RGB_MATRIX_TYPING_PASSIVE);
    EXPECT_EQ(rgb_effect_typing_profile.idle_timeout_s * 1000UL, (uint32_t)IDLE_TIMEOUT);
}

TEST_F(RgbEffect, IdleTimeoutShowsIdleEffect) {
    wait_and_tick(IDLE_TIMEOUT);
    EXPECT_EQ(fake_effect, RGB_MATRIX_TYPING_ACTIVE);
    EXPECT_FALSE(rgb_effect_is_idle());

    wait_and_tick(1);
    EXPECT_EQ(fake_effect, RGB_MATRIX_TYPING_PASSIVE);
    EXPECT_EQ(fake_speed, RGB_MATRIX_ANIMATION_SPEED_SLOWER);
    EXPECT_TRUE(rgb_effect_is_idle());
}

TEST_F(RgbEffect, IdleTimeoutDoesNothingWithIdleAnimationOff) {
    rgb_effect_toggle_idle_animation();
    rgb_effect_cycle_active(); // shows an active effect
    uint8_t shown = fake_effect;
    go_idle();
    EXPECT_EQ(fake_effect, shown);
    EXPECT_FALSE(rgb_effect_is_idle());
}

TEST_F(RgbEffect, KeyPressWakesIdleEffect) {
    go_idle();
    ASSERT_TRUE(rgb_effect_is_idle());

    key_press();
    EXPECT_EQ(fake_effect, RGB_MATRIX_TYPING_ACTIVE);
    EXPECT_EQ(fake_speed, RGB_MATRIX_ANIMATION_SPEED_FAST);
    EXPECT_FALSE(rgb_effect_is_idle());

    // The timeout starts again from the key press.
    wait_and_tick(IDLE_TIMEOUT);
    EXPECT_EQ(fake_effect, RGB_MATRIX_TYPING_ACTIVE);
}

TEST_F(RgbEffect, CycleActiveWithIdleAnimationOn) {
    const uint8_t order[] = {RGB_MATRIX_TYPING_HEATMAP, RGB_MATRIX_SOLID_REACTIVE_MULTINEXUS, RGB_MATRIX_SOLID_REACTIVE_SIMPLE, RGB_MATRIX_TYPING_HEATMAP};
    for (uint8_t effect : order) {
        rgb_effect_cycle_active();
        EXPECT_EQ(rgb_effect_active_effect(), effect);
        EXPECT_EQ(fake_effect, effect);
        EXPECT_EQ(rgb_effect_idle_effect(), RGB_MATRIX_TYPING_PASSIVE);
    }
}

TEST_F(RgbEffect, CycleIdleSimpleWithIdleAnimationOn) {
    const uint8_t order[] = {RGB_MATRIX_SOLID_COLOR, RGB_MATRIX_BREATHING, RGB_MATRIX_CYCLE_ALL, RGB_MATRIX_SOLID_COLOR};
    for (uint8_t effect : order) {
        rgb_effect_cycle_idle_simple();
        EXPECT_EQ(rgb_effect_idle_effect(), effect);
        EXPECT_EQ(fake_effect, RGB_MATRIX_TYPING_ACTIVE) << "the active effect stays until the timeout";
    }
    go_idle();
    EXPECT_EQ(fake_effect, RGB_MATRIX_SOLID_COLOR);
}

TEST_F(RgbEffect, CycleIdleColorWithIdleAnimationOn) {
    const uint8_t order[] = {RGB_MATRIX_RAINBOW_PINWHEELS, RGB_MATRIX_CYCLE_LEFT_RIGHT, RGB_MATRIX_RAINBOW_PINWHEELS};
    for (uint8_t effect : order) {
        rgb_effect_cycle_idle_color();
        EXPECT_EQ(rgb_effect_idle_effect(), effect);
        EXPECT_EQ(fake_effect, RGB_MATRIX_TYPING_ACTIVE);
    }
}

TEST_F(RgbEffect, CycleIdleWhileIdleShowsNewIdleEffect) {
    go_idle();
    rgb_effect_cycle_idle_color();
    EXPECT_EQ(fake_effect, RGB_MATRIX_RAINBOW_PINWHEELS);
    EXPECT_TRUE(rgb_effect_is_idle());
}

TEST_F(RgbEffect, CyclesWithIdleAnimationOffShowAndStoreTheEffect) {
    rgb_effect_toggle_idle_animation();
    ASSERT_EQ(fake_effect, RGB_MATRIX_TYPING_PASSIVE);

    // The cycle starts from the current effect.
    // Each cycle starts from the current effect. The current effect is the
    // idle effect (CYCLE_ALL), which is not in the active cycle, so RGB_ATG
    // starts at SIMPLE.
    const uint8_t active_order[] = {RGB_MATRIX_SOLID_REACTIVE_SIMPLE, RGB_MATRIX_TYPING_HEATMAP, RGB_MATRIX_SOLID_REACTIVE_MULTINEXUS, RGB_MATRIX_SOLID_REACTIVE_SIMPLE};
    for (uint8_t effect : active_order) {
        rgb_effect_cycle_active();
        EXPECT_EQ(fake_effect, effect);
        EXPECT_EQ(rgb_effect_active_effect(), effect);
    }

    const uint8_t simple_order[] = {RGB_MATRIX_SOLID_COLOR, RGB_MATRIX_BREATHING, RGB_MATRIX_CYCLE_ALL, RGB_MATRIX_SOLID_COLOR};
    for (uint8_t effect : simple_order) {
        rgb_effect_cycle_idle_simple();
        EXPECT_EQ(fake_effect, effect);
        EXPECT_EQ(rgb_effect_idle_effect(), effect);
    }

    const uint8_t color_order[] = {RGB_MATRIX_RAINBOW_PINWHEELS, RGB_MATRIX_CYCLE_LEFT_RIGHT, RGB_MATRIX_RAINBOW_PINWHEELS};
    for (uint8_t effect : color_order) {
        rgb_effect_cycle_idle_color();
        EXPECT_EQ(fake_effect, effect);
        EXPECT_EQ(rgb_effect_idle_effect(), effect);
    }
    EXPECT_EQ(fake_speed, RGB_MATRIX_ANIMATION_SPEED_SLOW);

    // Idle animation on again uses the effects chosen while it was off.
    rgb_effect_toggle_idle_animation();
    EXPECT_EQ(fake_effect, RGB_MATRIX_SOLID_REACTIVE_SIMPLE);
    go_idle();
    EXPECT_EQ(fake_effect, RGB_MATRIX_RAINBOW_PINWHEELS);
}

TEST_F(RgbEffect, IdleAnimationOffShowsIdleEffect) {
    rgb_effect_toggle_idle_animation();
    EXPECT_FALSE(rgb_effect_idle_animation());
    EXPECT_EQ(fake_effect, RGB_MATRIX_TYPING_PASSIVE);
    EXPECT_FALSE(rgb_effect_is_idle());
}

TEST_F(RgbEffect, GamingProfileWithIdleAnimationOn) {
    rgb_effect_toggle_layer_indicator();
    ASSERT_FALSE(rgb_effect_layer_indicator());

    rgb_effect_set_profile(&rgb_effect_gaming_profile);
    EXPECT_EQ(rgb_effect_active_effect(), RGB_MATRIX_SOLID_REACTIVE_MULTINEXUS);
    EXPECT_EQ(rgb_effect_idle_effect(), RGB_MATRIX_RAINBOW_PINWHEELS);
    EXPECT_EQ(fake_effect, RGB_MATRIX_SOLID_REACTIVE_MULTINEXUS);
    EXPECT_TRUE(rgb_effect_layer_indicator());

    wait_and_tick(GAMING_IDLE_TIMEOUT);
    EXPECT_EQ(fake_effect, RGB_MATRIX_SOLID_REACTIVE_MULTINEXUS);
    wait_and_tick(1);
    EXPECT_EQ(fake_effect, RGB_MATRIX_RAINBOW_PINWHEELS);
}

TEST_F(RgbEffect, TypingProfileKeepsLayerIndicator) {
    rgb_effect_set_profile(&rgb_effect_gaming_profile);
    rgb_effect_toggle_layer_indicator();
    rgb_effect_set_profile(&rgb_effect_typing_profile);
    EXPECT_FALSE(rgb_effect_layer_indicator());
    EXPECT_EQ(fake_effect, RGB_MATRIX_TYPING_ACTIVE);
    wait_and_tick(GAMING_IDLE_TIMEOUT + 1);
    EXPECT_EQ(fake_effect, RGB_MATRIX_TYPING_ACTIVE) << "typing timeout, not gaming";
}

// A profile set while idle animation is off stores both effects, so they
// apply when idle animation is on again.
TEST_F(RgbEffect, GamingProfileWithIdleAnimationOff) {
    rgb_effect_toggle_idle_animation();
    rgb_effect_set_profile(&rgb_effect_gaming_profile);
    EXPECT_EQ(fake_effect, RGB_MATRIX_SOLID_REACTIVE_MULTINEXUS);
    EXPECT_EQ(rgb_effect_active_effect(), RGB_MATRIX_SOLID_REACTIVE_MULTINEXUS);
    EXPECT_EQ(rgb_effect_idle_effect(), RGB_MATRIX_RAINBOW_PINWHEELS);

    go_idle();
    EXPECT_EQ(fake_effect, RGB_MATRIX_SOLID_REACTIVE_MULTINEXUS);

    rgb_effect_toggle_idle_animation();
    EXPECT_EQ(fake_effect, RGB_MATRIX_SOLID_REACTIVE_MULTINEXUS);
    key_press();
    wait_and_tick(GAMING_IDLE_TIMEOUT + 1);
    EXPECT_EQ(fake_effect, RGB_MATRIX_RAINBOW_PINWHEELS);
}

TEST_F(RgbEffect, TypingProfileWithIdleAnimationOff) {
    rgb_effect_set_profile(&rgb_effect_gaming_profile);
    rgb_effect_toggle_idle_animation();
    rgb_effect_set_profile(&rgb_effect_typing_profile);
    EXPECT_EQ(fake_effect, RGB_MATRIX_TYPING_ACTIVE);
    EXPECT_EQ(rgb_effect_active_effect(), RGB_MATRIX_TYPING_ACTIVE);
    EXPECT_EQ(rgb_effect_idle_effect(), RGB_MATRIX_TYPING_PASSIVE);

    rgb_effect_toggle_idle_animation();
    key_press();
    wait_and_tick(IDLE_TIMEOUT + 1);
    EXPECT_EQ(fake_effect, RGB_MATRIX_TYPING_PASSIVE);
}

// Reset starts on the active effect of the typing profile, so the idle timeout
// works after reset. Before, reset always showed SOLID_REACTIVE_SIMPLE, which
// is not the active effect of the milkshake theme. The build has one theme, so
// the profiles of the other themes are copied from config.h.
TEST_F(RgbEffect, ResetStartsOnActiveEffectForEachTheme) {
    for (const rgb_effect_profile_t *typing : {&default_typing, &laser_typing, &milkshake_typing}) {
        SCOPED_TRACE(+typing->active_effect);
        rgb_effect_set_profile(&rgb_effect_gaming_profile);
        rgb_effect_toggle_idle_animation();
        rgb_effect_toggle_layer_indicator();
        go_idle();

        rgb_effect_reset(typing);
        EXPECT_EQ(fake_effect, typing->active_effect);
        EXPECT_EQ(rgb_effect_active_effect(), typing->active_effect);
        EXPECT_EQ(rgb_effect_idle_effect(), typing->idle_effect);
        EXPECT_TRUE(rgb_effect_idle_animation());
        EXPECT_TRUE(rgb_effect_layer_indicator());

        key_press();
        go_idle();
        EXPECT_EQ(fake_effect, typing->idle_effect);
    }
}

TEST_F(RgbEffect, ResetSetsSpeedOfActiveEffect) {
    rgb_effect_reset(&milkshake_typing);
    EXPECT_EQ(fake_speed, RGB_MATRIX_ANIMATION_SPEED_MEDIUM);
}

TEST_F(RgbEffect, InvariantHoldsAfterEachOperation) {
    expect_invariant();
    rgb_effect_cycle_active();
    expect_invariant();
    go_idle();
    expect_invariant();
    rgb_effect_cycle_idle_simple(); // while idle, no wake
    expect_invariant();
    rgb_effect_cycle_idle_color();
    expect_invariant();
    rgb_effect_cycle_active(); // while idle, no wake
    expect_invariant();
    go_idle();
    rgb_effect_set_profile(&rgb_effect_gaming_profile);
    expect_invariant();
    rgb_effect_toggle_layer_indicator();
    expect_invariant();
    rgb_effect_toggle_idle_animation();
    rgb_effect_cycle_idle_simple();
    rgb_effect_toggle_idle_animation();
    expect_invariant();
    go_idle();
    key_press();
    expect_invariant();
    rgb_effect_reset(&milkshake_typing);
    expect_invariant();
    rgb_effect_set_profile(&rgb_effect_typing_profile);
    expect_invariant();
}

TEST_F(RgbEffect, StateWordRoundTrips) {
    rgb_effect_set_profile(&rgb_effect_gaming_profile);
    uint32_t state = rgb_effect_state();
    rgb_effect_reset(&milkshake_typing);
    rgb_effect_set_state(state);
    EXPECT_EQ(rgb_effect_active_effect(), RGB_MATRIX_SOLID_REACTIVE_MULTINEXUS);
    EXPECT_EQ(rgb_effect_idle_effect(), RGB_MATRIX_RAINBOW_PINWHEELS);
}
