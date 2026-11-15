// Copyright 2026 Richard Baptist (@rpbaptist)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "keyboard_report_util.hpp"
#include "test_common.hpp"

extern "C" {
#include CRKBD_RPBAPTIST_H
}

using testing::_;

// Columns of row 0 in the test keymap.
// COL_WRONG_LOWER holds the lower layer of the tri-layer pair of the other base
// layers.
enum { COL_LEFT, COL_NAV, COL_TYPING, COL_GAMING, COL_WASD, COL_LCK_NMP, COL_WRONG_LOWER, COL_COUNT };

class Layers : public TestFixture {
   protected:
    void SetUp() override {
        const uint16_t base[COL_COUNT]   = {MO(_SYM), MO(_NAV), TYPING, GAMING, WASD, LCK_NMP, MO(_GAMING_EXT)};
        const uint16_t gaming[COL_COUNT] = {MO(_GAMING_EXT), MO(_NAV), TYPING, GAMING, WASD, LCK_NMP, MO(_SYM)};
        set_keymap({});
        for (uint8_t col = 0; col < COL_COUNT; col++) {
            add_key(KeymapKey(_COLEMAKDH, col, 0, base[col]));
            add_key(KeymapKey(_GAMING, col, 0, gaming[col]));
            add_key(KeymapKey(_WASD, col, 0, gaming[col]));
            for (uint8_t layer : {_GAMING_EXT, _NUMPAD, _SYM, _NAV, _UTIL}) {
                add_key(KeymapKey(layer, col, 0, KC_TRNS));
            }
        }
        default_layer_set(1UL << _COLEMAKDH);
    }

    KeymapKey key(uint8_t col) {
        return KeymapKey(_COLEMAKDH, col, 0, KC_NO);
    }

    void press(uint8_t col) {
        key(col).press();
        run_one_scan_loop();
    }

    void release(uint8_t col) {
        key(col).release();
        run_one_scan_loop();
    }

    void tap(uint8_t col) {
        press(col);
        release(col);
    }

    // Hold the left thumb (SYM or GAMING_EXT) and NAV, check UTIL, release.
    void expect_util_from_left_and_nav() {
        press(COL_LEFT);
        EXPECT_FALSE(layer_state_is(_UTIL));
        press(COL_NAV);
        EXPECT_TRUE(layer_state_is(_UTIL));
        release(COL_NAV);
        EXPECT_FALSE(layer_state_is(_UTIL));
        release(COL_LEFT);
    }

    // Hold the wrong lower layer and NAV, check no UTIL, release.
    void expect_no_util_from_wrong_lower_and_nav() {
        press(COL_WRONG_LOWER);
        press(COL_NAV);
        EXPECT_FALSE(layer_state_is(_UTIL));
        release(COL_NAV);
        release(COL_WRONG_LOWER);
    }
};

TEST_F(Layers, SymAndNavGiveUtilOnTyping) {
    TestDriver driver;
    EXPECT_NO_REPORT(driver);

    expect_util_from_left_and_nav();
    VERIFY_AND_CLEAR(driver);
}

TEST_F(Layers, GamingExtAndNavGiveNoUtilOnTyping) {
    TestDriver driver;
    EXPECT_NO_REPORT(driver);

    expect_no_util_from_wrong_lower_and_nav();
    VERIFY_AND_CLEAR(driver);
}

TEST_F(Layers, SymAndNavGiveNoUtilOnGaming) {
    TestDriver driver;
    EXPECT_NO_REPORT(driver);

    default_layer_set(1UL << _GAMING);
    expect_no_util_from_wrong_lower_and_nav();
    VERIFY_AND_CLEAR(driver);
}

TEST_F(Layers, SymAndNavGiveNoUtilOnWasd) {
    TestDriver driver;
    EXPECT_NO_REPORT(driver);

    default_layer_set(1UL << _WASD);
    expect_no_util_from_wrong_lower_and_nav();
    VERIFY_AND_CLEAR(driver);
}

TEST_F(Layers, GamingExtAndNavGiveUtilOnGaming) {
    TestDriver driver;
    EXPECT_NO_REPORT(driver);

    default_layer_set(1UL << _GAMING);
    expect_util_from_left_and_nav();
    VERIFY_AND_CLEAR(driver);
}

TEST_F(Layers, GamingExtAndNavGiveUtilOnWasd) {
    TestDriver driver;
    EXPECT_NO_REPORT(driver);

    default_layer_set(1UL << _WASD);
    expect_util_from_left_and_nav();
    VERIFY_AND_CLEAR(driver);
}

TEST_F(Layers, TypingSelectsColemakBase) {
    TestDriver driver;
    EXPECT_NO_REPORT(driver);

    default_layer_set(1UL << _GAMING);
    tap(COL_TYPING);
    EXPECT_EQ(get_highest_layer(default_layer_state), _COLEMAKDH);
    VERIFY_AND_CLEAR(driver);
}

TEST_F(Layers, GamingSelectsGamingBase) {
    TestDriver driver;
    EXPECT_NO_REPORT(driver);

    tap(COL_GAMING);
    EXPECT_EQ(get_highest_layer(default_layer_state), _GAMING);
    VERIFY_AND_CLEAR(driver);
}

TEST_F(Layers, WasdSelectsWasdBase) {
    TestDriver driver;
    EXPECT_NO_REPORT(driver);

    tap(COL_WASD);
    EXPECT_EQ(get_highest_layer(default_layer_state), _WASD);
    VERIFY_AND_CLEAR(driver);
}

TEST_F(Layers, BootSelectsColemakBase) {
    TestDriver driver;
    EXPECT_NO_REPORT(driver);

    default_layer_set(1UL << _WASD);
    keyboard_post_init_user();
    EXPECT_EQ(get_highest_layer(default_layer_state), _COLEMAKDH);
    VERIFY_AND_CLEAR(driver);
}

TEST_F(Layers, LckNmpTogglesNumpadLock) {
    TestDriver driver;
    EXPECT_NO_REPORT(driver);

    tap(COL_LCK_NMP);
    EXPECT_TRUE(layer_state_is(_NUMPAD));
    EXPECT_TRUE(is_layer_locked(_NUMPAD));

    tap(COL_LCK_NMP);
    EXPECT_FALSE(layer_state_is(_NUMPAD));
    EXPECT_FALSE(is_layer_locked(_NUMPAD));
    VERIFY_AND_CLEAR(driver);
}
