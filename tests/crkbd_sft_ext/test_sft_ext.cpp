// Copyright 2026 Richard Baptist (@rpbaptist)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "keyboard_report_util.hpp"
#include "test_common.hpp"

extern "C" {
#include CRKBD_RPBAPTIST_H

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    return process_sft_ext(keycode, record);
}
}

using testing::_;
using testing::InSequence;

class SftExt : public TestFixture {
   protected:
    KeymapKey sft_ext = KeymapKey(_COLEMAKDH, 0, 0, SFT_EXT);
    KeymapKey key_a   = KeymapKey(_COLEMAKDH, 1, 0, KC_A);
    KeymapKey ext_1   = KeymapKey(_GAMING_EXT, 1, 0, KC_1);
    KeymapKey ext_sft = KeymapKey(_GAMING_EXT, 0, 0, KC_TRNS); // as in keymap.c

    void SetUp() override {
        set_keymap({sft_ext, key_a, ext_1, ext_sft});
    }

    // Tap, then press again inside TAPPING_TERM. Leaves the second press held.
    void double_tap_and_hold(TestDriver &driver) {
        EXPECT_REPORT(driver, (KC_LSFT));
        EXPECT_EMPTY_REPORT(driver);
        tap_key(sft_ext);
        idle_for(TAPPING_TERM / 2);
        sft_ext.press();
        run_one_scan_loop();
        VERIFY_AND_CLEAR(driver);
    }
};

TEST_F(SftExt, HoldSendsShiftAtOnce) {
    TestDriver driver;
    InSequence s;

    EXPECT_REPORT(driver, (KC_LSFT));
    sft_ext.press();
    run_one_scan_loop();
    VERIFY_AND_CLEAR(driver);

    EXPECT_REPORT(driver, (KC_LSFT, KC_A));
    EXPECT_REPORT(driver, (KC_LSFT));
    tap_key(key_a);
    VERIFY_AND_CLEAR(driver);

    EXPECT_EMPTY_REPORT(driver);
    sft_ext.release();
    run_one_scan_loop();
    VERIFY_AND_CLEAR(driver);
}

TEST_F(SftExt, DoubleTapSkipsShiftAndArmsOneShotExt) {
    TestDriver driver;
    InSequence s;

    double_tap_and_hold(driver);

    EXPECT_NO_REPORT(driver);
    sft_ext.release();
    run_one_scan_loop();
    VERIFY_AND_CLEAR(driver);
    EXPECT_TRUE(layer_state_is(_GAMING_EXT));

    EXPECT_REPORT(driver, (KC_1));
    EXPECT_EMPTY_REPORT(driver);
    tap_key(ext_1);
    VERIFY_AND_CLEAR(driver);
    EXPECT_FALSE(layer_state_is(_GAMING_EXT));

    EXPECT_REPORT(driver, (KC_A));
    EXPECT_EMPTY_REPORT(driver);
    tap_key(key_a);
    VERIFY_AND_CLEAR(driver);
}

TEST_F(SftExt, TapThenHoldKeepsExtWhileHeld) {
    TestDriver driver;
    InSequence s;

    double_tap_and_hold(driver);

    EXPECT_REPORT(driver, (KC_1));
    EXPECT_EMPTY_REPORT(driver);
    tap_key(ext_1);
    EXPECT_REPORT(driver, (KC_1));
    EXPECT_EMPTY_REPORT(driver);
    tap_key(ext_1);
    VERIFY_AND_CLEAR(driver);

    EXPECT_NO_REPORT(driver);
    sft_ext.release();
    run_one_scan_loop();
    VERIFY_AND_CLEAR(driver);
    EXPECT_FALSE(layer_state_is(_GAMING_EXT));
}

TEST_F(SftExt, PressWhileExtActiveCancelsAndSendsShift) {
    TestDriver driver;
    InSequence s;

    double_tap_and_hold(driver);
    sft_ext.release();
    run_one_scan_loop();
    EXPECT_TRUE(layer_state_is(_GAMING_EXT));

    EXPECT_REPORT(driver, (KC_LSFT));
    sft_ext.press();
    run_one_scan_loop();
    VERIFY_AND_CLEAR(driver);
    EXPECT_FALSE(layer_state_is(_GAMING_EXT));

    EXPECT_EMPTY_REPORT(driver);
    sft_ext.release();
    run_one_scan_loop();
    VERIFY_AND_CLEAR(driver);
}

TEST_F(SftExt, UnusedExtTimesOut) {
    TestDriver driver;
    InSequence s;

    double_tap_and_hold(driver);
    sft_ext.release();
    run_one_scan_loop();
    EXPECT_TRUE(layer_state_is(_GAMING_EXT));

    idle_for(ONESHOT_TIMEOUT + 10);
    EXPECT_FALSE(layer_state_is(_GAMING_EXT));

    EXPECT_REPORT(driver, (KC_A));
    EXPECT_EMPTY_REPORT(driver);
    tap_key(key_a);
    VERIFY_AND_CLEAR(driver);
}

TEST_F(SftExt, SlowSecondTapSendsShiftAgain) {
    TestDriver driver;
    InSequence s;

    EXPECT_REPORT(driver, (KC_LSFT));
    EXPECT_EMPTY_REPORT(driver);
    tap_key(sft_ext);
    idle_for(TAPPING_TERM + 10);
    EXPECT_REPORT(driver, (KC_LSFT));
    EXPECT_EMPTY_REPORT(driver);
    tap_key(sft_ext);
    VERIFY_AND_CLEAR(driver);
    EXPECT_FALSE(layer_state_is(_GAMING_EXT));
}

TEST_F(SftExt, ShiftedKeyDoesNotStartDoubleTap) {
    TestDriver driver;
    InSequence s;

    EXPECT_REPORT(driver, (KC_LSFT));
    EXPECT_REPORT(driver, (KC_LSFT, KC_A));
    EXPECT_REPORT(driver, (KC_LSFT));
    EXPECT_EMPTY_REPORT(driver);
    sft_ext.press();
    run_one_scan_loop();
    tap_key(key_a);
    sft_ext.release();
    run_one_scan_loop();
    VERIFY_AND_CLEAR(driver);

    EXPECT_REPORT(driver, (KC_LSFT));
    EXPECT_EMPTY_REPORT(driver);
    tap_key(sft_ext);
    VERIFY_AND_CLEAR(driver);
    EXPECT_FALSE(layer_state_is(_GAMING_EXT));
}
