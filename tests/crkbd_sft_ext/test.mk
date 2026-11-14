# Host-side test of the crkbd SFT_EXT key. Run with
# .claude/skills/verify/scripts/run-test.sh crkbd_sft_ext
# (the script sets QMK_USERSPACE).

CRKBD_KEYMAP := $(QMK_USERSPACE)/keyboards/crkbd/keymaps/rpbaptist

SRC += $(CRKBD_KEYMAP)/sft_ext.c
OPT_DEFS += -DQMK_KEYBOARD_H=\"quantum.h\"
OPT_DEFS += -DCRKBD_RPBAPTIST_H=\"$(CRKBD_KEYMAP)/rpbaptist.h\"
OPT_DEFS += -DCRKBD_KEYMAP_CONFIG=\"$(CRKBD_KEYMAP)/config.h\"
