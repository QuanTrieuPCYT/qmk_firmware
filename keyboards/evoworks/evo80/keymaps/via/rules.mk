VIA_ENABLE = yes
LTO_ENABLE = yes

ifeq ($(strip $(LTO_ENABLE)), yes)
    OPT_DEFS += -DLTO_ENABLED
endif