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

// One row for each base layer. The first row is the boot default. Each row is
// at the index of its layer, so the tri-layer rule reads its row with no
// search. Thus the base layers must be the first entries of layer_names.
typedef struct {
    uint8_t  layer;
    uint16_t keycode;   // Selects the layer
    uint8_t  tri_lower; // With _NAV, gives _UTIL
#ifdef RGB_MATRIX_ENABLE
    const rgb_effect_profile_t *profile;
#endif
} base_layer_t;

#ifdef RGB_MATRIX_ENABLE
#    define BASE_PROFILE(p) .profile = &(p)
#else
#    define BASE_PROFILE(p)
#endif

static const base_layer_t base_layers[] = {
    [_COLEMAKDH] = {.layer = _COLEMAKDH, .keycode = TYPING, .tri_lower = _SYM, BASE_PROFILE(rgb_effect_typing_profile)},
    [_GAMING]    = {.layer = _GAMING, .keycode = GAMING, .tri_lower = _GAMING_EXT, BASE_PROFILE(rgb_effect_gaming_profile)},
    [_WASD]      = {.layer = _WASD, .keycode = WASD, .tri_lower = _GAMING_EXT, BASE_PROFILE(rgb_effect_gaming_profile)},
};

static const base_layer_t *base_layer_by_keycode(uint16_t keycode) {
    for (const base_layer_t *base = base_layers; base < base_layers + ARRAY_SIZE(base_layers); base++) {
        if (base->keycode == keycode) {
            return base;
        }
    }
    return NULL;
}

static void select_base_layer(const base_layer_t *base) {
#ifdef RGB_MATRIX_ENABLE
    rgb_effect_set_profile(base->profile);
#endif
    set_single_default_layer(base->layer);
}

void keyboard_post_init_user(void) {
    set_single_default_layer(base_layers[0].layer);
#ifdef RGB_MATRIX_ENABLE
    keyboard_post_init_rgb();
#endif
}

layer_state_t layer_state_set_user(layer_state_t state) {
    uint8_t layer = get_highest_layer(default_layer_state);
    return layer < ARRAY_SIZE(base_layers) ? update_tri_layer_state(state, base_layers[layer].tri_lower, _NAV, _UTIL) : state;
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
#ifdef RGB_MATRIX_ENABLE
    process_record_user_rgb_matrix(keycode, record);
#endif

    if (!process_sft_ext(keycode, record)) {
        return false;
    }

    const base_layer_t *base = base_layer_by_keycode(keycode);
    if (base) {
        if (record->event.pressed) {
            select_base_layer(base);
        }
        return true;
    }

    switch (keycode) {
        case BSP_DEL:
            if (record->event.pressed) {
                uint8_t saved_mods = get_mods() & MOD_MASK_SHIFT;

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
        case LCK_NMP:
            if (record->event.pressed) {
                layer_lock_invert(_NUMPAD);
            }
            return false;
    }
    return true;
}
