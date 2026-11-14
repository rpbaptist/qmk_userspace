// Copyright 2026 Richard Baptist (@rpbaptist)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "rpbaptist.h"

// SFT_EXT, the gaming thumb key:
// - Press: SHIFT at once, no delay when held.
// - Tap, then press again within TAPPING_TERM: no SHIFT. GAMING_EXT is on for
//   the next key, or while this press is held.
// - Press while GAMING_EXT is on: cancel it and send SHIFT.
// - ONESHOT_TIMEOUT cancels an unused GAMING_EXT.
bool process_sft_ext(uint16_t keycode, keyrecord_t *record) {
    static uint16_t press_time   = 0;
    static uint16_t release_time = 0;
    static bool     interrupted  = true;
    static bool     tapped_once  = false;

    if (keycode != SFT_EXT) {
        if (record->event.pressed) {
            interrupted = true;
            tapped_once = false;
        }
        return true;
    }

    if (record->event.pressed) {
        if (is_oneshot_layer_active()) {
            clear_oneshot_layer_state(ONESHOT_START);
            register_code(KC_LSFT);
            interrupted = true; // A cancel press does not start a double tap
        } else if (tapped_once && TIMER_DIFF_16(record->event.time, release_time) < TAPPING_TERM) {
            set_oneshot_layer(_GAMING_EXT, ONESHOT_START);
            interrupted = false;
        } else {
            register_code(KC_LSFT);
            press_time  = record->event.time;
            interrupted = false;
        }
        tapped_once = false;
    } else if (get_oneshot_layer_state() & ONESHOT_PRESSED) {
        // Release of the press that armed GAMING_EXT. QMK used up the one-shot
        // because that press returned false (quantum/action.c), so arm it
        // again, unless a key was used while it was held.
        if (!interrupted) {
            set_oneshot_layer(_GAMING_EXT, ONESHOT_START);
        }
        clear_oneshot_layer_state(ONESHOT_PRESSED);
    } else {
        unregister_code(KC_LSFT);
        tapped_once  = !interrupted && TIMER_DIFF_16(record->event.time, press_time) < TAPPING_TERM;
        release_time = record->event.time;
    }
    return false;
}
