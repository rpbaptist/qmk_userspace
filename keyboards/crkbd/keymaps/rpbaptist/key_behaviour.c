// Copyright 2026 Richard Baptist (@rpbaptist)
// SPDX-License-Identifier: GPL-2.0-or-later

// Key behaviour: the QMK input hooks of the crkbd keymap. The firmware and
// the host tests (tests/crkbd_keys) link this file.

#include "rpbaptist.h"

// SFT_EXT, the gaming thumb key:
// - Press: SHIFT at once, no delay when held.
// - Tap, then press again within TAPPING_TERM: no SHIFT. GAMING_EXT is on for
//   the next key, or while this press is held.
// - Press while GAMING_EXT is on: cancel it and send SHIFT.
// - ONESHOT_TIMEOUT cancels an unused GAMING_EXT.
static bool process_sft_ext(uint16_t keycode, keyrecord_t *record) {
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

layer_state_t layer_state_set_user(layer_state_t state) {
    switch (get_highest_layer(default_layer_state)) {
        case _COLEMAKDH:
            state = update_tri_layer_state(state, _SYM, _NAV, _UTIL);
            break;
        case _GAMING:
        case _WASD:
            state = update_tri_layer_state(state, _GAMING_EXT, _NAV, _UTIL);
            break;
    }
    return state;
}

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case T_NAV:
            return TAPPING_TERM + 20;
        case SFT_ENT:
            return TAPPING_TERM - 20;
        default:
            return TAPPING_TERM;
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    static uint8_t saved_mods = 0;

#ifdef RGB_MATRIX_ENABLE
    process_record_user_rgb_matrix(keycode, record);
#endif

    if (!process_sft_ext(keycode, record)) {
        return false;
    }

    switch (keycode) {
        case BSP_DEL:
            if (record->event.pressed) {
                saved_mods = get_mods() & MOD_MASK_SHIFT;

                if (saved_mods == MOD_MASK_SHIFT) { // Two SHIFT keys: send SHIFT + DEL
                    register_code(KC_DEL);
                } else if (saved_mods) { // One SHIFT key: send DEL without SHIFT
                    del_mods(saved_mods);
                    register_code(KC_DEL);
                    add_mods(saved_mods);
                } else {
                    register_code(KC_BSPC);
                }
            } else {
                unregister_code(KC_DEL);
                unregister_code(KC_BSPC);
            }
            return false;
        case TYPING:
            if (record->event.pressed) {
#ifdef RGB_MATRIX_ENABLE
                rgb_matrix_set_typing_defaults();
#endif
                default_layer_set(1U << _COLEMAKDH);
            }
            return true;
        case GAMING:
            if (record->event.pressed) {
#ifdef RGB_MATRIX_ENABLE
                rgb_matrix_set_gaming_defaults();
#endif
                default_layer_set(1U << _GAMING);
            }
            return true;
        case WASD:
            if (record->event.pressed) {
#ifdef RGB_MATRIX_ENABLE
                rgb_matrix_set_gaming_defaults();
#endif
                default_layer_set(1U << _WASD);
            }
            return true;
        case LCK_NMP:
            if (record->event.pressed) {
                layer_lock_invert(_NUMPAD);
            }
            return false;
    }
    return true;
}
