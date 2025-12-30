/* Copyright 2023 Finalkey
 * Copyright 2023 LiWenLiu <https://github.com/LiuLiuQMK>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

/* Mechanical locking support. Use KC_LCAP, KC_LNUM or KC_LSCR instead in keymap */
#define LOCKING_SUPPORT_ENABLE
/* Locking resynchronize hack */
#define LOCKING_RESYNC_ENABLE

/* Define less important options */

/*
 * Force NKRO
 *
 * Force NKRO (nKey Rollover) to be enabled by default, regardless of the saved
 * state in the bootmagic EEPROM settings. (Note that NKRO must be enabled in the
 * makefile for this to work.)
 *
 * If forced on, NKRO can be disabled via magic key (default = LShift+RShift+N)
 * until the next keyboard reset.
 *
 * NKRO may prevent your keystrokes from being detected in the BIOS, but it is
 * fully operational during normal computer usage.
 *
 * For a less heavy-handed approach, enable NKRO via magic key (LShift+RShift+N)
 * or via bootmagic (hold SPACE+N while plugging in the keyboard). Once set by
 * bootmagic, NKRO mode will always be enabled until it is toggled again during a
 * power-up.
 *
 */
#define FORCE_NKRO
/*
 * Feature disable options
 * These options are also useful to firmware size reduction.
 */

#define MATRIX_UNSELECT_DRIVE_HIGH
#define CORTEX_ENABLE_WFI_IDLE          FALSE
#define NO_MUSIC_MODE
#define LAYER_STATE_8BIT

/* Ensure we jump to bootloader if the RESET keycode was pressed */
#define EARLY_INIT_PERFORM_BOOTLOADER_JUMP TRUE

// delay-related configurations
#define DEBOUNCE 0
#define MATRIX_IO_DELAY 0
#define USB_POLLING_INTERVAL_MS 1

// nop fudge declaration
#ifndef NOP_FUDGE
#define NOP_FUDGE 0.4
#endif

// secure-related config
#define SECURE_UNLOCK_TIMEOUT 5000
#define SECURE_IDLE_TIMEOUT 0

// rgb matrix-related config
#define RGB_MATRIX_DEFAULT_MODE RGB_MATRIX_FLOWER_BLOOMING
#define RGB_MATRIX_DEFAULT_SPD 255
#define RGB_MATRIX_DEFAULT_HUE 213
#define RGB_MATRIX_DEFAULT_SAT 201
#define RGB_MATRIX_DEFAULT_VAL 180 // default brightness
#define RGB_MATRIX_LED_COUNT 96
#define RGB_MATRIX_KEYPRESSES
#define RGB_MATRIX_KEYRELEASES
#define RGB_MATRIX_FRAMEBUFFER_EFFECTS
#define RGB_MATRIX_TIMEOUT 60000
#define RGB_MATRIX_LED_FLUSH_LIMIT 16
#define RGB_MATRIX_MAXIMUM_BRIGHTNESS 255
#define RGB_MATRIX_SLEEP

// custom keycodes
#define LAYER_LOCK_ENABLE
#define SELECT_WORD_ENABLE
#define QK_LAYER_LOCK 0x7C7B
#define QK_LLCK QK_LAYER_LOCK
#define QK_SELECT_WORD 0x7C7C
#define QK_SELW QK_SELECT_WORD
#define MAC_SPT 0x7C7D