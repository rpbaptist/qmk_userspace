// Copyright 2026 Richard Baptist (@rpbaptist)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "keyboard_report_util.hpp"
#include "test_common.hpp"

extern "C" {
#include CRKBD_RPBAPTIST_H
}

using testing::_;
using testing::InSequence;

class BspDel : public TestFixture {
   protected:
    KeymapKey bsp_del = KeymapKey(_COLEMAKDH, 0, 0, BSP_DEL);
    KeymapKey lsft    = KeymapKey(_COLEMAKDH, 1, 0, KC_LSFT);
    KeymapKey rsft    = KeymapKey(_COLEMAKDH, 2, 0, KC_RSFT);

    void SetUp() override {
        set_keymap({bsp_del, lsft, rsft});
    }
};

TEST_F(BspDel, TapSendsBackspace) {
    TestDriver driver;
    InSequence s;

    EXPECT_REPORT(driver, (KC_BSPC));
    EXPECT_EMPTY_REPORT(driver);
    tap_key(bsp_del);
    VERIFY_AND_CLEAR(driver);
}

TEST_F(BspDel, OneShiftSendsDeleteWithoutShift) {
    TestDriver driver;
    InSequence s;

    EXPECT_REPORT(driver, (KC_LSFT));
    lsft.press();
    run_one_scan_loop();
    VERIFY_AND_CLEAR(driver);

    EXPECT_REPORT(driver, (KC_DEL));
    EXPECT_REPORT(driver, (KC_LSFT));
    tap_key(bsp_del);
    VERIFY_AND_CLEAR(driver);

    EXPECT_EMPTY_REPORT(driver);
    lsft.release();
    run_one_scan_loop();
    VERIFY_AND_CLEAR(driver);
}

TEST_F(BspDel, TwoShiftsSendShiftDelete) {
    TestDriver driver;
    InSequence s;

    EXPECT_REPORT(driver, (KC_LSFT));
    EXPECT_REPORT(driver, (KC_LSFT, KC_RSFT));
    lsft.press();
    run_one_scan_loop();
    rsft.press();
    run_one_scan_loop();
    VERIFY_AND_CLEAR(driver);

    EXPECT_REPORT(driver, (KC_LSFT, KC_RSFT, KC_DEL));
    EXPECT_REPORT(driver, (KC_LSFT, KC_RSFT));
    tap_key(bsp_del);
    VERIFY_AND_CLEAR(driver);

    EXPECT_REPORT(driver, (KC_RSFT));
    EXPECT_EMPTY_REPORT(driver);
    lsft.release();
    run_one_scan_loop();
    rsft.release();
    run_one_scan_loop();
    VERIFY_AND_CLEAR(driver);
}
