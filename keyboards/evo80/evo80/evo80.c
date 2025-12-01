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

#include "../../../lib/rdr_lib/rdr_common.h"

void matrix_io_delay(void) {
}

void matrix_output_select_delay(void) {
}

void matrix_output_unselect_delay(uint8_t line, bool key_pressed) {
}

led_config_t g_led_config = { {
	{ 0        , 1        , 2        , 3        , 4        , 5        , 6        , 7        , 8        , 9        , 10       , 11       , 12       , 14       , 15       , 16        },
	{ 17       , 18       , 19       , 20       , 21       , 22       , 23       , 24       , 25       , 26       , 27       , 28       , 29       , 30       , 31       , 32        },
	{ 34       , 35       , 36       , 37       , 38       , 39       , 40       , 41       , 42       , 43       , 44       , 45       , 46       , 47       , 49       , 50        },
	{ 52       , 54       , 55       , 56       , 57       , 58       , 59       , 60       , 61       , 62       , 63       , 64       , 65       , 66       , 33       , 51        },
	{ 67       , NO_LED   , 68       , 69       , 70       , 71       , 72       , 73       , 74       , 75       , 76       , 77       , 13       , 78       , 79       , NO_LED    },
	{ 80       , 81       , 82       , 53       , NO_LED   , 83       , NO_LED   , NO_LED   , 48       , 84       , 85       , 86       , 87       , 88       , 89       , 90        }
},{
    // "Fine-tuned" complex configuration
    { 8, 7},    { 21, 7},   { 34, 7},   { 47, 7},   { 60, 7},   { 73, 7},  { 86, 7},   { 99, 7},  { 112, 7},  { 125, 7},  { 138, 7},  { 151, 7},  { 164, 7},  { 177, 7},  { 190, 7},  { 203, 7},  { 216, 7}, 
    { 8, 17},   { 21, 17},  { 34, 17},  { 47, 17},  { 60, 17},  { 73, 17}, { 86, 17},  { 99, 17}, { 112, 17}, { 125, 17}, { 138, 17}, { 151, 17}, { 164, 17}, { 177, 17}, { 190, 17}, { 203, 17}, { 216, 17},
    { 8, 27},   { 21, 27},  { 34, 27},  { 47, 27},  { 60, 27},  { 73, 27}, { 86, 27},  { 99, 27}, { 112, 27}, { 125, 27}, { 138, 27}, { 151, 27}, { 164, 27}, { 177, 27}, { 177, 27}, { 190, 27}, { 203, 27}, { 216, 27},
    { 8, 37},   { 21, 37},  { 34, 37},  { 47, 37},  { 60, 37},  { 73, 37}, { 86, 37},  { 99, 37}, { 112, 37}, { 125, 37}, { 138, 37}, { 151, 37}, { 164, 37}, { 177, 37}, { 190, 37},
    { 8, 47},               { 34, 47},  { 47, 47},  { 60, 47},  { 73, 47}, { 86, 47},  { 99, 47}, { 112, 47}, { 125, 47}, { 138, 47}, { 151, 47},             { 177, 47},             { 203, 47},
    { 8, 57},   { 21, 57},  { 34, 57},                          { 73, 57},                                                { 138, 57}, { 151, 57}, { 164, 57}, { 177, 57}, { 190, 57}, { 203, 57}, { 216, 57},

    { 225, 65}, { 225, 65}, { 225, 65}, { 225, 65}, { 225, 65}
}, {
    1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
    1,      1,  1,  1,  1,  1,  1,  1,  1,  1,  1,      1,      1,
    1,  1,  1,          1,                  1,  1,  1,  1,  1,  1,  1,

    0,  0,  0,  0,  0
} };

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    User_Led_Show();
    return false;
}

void notify_usb_device_state_change_user(enum usb_device_state usb_device_state)  {
    if (Keyboard_Info.Key_Mode == QMK_USB_MODE) {
        if(usb_device_state == USB_DEVICE_STATE_CONFIGURED) {
            Usb_If_Ok = true; // usb enumeration complete
            Usb_If_Ok_Led = true;
            Usb_If_Ok_Delay = 0;
            Usb_Suspend_Sig = false;
        } else {
            Usb_If_Ok = false;
            Usb_If_Ok_Led = false;
            Usb_Suspend_Sig = true;
        }
    } else {
        Usb_If_Ok = false;
        Usb_If_Ok_Led = false;
        Usb_Suspend_Sig = false;
    }
}

void housekeeping_task_user(void) {
    if(User_State_Fulfill_Flag){
        User_Keyboard_Reset();
        User_State_Fulfill_Flag = 0x00;
    }
   
    if (WIN_MAC_CHANGE) {
        WIN_MAC_CHANGE = false;
        User_Mac_Win_Change();
    }

    es_chibios_user_idle_loop_hook();
}

void board_init(void) {
    User_Keyboard_Init();
}

void keyboard_post_init_user(void) {
    User_Keyboard_Post_Init();
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {   // process when user presses a key
    Usb_Change_Mode_Delay = 0;                                      // restore rgb timeout
    Usb_Change_Mode_Wakeup = false;

    return Key_Value_Dispose(keycode, record);
}