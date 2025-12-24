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
#include "features/layer_lock.h"

#define BATT_LED_START_IDX 35
#define BATT_LED_END_IDX   44
#define BATT_LED_TOTAL     10
#define BATT_LED_MAX_WAVE  127

#ifdef LTO_ENABLED
__attribute__((weak, noinline)) void bootloader_jump(void) {}
__attribute__((weak, noinline)) void mcu_reset(void) {}
#endif

static inline uint8_t get_wave_value(uint8_t index) {
    uint8_t x = index & 127;
    if (x & 64) x = 128 - x; 
    uint16_t val = x << 2;
    return (val > 255) ? 255 : (uint8_t)val;
}

static inline uint8_t fast_div10(uint8_t x) {
    return ((uint16_t)x * 205) >> 11;
}

static inline uint8_t fast_div100(uint16_t x) {
    return (x * 41) >> 12;
}

#ifdef VIA_ENABLE // via exclusive feature
#include "via.h"
enum via_custom_config_value {
    id_debounce_time = 1,
    id_nkro_toggle   = 2,
    id_mac_mode      = 3,
    id_win_lock      = 4,
    id_rgb_toggle    = 5,
    id_logo_toggle   = 6
};

void custom_config_set_value(uint8_t *data) {
    switch (data[0]) {
        case id_debounce_time:
        {
            Keyboard_Info.Debounce_Delay = data[1];
            Debounce_Function_Count = (Keyboard_Info.Debounce_Delay != 2);
            break;
        }
        case id_nkro_toggle:
        {
            clear_keyboard();
            keymap_config.raw ^= 0x80;
            Keyboard_Info.Nkro ^= 1;
            break;
        }
        case id_mac_mode:
        {
            unregister_code(0x65);
            unregister_code(0xe2);
            unregister_code(0xe3);
            unregister_code(0xe6);
            unregister_code(0xe7);

            Keyboard_Info.Mac_Win_Mode ^= 1;
            if (biton(layer_state) != Keyboard_Info.Mac_Win_Mode) {
                layer_move(Keyboard_Info.Mac_Win_Mode);
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
        case id_rgb_toggle:
        {
            Keyboard_Info.Led_On_Off ^= 1;
            if (Keyboard_Info.Led_On_Off && rgb_matrix_get_val() == 0) {
                 rgb_matrix_sethsv_noeeprom(rgb_matrix_get_hue(), rgb_matrix_get_sat(), 0xB4);
            }
            break;
        }
        case id_logo_toggle:
        {
            Keyboard_Info.Logo_On_Off ^= 1;
            if (Keyboard_Info.Logo_On_Off && Keyboard_Info.Logo_Brightness == 0) {
                Keyboard_Info.Logo_Brightness = 105;
            }
            Logo_Init();
            break;
        }
    }
}

void custom_config_get_value(uint8_t *data) {
    uint8_t *result  = &data[1];
    switch (data[0]) {
        case id_debounce_time: *result = Keyboard_Info.Debounce_Delay; break;
        case id_nkro_toggle:   *result = Keyboard_Info.Nkro; break;
        case id_mac_mode:      *result = Keyboard_Info.Mac_Win_Mode; break;
        case id_win_lock:      *result = Keyboard_Info.Win_Lock; break;
        case id_rgb_toggle:    *result = !Keyboard_Info.Led_On_Off; break;
        case id_logo_toggle:   *result = !Keyboard_Info.Logo_On_Off; break;
    }
}

void custom_config_save(void) {
    Save_Flash_Set();
}

void via_custom_value_command_kb(uint8_t *data, uint8_t length) {
    if (data[1] == id_custom_channel) {
        uint8_t *payload = data + 2; 
        switch (data[0]) {
            case id_custom_set_value: custom_config_set_value(payload); break;
            case id_custom_get_value: custom_config_get_value(payload); break;
            case id_custom_save:      custom_config_save(); break;
            default:                  data[0] = id_unhandled; break;
        }
        return;
    }
    data[0] = id_unhandled;
}
#endif

static const uint8_t BATT_COLOR_LUT[4][3] = {
    {180, 0,   0},
    {180, 180, 0},
    {0,   0,   180},
    {0,   180, 0}
};

static inline bool kb_get_caps_lock_state(void) {
    if (Keyboard_Info.Key_Mode == QMK_USB_MODE) {
        return host_keyboard_led_state().caps_lock && Usb_If_Ok_Led;
    }
    return ((Keyboard_Status.System_Led_Status << 30) < 0);
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
    const uint8_t pin_state = es_stdby_pin_state;
    const uint8_t current_batt = Keyboard_Info.Batt_Number;
    const bool is_charging = (pin_state == 1);
    const bool is_full     = (pin_state == 2);
    uint8_t r, g, b;
    uint8_t batt_len_lit;
#if LOGO_LED_ENABLE
    uint8_t logo_len_lit;
#endif
    uint8_t wave_offset = 0;
    bool use_wave = false;
    if (is_charging) {
        uint8_t interval = (Keyboard_Info.Led_On_Off) ? 8 : 4;
        if (timer_elapsed(last_batt_timer) >= interval) {
            last_batt_timer = timer_read();
            uint8_t cnt = User_Key_Batt_Count;
            if (cnt > 3) cnt -= 3;
            else cnt = 127;
            User_Key_Batt_Count = cnt;
        }
        wave_offset = User_Key_Batt_Count;
        use_wave = true;
    } else {
        last_batt_timer = 0;
    }
    if (is_full) {
        r = 0; g = 180; b = 0;
        batt_len_lit = BATT_LED_TOTAL;
#if LOGO_LED_ENABLE
        logo_len_lit = LOGO_LED_SIZE - 1;
        rgb_matrix_set_color(LED_STOP_INDEX + LOGO_LED_SIZE - 1, 0, 180, 0);
#endif
    } else {
        uint8_t c_idx = 3;
        if (current_batt <= 10) c_idx = 0;
        else if (current_batt < 50) c_idx = 1;
        else if (current_batt < 80) c_idx = 2;
        r = BATT_COLOR_LUT[c_idx][0];
        g = BATT_COLOR_LUT[c_idx][1];
        b = BATT_COLOR_LUT[c_idx][2];
        batt_len_lit = fast_div10(current_batt + 9);
        if (batt_len_lit > BATT_LED_TOTAL) batt_len_lit = BATT_LED_TOTAL;
#if LOGO_LED_ENABLE
        uint16_t logo_calc = current_batt * (LOGO_LED_SIZE - 1) + 50;
        logo_len_lit = fast_div100(logo_calc); 
        if (current_batt > 0 && logo_len_lit == 0) logo_len_lit = 1;
        if (is_charging) {
            uint8_t val = (Batt_Led_Count < 25) ? 255 : 0;
            rgb_matrix_set_color(LED_STOP_INDEX + LOGO_LED_SIZE - 1, val, val, val);
        } else {
            rgb_matrix_set_color(LED_STOP_INDEX + LOGO_LED_SIZE - 1, r, g, b);
        }
#endif
    }
    uint8_t current_led_idx = BATT_LED_START_IDX;
    uint8_t cur_wave = wave_offset;
    uint8_t count = 0;
    do {
        if (count < batt_len_lit) {
            if (use_wave) {
                uint8_t wave = get_wave_value(cur_wave);
                rgb_matrix_set_color(current_led_idx, (r * wave) >> 8, (g * wave) >> 8, (b * wave) >> 8);
                cur_wave = (cur_wave + 8) & 127;
            } else {
                rgb_matrix_set_color(current_led_idx, r, g, b);
            }
        } else {
            rgb_matrix_set_color(current_led_idx, 0, 0, 0);
        }
        current_led_idx++;
        count++;
    } while (count < BATT_LED_TOTAL);
#if LOGO_LED_ENABLE
    current_led_idx = LED_STOP_INDEX;
    cur_wave = wave_offset;
    const uint8_t logo_limit = LOGO_LED_SIZE - 1;
    count = 0;
    if (logo_limit > 0) {
        do {
            if (count < logo_len_lit) {
                 if (use_wave) {
                    uint8_t wave = get_wave_value(cur_wave);
                    rgb_matrix_set_color(current_led_idx, (r * wave) >> 8, (g * wave) >> 8, (b * wave) >> 8);
                    cur_wave = (cur_wave + 8) & 127;
                } else {
                    rgb_matrix_set_color(current_led_idx, r, g, b);
                }
            } else {
                rgb_matrix_set_color(current_led_idx, 0, 0, 0);
            }
            current_led_idx++;
            count++;
        } while (count < logo_limit);
    }
#endif
}

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    User_Led_Show();
    if (User_Key_Batt_Num_Show) {
        kb_led_batt_number_show();
    } 
#if LOGO_LED_ENABLE
    else if (kb_get_caps_lock_state()) {
        uint8_t start_idx = LED_STOP_INDEX;
        uint8_t end_idx = LED_STOP_INDEX + LOGO_LED_SIZE;
        if (end_idx >= led_min && start_idx <= led_max) {
            uint8_t loop_start = (start_idx > led_min) ? start_idx : led_min;
            uint8_t loop_end   = (end_idx   < led_max) ? end_idx   : led_max;
            for (uint8_t i = loop_start; i < loop_end; i++) {
                rgb_matrix_set_color(i, RGB_MATRIX_MAXIMUM_BRIGHTNESS, RGB_MATRIX_MAXIMUM_BRIGHTNESS, RGB_MATRIX_MAXIMUM_BRIGHTNESS);
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
    if (!process_layer_lock(keycode, record, QK_LAYER_LOCK)) { return false; }
    return Key_Value_Dispose(keycode, record);
}