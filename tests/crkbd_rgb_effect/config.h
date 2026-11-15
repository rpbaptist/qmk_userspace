#pragma once

#include "test_common.h"

// Use the real effect settings (effects, speeds, timeouts, theme). They are
// in the RGB_MATRIX_ENABLE block of the keymap config. The harness has no RGB
// matrix, so undefine the flag again after the include.
#define RGB_MATRIX_ENABLE
#include CRKBD_KEYMAP_CONFIG
#undef RGB_MATRIX_ENABLE

// rgb_matrix.h needs the LED count to compile. crkbd rev1 has 54 LEDs.
#define RGB_MATRIX_LED_COUNT 54
