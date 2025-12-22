LTO_ENABLE = yes
RAW_ENABLE = yes
DYNAMIC_KEYMAP_ENABLE = yes

ifeq ($(strip $(LTO_ENABLE)), yes)
    OPT_DEFS += -DLTO_ENABLED
endif