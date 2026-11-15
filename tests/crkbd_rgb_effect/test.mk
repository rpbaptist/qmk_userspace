# Host-side test of the crkbd RGB idle animation (rgb_effect.c) with a fake
# RGB engine. Run with
# .claude/skills/verify/scripts/run-test.sh crkbd_rgb_effect
# (the script sets QMK_USERSPACE).

CRKBD_KEYMAP := $(QMK_USERSPACE)/keyboards/crkbd/keymaps/rpbaptist

SRC += $(CRKBD_KEYMAP)/rgb_effect.c
OPT_DEFS += -DQMK_KEYBOARD_H=\"quantum.h\"
OPT_DEFS += -DCRKBD_RGB_EFFECT_H=\"$(CRKBD_KEYMAP)/rgb_effect.h\"
OPT_DEFS += -DCRKBD_RGB_EFFECT_ENGINE_H=\"$(CRKBD_KEYMAP)/rgb_effect_engine.h\"
OPT_DEFS += -DCRKBD_KEYMAP_CONFIG=\"$(CRKBD_KEYMAP)/config.h\"

# Include dirs for rgb_matrix.h (effect enum only). No RGB matrix code links.
COMMON_VPATH += $(QUANTUM_DIR)/rgb_matrix $(QUANTUM_DIR)/rgb_matrix/animations
