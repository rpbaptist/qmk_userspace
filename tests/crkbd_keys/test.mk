# Host-side tests of the crkbd key behaviour module. Run with
# .claude/skills/verify/scripts/run-test.sh crkbd_keys
# (the script sets QMK_USERSPACE).

CRKBD_KEYMAP := $(QMK_USERSPACE)/keyboards/crkbd/keymaps/rpbaptist

SRC += $(CRKBD_KEYMAP)/key_behaviour.c
LAYER_LOCK_ENABLE = yes
OPT_DEFS += -DQMK_KEYBOARD_H=\"quantum.h\"
OPT_DEFS += -DCRKBD_RPBAPTIST_H=\"$(CRKBD_KEYMAP)/rpbaptist.h\"
OPT_DEFS += -DCRKBD_KEYMAP_CONFIG=\"$(CRKBD_KEYMAP)/config.h\"
