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
    KeymapKey lctl    = KeymapKey(_COLEMAKDH, 3, 0, KC_LCTL);

    void SetUp() override {
        set_keymap({bsp_del, lsft, rsft, lctl});
    }

    void press(KeymapKey &key) {
        key.press();
        run_one_scan_loop();
    }

    void release(KeymapKey &key) {
        key.release();
        run_one_scan_loop();
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

TEST_F(BspDel, TwoShiftsSendShiftBackspace) {
    TestDriver driver;
    InSequence s;

    EXPECT_REPORT(driver, (KC_LSFT));
    EXPECT_REPORT(driver, (KC_LSFT, KC_RSFT));
    lsft.press();
    run_one_scan_loop();
    rsft.press();
    run_one_scan_loop();
    VERIFY_AND_CLEAR(driver);

    EXPECT_REPORT(driver, (KC_LSFT, KC_RSFT, KC_BSPC));
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

TEST_F(BspDel, CtrlSendsCtrlBackspace) {
    TestDriver driver;
    InSequence s;

    EXPECT_REPORT(driver, (KC_LCTL));
    press(lctl);
    VERIFY_AND_CLEAR(driver);

    EXPECT_REPORT(driver, (KC_LCTL, KC_BSPC));
    EXPECT_REPORT(driver, (KC_LCTL));
    tap_key(bsp_del);
    VERIFY_AND_CLEAR(driver);

    EXPECT_EMPTY_REPORT(driver);
    release(lctl);
    VERIFY_AND_CLEAR(driver);
}

TEST_F(BspDel, CtrlAndOneShiftSendCtrlDelete) {
    TestDriver driver;
    InSequence s;

    EXPECT_REPORT(driver, (KC_LCTL));
    EXPECT_REPORT(driver, (KC_LCTL, KC_RSFT));
    press(lctl);
    press(rsft);
    VERIFY_AND_CLEAR(driver);

    EXPECT_REPORT(driver, (KC_LCTL, KC_DEL));
    EXPECT_REPORT(driver, (KC_LCTL, KC_RSFT));
    tap_key(bsp_del);
    VERIFY_AND_CLEAR(driver);

    EXPECT_REPORT(driver, (KC_LCTL));
    EXPECT_EMPTY_REPORT(driver);
    release(rsft);
    release(lctl);
    VERIFY_AND_CLEAR(driver);
}

TEST_F(BspDel, CtrlAndTwoShiftsSendCtrlShiftBackspace) {
    TestDriver driver;
    InSequence s;

    EXPECT_REPORT(driver, (KC_LCTL));
    EXPECT_REPORT(driver, (KC_LCTL, KC_LSFT));
    EXPECT_REPORT(driver, (KC_LCTL, KC_LSFT, KC_RSFT));
    press(lctl);
    press(lsft);
    press(rsft);
    VERIFY_AND_CLEAR(driver);

    EXPECT_REPORT(driver, (KC_LCTL, KC_LSFT, KC_RSFT, KC_BSPC));
    EXPECT_REPORT(driver, (KC_LCTL, KC_LSFT, KC_RSFT));
    tap_key(bsp_del);
    VERIFY_AND_CLEAR(driver);

    EXPECT_REPORT(driver, (KC_LCTL, KC_RSFT));
    EXPECT_REPORT(driver, (KC_LCTL));
    EXPECT_EMPTY_REPORT(driver);
    release(lsft);
    release(rsft);
    release(lctl);
    VERIFY_AND_CLEAR(driver);
}
