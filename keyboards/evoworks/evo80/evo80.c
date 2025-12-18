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

#define BATT_LED_START_IDX 35
#define BATT_LED_END_IDX   44
#define BATT_LED_TOTAL     10

#ifdef VIA_ENABLE // via exclusive feature
#include "via.h"
enum via_custom_config_value {
    id_debounce_time   = 1,
    id_nkro_toggle     = 2,
    id_mac_mode        = 3,
    id_win_lock        = 4
};

void custom_config_set_value(uint8_t *data) {
    uint8_t *value_id   = &(data[0]);
    uint8_t *value_data = &(data[1]);

    switch ( *value_id ) {
        case id_debounce_time:
        {
            Keyboard_Info.Debounce_Delay = *value_data;
            Debounce_Function_Count = (_Bool)(1 - (Keyboard_Info.Debounce_Delay == 2));
            break;
        }
        case id_nkro_toggle:
        {
            if (keymap_config.raw < 0x80) {
                clear_keyboard();
                keymap_config.raw = keymap_config.raw | 0x80;
                Keyboard_Info.Nkro = 1;
            } else {
                if (0x7f < keymap_config.raw) {
                    clear_keyboard();
                    keymap_config.raw = keymap_config.raw & 0x7f;
                    Keyboard_Info.Nkro = 0;
                }
            }
            break;
        }
        case id_mac_mode:
        {
            unregister_code(0x65);
            unregister_code(0xe2);
            unregister_code(0xe3);
            unregister_code(0xe6);
            unregister_code(0xe7);
            if (Keyboard_Info.Mac_Win_Mode) {
                Keyboard_Info.Mac_Win_Mode = 0;
                if (biton(layer_state) != 0) layer_move(0);
            } else {
                Keyboard_Info.Mac_Win_Mode = 1;
                if (biton(layer_state) != 1) layer_move(1);
            }
            break;
        }
        case id_win_lock:
        {
            if (Keyboard_Info.Win_Lock) {
                Keyboard_Info.Win_Lock = 0;
            } else if (Keyboard_Info.Mac_Win_Mode == 0) {
                Keyboard_Info.Win_Lock = 1;
                unregister_code(0xe3);
                unregister_code(0xe7);
                unregister_code(0x65);
            }
            break;
        }
    }
}

void custom_config_get_value(uint8_t *data) {
    uint8_t *value_id   = &(data[0]);
    uint8_t *value_data = &(data[1]);

    switch ( *value_id ) {
        case id_debounce_time:
        {
            *value_data = Keyboard_Info.Debounce_Delay;
            break;
        }
        case id_nkro_toggle:
        {
            *value_data = 1 - (keymap_config.raw < 0x80);
            break;
        }
        case id_mac_mode:
        {
            *value_data = Keyboard_Info.Mac_Win_Mode;
            break;
        }
        case id_win_lock:
        {
            *value_data = Keyboard_Info.Win_Lock;
            break;
        }
    }
}

void custom_config_save(void) {
    Save_Flash_Set();
}

void via_custom_value_command_kb(uint8_t *data, uint8_t length) {
    uint8_t *command_id        = &(data[0]);
    uint8_t *channel_id        = &(data[1]);
    uint8_t *value_id_and_data = &(data[2]);

    if ( *channel_id == id_custom_channel ) {
        switch ( *command_id ) {
            case id_custom_set_value:
            {
                custom_config_set_value(value_id_and_data);
                break;
            }
            case id_custom_get_value:
            {
                custom_config_get_value(value_id_and_data);
                break;
            }
            case id_custom_save:
            {
                custom_config_save();
                break;
            }
            default:
            {
                *command_id = id_unhandled;
                break;
            }
        }
        return;
    }
    *command_id = id_unhandled;
}
#endif

static const uint8_t wave_tab_led[128] = {
    0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60, 64, 68, 72, 76, 80, 84, 88, 92, 96, 100, 104, 108, 112, 116, 120, 124, 128, 132, 136, 140, 144, 148, 152, 156, 160, 164, 168, 172, 176, 180, 184, 188, 192, 196, 200, 204, 208, 212, 216, 220, 224, 228, 232, 236, 240, 244, 248, 255, 255, 248, 244, 240, 236, 232, 228, 224, 220, 216, 212, 208, 204, 200, 196, 192, 188, 184, 180, 176, 172, 168, 164, 160, 156, 152, 148, 144, 140, 136, 132, 128, 124, 120, 116, 112, 108, 104, 100, 96, 92, 88, 84, 80, 76, 72, 68, 64, 60, 56, 52, 48, 44, 40, 36, 32, 28, 24, 20, 16, 12, 8, 4, 0
};

static inline bool kb_get_caps_lock_state(void) {
    if (Keyboard_Info.Key_Mode == QMK_USB_MODE) {
        return host_keyboard_led_state().caps_lock && Usb_If_Ok_Led;
    }
    return (Keyboard_Status.System_Led_Status & 0x02);
}

static inline void set_battery_color(uint8_t percent, uint8_t *r, uint8_t *g, uint8_t *b) {
    if (percent <= 10) {
        *r = 180; *g = 0;   *b = 0;
    } else if (percent < 50) {
        *r = 180; *g = 180; *b = 0;
    } else if (percent < 80) {
        *r = 0;   *g = 0;   *b = 180;
    } else {
        *r = 0;   *g = 180; *b = 0;
    }
}

bool led_update_user(led_t led_state) {
    if (Keyboard_Info.Key_Mode != QMK_USB_MODE) {
        if (led_state.caps_lock) {
            Keyboard_Status.System_Led_Status |= 0x02;
        } else {
            Keyboard_Status.System_Led_Status &= ~0x02;
        }
    }
    rgb_matrix_set_flags(LED_FLAG_ALL);
    return true;
}

void matrix_io_delay(void) {}
void matrix_output_select_delay(void) {}
void matrix_output_unselect_delay(uint8_t line, bool key_pressed) {}

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

void kb_led_batt_number_show(void) {
    static uint16_t last_batt_timer = 0;
    if (es_stdby_pin_state == 1) {
        if (Batt_Led_Count < 25) {
            rgb_matrix_set_color(LED_STOP_INDEX + LOGO_LED_SIZE - 1, 255, 255, 255);
        } else {
            rgb_matrix_set_color(LED_STOP_INDEX + LOGO_LED_SIZE - 1, 0, 0, 0);
        }
        if (timer_elapsed(last_batt_timer) >= 4) {
            last_batt_timer = timer_read();
            if (User_Key_Batt_Count > 3) {
                User_Key_Batt_Count -= 3;
            } else {
                User_Key_Batt_Count = 127;
            }
        }
        uint8_t wave_offset = User_Key_Batt_Count;
        for (uint8_t i = BATT_LED_START_IDX; i <= BATT_LED_END_IDX; i++) {
            rgb_matrix_set_color(i, 0, wave_tab_led[wave_offset], 0);
            wave_offset = (wave_offset + 8) & 127; 
        }
        wave_offset = User_Key_Batt_Count;
        for (uint8_t i = 0; i < LOGO_LED_SIZE - 1; i++) {
             rgb_matrix_set_color(LED_STOP_INDEX + i, 0, wave_tab_led[wave_offset], 0);
             wave_offset = (wave_offset + 8) & 127;
        }
    } else if (es_stdby_pin_state == 2) {
        rgb_matrix_set_color(LED_STOP_INDEX + LOGO_LED_SIZE - 1, 0, 180, 0);
        for (uint8_t i = BATT_LED_START_IDX; i <= BATT_LED_END_IDX; i++) {
            rgb_matrix_set_color(i, 0, 180, 0);
        }
        for (uint8_t i = 0; i < LOGO_LED_SIZE - 1; i++) {
            rgb_matrix_set_color(LED_STOP_INDEX + i, 0, 180, 0);
        }
    } else {
        uint8_t led_count = (Keyboard_Info.Batt_Number + 9) / 10;
        if (led_count > 10) led_count = 10;
        uint8_t r, g, b;
        set_battery_color(Keyboard_Info.Batt_Number, &r, &g, &b);
        rgb_matrix_set_color(LED_STOP_INDEX + LOGO_LED_SIZE - 1, r, g, b);
        for (uint8_t i = BATT_LED_START_IDX; i <= BATT_LED_END_IDX; i++) {
            if ((i - BATT_LED_START_IDX) < led_count) {
                rgb_matrix_set_color(i, r, g, b);
            } else {
                rgb_matrix_set_color(i, 0, 0, 0);
            }
        }
        uint8_t logo_lit = (Keyboard_Info.Batt_Number * (LOGO_LED_SIZE - 1) + 50) / 100;
        if (Keyboard_Info.Batt_Number > 0 && logo_lit == 0) logo_lit = 1; 
        for (uint8_t i = 0; i < LOGO_LED_SIZE - 1; i++) {
            if (i < logo_lit) {
                rgb_matrix_set_color(LED_STOP_INDEX + i, r, g, b);
            } else {
                rgb_matrix_set_color(LED_STOP_INDEX + i, 0, 0, 0);
            }
        }
    }
}

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    User_Led_Show();
    if (User_Key_Batt_Num_Show) kb_led_batt_number_show();
#if LOGO_LED_ENABLE
    else if (kb_get_caps_lock_state()) {
        for (uint8_t i = 0; i < LOGO_LED_SIZE; i++) {
            uint8_t current_index = LED_STOP_INDEX + i;
            
            if (current_index >= led_min && current_index <= led_max) {
                rgb_matrix_set_color(current_index, RGB_MATRIX_MAXIMUM_BRIGHTNESS, RGB_MATRIX_MAXIMUM_BRIGHTNESS, RGB_MATRIX_MAXIMUM_BRIGHTNESS);
            }
        }
    }
#endif
    return false;
}

void notify_usb_device_state_change_user(enum usb_device_state usb_device_state) {
    bool is_configured = (usb_device_state == USB_DEVICE_STATE_CONFIGURED);
    
    if (Keyboard_Info.Key_Mode == QMK_USB_MODE) {
        Usb_If_Ok = is_configured;
        Usb_If_Ok_Led = is_configured;
        if (is_configured) Usb_If_Ok_Delay = 0;
        Usb_Suspend_Sig = !is_configured;
    } else {
        Usb_If_Ok = false;
        Usb_If_Ok_Led = false;
        Usb_Suspend_Sig = false;
    }
}

void housekeeping_task_user(void) {
    if (User_State_Fulfill_Flag) {
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

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    Usb_Change_Mode_Delay = 0;
    Usb_Change_Mode_Wakeup = false;
    return Key_Value_Dispose(keycode, record);
}
