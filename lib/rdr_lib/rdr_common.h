#pragma once

#include "quantum.h"
#include "raw_hid.h"
#include "usb_main.h"
#include "report.h"
#include "action_util.h"

/************************ I/O Ports **************************/
/************************ I/O Ports **************************/
/************************ I/O Ports **************************/
#define ES_BATT_ADC_IO      (C4)
#define ES_BATT_STDBY_IO    (A13)
#define ES_USB_POWER_IO     (C5)
#define ES_SPI_ACK_IO       (A4)
#define ES_PWM_DMA_IO       (A2)

#define ES_WUKEUP_IO        (D1)
#define ES_SDB_POWER_IO     (A3)
#define ES_LED_POWER_IO     (D0)


/************************ SPI Commands **************************/
/************************ SPI Commands **************************/
/************************ SPI Commands **************************/
#define USER_EMI_COMMAND	    0XBB
#define USER_KEYBOARD_COMMAND	0X0A
#define USER_KEYBOARD_LENGTH    (64)

#define USER_SWITCH_2P4G_MODE	0X00
#define USER_SWITCH_BLE_1_MODE	0X01
#define USER_SWITCH_BLE_2_MODE	0X02
#define USER_SWITCH_BLE_3_MODE	0X03
#define USER_SWITCH_2P4G_PAIR	0X04
#define USER_SWITCH_BLE_1_PAIR	0X05
#define USER_SWITCH_BLE_2_PAIR	0X06
#define USER_SWITCH_BLE_3_PAIR	0X07
#define USER_SWITCH_USB_MODE	0X08

#define USER_KEYBOARD_SLEEP		0X09
#define USER_KEYBOARD_WAKEUP	0X0A

#define USER_KEY_BYTE_DATA		0X0B
#define USER_KEY_BIT_DATA		0X0C
#define USER_MOUSE_DATA			0X0D
#define USER_CONSUMER_DATA		0X0E
#define USER_SYSTEM_DATA		0X0F

#define USER_BATTERY_DATA		0X10

#define USER_GET_RF_STATUS	    0X11

#define USER_BLE1_WRITE_NAME	0X12
#define USER_BLE2_WRITE_NAME    0X13
#define USER_BLE3_WRITE_NAME    0X14

#define USER_KEY_BYTE_LENGTH	0X08
#define USER_KEY_BIT_LENGTH		0X0F
#define USER_MOUSE_LENGTH		0X08
#define USER_CONSUMER_LENGTH	0X03
#define USER_SYSTEM_LENGTH		0X03
#define USER_BATTERY_LENGTH		0X02

#define KB_REPORT_ID            0x06                 // Extend keyboard report ID.
#define SYS_REPORT_ID     	    0x03                 // Extend System   report ID.
#define CON_REPORT_ID     	    0x04                 // Extend Consumer report ID.
#define MOUSE_REPORT_ID  	    0x02                 // Extend mouse	report ID.

#define LOGO_LED_ENABLE         (1)
#define SIDE_LED_ENABLE         (0)

/************************ Core Definitions **************************/
/************************ Core Definitions **************************/
/************************ Core Definitions **************************/
enum Custom_KeyModes {
    QMK_BLE_MODE = 0,
    QMK_2P4G_MODE,
    QMK_USB_MODE
};

enum Custom_BleChannels {
    QMK_BLE_CHANNEL_1 = 1,
    QMK_BLE_CHANNEL_2,
    QMK_BLE_CHANNEL_3
};

enum Custom_Spi_Ack_S {
    SPI_NACK,
    SPI_ACK
};

enum Custom_Spi_Busy_S {
    SPI_BUSY,
    SPI_IDLE
};

enum Custom_Ble_24G_Status_S {
    BLE_24G_NONE,
    BLE_24G_PIAR,
    BLE_24G_RETURN
};

typedef enum {
    KB_MODE_CONNECT_OK,   	        // Connection successful
    KB_MODE_CONNECT_PAIR,        	// Pairing
    KB_MODE_CONNECT_RETURN,        	// Reconnected
} keyboard_System_state_e;

typedef enum {
    USER_SLEEP_PASS,	            // Sleep success
    USER_SLEEP_FIAL,            	// Sleep failure
} keyboard_System_Sleep_Status_s;

/************************ Default Work Modes **************************/
/************************ Default Work Modes **************************/
/************************ Default Work Modes **************************/
#define INIT_WORK_MODE          (QMK_USB_MODE)       // Default work mode
#define INIT_BLE_CHANNEL        (QMK_BLE_CHANNEL_1)  // Default Bluetooth channel
#define INIT_BATT_NUMBER        (50)                 // Default battery percentage on power-up

#define INIT_SIX_KEY            (0)                  // Six-key (6KRO)
#define INIT_ALL_KEY            (1)                  // Full-key (NKRO)
#define INIT_ALL_SIX_KEY        (INIT_ALL_KEY)       // Full-key (NKRO)

#define INIT_WIN_MODE           (0)                  // Windows mode
#define INIT_MAC_MODE           (1)                  // Mac mode
#define INIT_WIN_MAC_MODE       (INIT_WIN_MODE)      // Default: Windows mode

#define INIT_WIN_NLOCK          (0)                  // Windows key not locked
#define INIT_WIN_LOCK           (1)                  // Windows key locked
#define INIT_WIN_LOCK_NLOCK     (INIT_WIN_NLOCK)     // Default: Windows key not locked

#define INIT_LED_ON             (0)                  // Backlight ON   +
#define INIT_LED_OFF            (1)                  // Backlight OFF  +
#define INIT_LED_ON_OFF         (INIT_LED_ON)        // Default backlight state

#define DEBOUNCE_DELAY_ONE      (2)                  // Debounce level 1
#define DEBOUNCE_DELAY_TWO      (5)                  // Debounce level 2
#define DEBOUNCE_DELAY_CLASS    (DEBOUNCE_DELAY_TWO) // Default debounce level

#define USER_DEFINE_KEY         (QK_KB)
enum Custom_Keycodes {
    QMK_KB_MODE_2P4G = USER_DEFINE_KEY,
    QMK_KB_MODE_BLE1,
    QMK_KB_MODE_BLE2,
    QMK_KB_MODE_BLE3,
    QMK_KB_MODE_USB,
    QMK_BATT_NUM,
    QMK_WIN_LOCK,
    QMK_KB_SIX_N_CH,
    RGB_RTOG,
    U_EE_CLR, // +
    IOS_SIRI, // +
    QMK_DEBOUNCE, // +
#if LOGO_LED_ENABLE
    LOGO_TOG,
    LOGO_MOD,
    LOGO_RMOD,
    LOGO_HUI,
    LOGO_HUD,
    LOGO_SAI,
    LOGO_SAD,
    LOGO_VAI,
    LOGO_VAD,
    LOGO_SPI,
    LOGO_SPD,
#endif
#if SIDE_LED_ENABLE
    SIDE_TOG,
    SIDE_MOD,
    SIDE_RMOD,
    SIDE_HUI,
    SIDE_HUD,
    SIDE_SAI,
    SIDE_SAD,
    SIDE_VAI,
    SIDE_VAD,
    SIDE_SPI,
    SIDE_SPD,
#endif
    QMK_KB_2P4G_PAIR,
    QMK_KB_BLE1_PAIR,
    QMK_KB_BLE2_PAIR,
    QMK_KB_BLE3_PAIR
};

#define WIN_COL                 (1)
#define WIN_ROL                 (3)

#define MAC_COL                 (2)
#define MAC_ROL                 (3)

#define KC_K29                  KC_BACKSLASH
#define KC_K42                  KC_NONUS_HASH
#define KC_K45                  KC_NONUS_BACKSLASH
#define KC_K56                  KC_INTERNATIONAL_1
#define KC_K14                  KC_INTERNATIONAL_3
#define KC_K132                 KC_INTERNATIONAL_4
#define KC_K131                 KC_INTERNATIONAL_5
#define KC_K133                 KC_INTERNATIONAL_2
#define KC_K151                 KC_LANGUAGE_1
#define KC_K150                 KC_LANGUAGE_2

#define MD_24G	                QMK_KB_MODE_2P4G
#define MD_BLE1	                QMK_KB_MODE_BLE1
#define MD_BLE2	                QMK_KB_MODE_BLE2
#define MD_BLE3	                QMK_KB_MODE_BLE3
#define MD_USB	                QMK_KB_MODE_USB
#define QK_BAT                  QMK_BATT_NUM
#define QK_WLO	                QMK_WIN_LOCK
#define SIX_N	                QMK_KB_SIX_N_CH
#define KEY_DEB                 QMK_DEBOUNCE

/************************基本变量**************************/
/************************基本变量**************************/
/************************基本变量**************************/
typedef struct {
    uint8_t Key_Mode;               // Keyboard work mode
    uint8_t Key_Mode_Old;           // Previous keyboard work mode
    uint8_t Ble_Channel;            // 蓝牙通道
    uint8_t Batt_Number;            // Battery level (percentage)
    uint8_t Nkro;                   // NKRO / 6-key rollover (non-conflict)
    uint8_t Mac_Win_Mode;           // Mac/Windows mode
    uint8_t Win_Lock;               // Windows key lock
    uint8_t Led_On_Off;             // 背光开关
    uint8_t Debounce_Delay;         // Key debounce
#if LOGO_LED_ENABLE
    uint8_t Logo_On_Off;            // Logo LED on/off
    uint8_t Logo_Mode;              // Logo LED mode
    uint8_t Logo_Colour;            // Logo LED color
    uint8_t Logo_Saturation;        // Logo LED saturation
    uint8_t Logo_Brightness;        // Logo LED brightness
    uint8_t Logo_Speed;             // Logo LED speed
#endif
#if SIDE_LED_ENABLE
    uint8_t Side_On_Off;            // Side LED on/off
    uint8_t Side_Mode;              // Side LED mode
    uint8_t Side_Colour;            // Side LED color
    uint8_t Side_Saturation;        // Side LED saturation
    uint8_t Side_Brightness;        // Side LED brightness
    uint8_t Side_Speed;             // Side LED speed
#endif
} Keyboard_Info_t;
extern Keyboard_Info_t Keyboard_Info;

typedef struct {
    uint8_t System_Work_Status;     // System status
    uint8_t System_Work_Mode;       // Work mode
    uint8_t System_Work_Channel;    // Work channel
    uint8_t System_Connect_Status;  // Connection status
    uint8_t System_Led_Status;      // System indicator LED status
    uint8_t System_Sleep_Mode;      // System sleep mode
} Keyboard_Status_t;
extern Keyboard_Status_t Keyboard_Status;

bool     Key_2p4g_Status;
bool     Key_Ble_1_Status;
bool     Key_Ble_2_Status;
bool     Key_Ble_3_Status;
bool     Key_Fn_Status;
//bool   Key_Reset_Status;
//bool   Keyboard_Reset;
uint8_t  Systick_6ms_Count;
uint8_t  Systick_10ms_Count;
uint16_t Systick_Interval_Count;
uint16_t Time_3s_Count;
uint16_t Time_Key_Press_3s_Count; // +
//uint16_t Func_Time_3s_Count;

/************************ Data Queue **************************/
/************************ Data Queue **************************/
/************************ Data Queue **************************/
#define APP_2G4_BUF_SIZE            (USER_KEYBOARD_LENGTH)
#define APP_2G4_BUF_CNT             (20)

uint8_t app_2g4_data[APP_2G4_BUF_CNT][APP_2G4_BUF_SIZE];
volatile uint8_t app_2g4_data_send;
volatile uint8_t app_2g4_data_rev;

uint8_t app_2g4_buffer_full(void);
uint8_t app_2g4_buffer_empty(void);
void app_2g4_buffer_rev_add(void);
void app_2g4_buffer_send_add(void);

/**************************EMI****************************/
/**************************EMI****************************/
/**************************EMI****************************/
bool Emi_Test_Start;
void Emi_Init(void);
void Emi_Read_Data(uint8_t *User_Data, uint8_t User_Length);
void Emi_Write_Data(uint8_t *User_Data, uint8_t User_Length);

/**************************SPI****************************/
/**************************SPI****************************/
/**************************SPI****************************/
#define SPI_DELAY_RF_TIME           (60)
#define SPI_DELAY_USB_TIME          (500 * 3)

#define MAX_NAME_LEN                (18)
#define USER_BlE_ID                 (0X000C)
#define USER_BlE1_NAME              "EVO80 BT1"
#define USER_BlE2_NAME              "EVO80 BT2"
#define USER_BlE3_NAME              "EVO80 BT3"

volatile uint8_t Spi_Send_Recv_Flg;
uint16_t Spi_Interval;
uint8_t  g_es_spi_rx_buf[64];
uint8_t  g_es_spi_tx_buf[64];
uint8_t  Repet_Send_Count;
uint8_t  Send_Key_Type;
bool     Init_Spi_Power_Up;
uint8_t  Init_Spi_100ms_Delay;
bool     Ble_Name_Spi_Send;
uint8_t  Ble_Name_Spi_Count;

const uint32_t g_es_dma_ch2pri_cfg;
const uint32_t g_es_dma_ch2alt_cfg;
const md_spi_inittypedef SPI2_InitStruct;

void es_ble_spi_init(void);
void es_ble_spi_deinit(void);
void es_spi_send_recv_by_dma(uint32_t num, uint8_t *rx_buf, uint8_t *tx_buf);
void Spi_Main_Loop(void);
void Spi_Send_Commad(uint8_t Commad);
uint8_t Spi_Ack_Send_Commad(uint8_t Commad);
void Get_Spi_Return_Data(uint8_t *Data);

/************************* Mode Switching ****************************/
/************************* Mode Switching ****************************/
/************************* Mode Switching ****************************/
volatile host_driver_t *es_qmk_driver;
const    host_driver_t es_user_driver;

uint8_t es_keyboard_leds(void);
void es_send_keyboard(report_keyboard_t *report);
void es_send_nkro(report_nkro_t *report);
void es_send_mouse(report_mouse_t *report);
void es_send_extra(report_extra_t *report);
void Mode_Synchronization(void);
void Ble_Name_Synchronization(void);
void User_bluetooth_send_keyboard(uint8_t *report, uint32_t len);

/************************ Key Debounce **************************/
/************************ Key Debounce **************************/
/************************ Key Debounce **************************/
unsigned int Debounce_Delay;                      // Debounce count for keys, max 127
uint8_t  Debounce_Point_Count;
uint16_t User_Key_3s_Count;
bool     Debounce_Function_Count;
bool     Debounce_Function_Status;

/************************ Keyboard Restore/Init Variables **************************/
/************************ Keyboard Restore/Init Variables **************************/
/************************ Keyboard Restore/Init Variables **************************/
// uint16_t Time_3s_EE_CLR_Count;   // +
// bool     User_QMK_EE_CLR_Flag;   // +
// bool     User_EE_CLR_Start_Flag; // +

/************************ Keyboard State Control Variables **************************/
/************************ Keyboard State Control Variables **************************/
/************************ Keyboard State Control Variables **************************/
uint16_t User_State_Flag;
uint16_t User_State_Count;
uint16_t User_State_Fulfill_Flag;
bool     User_State_EE_CLR_LED_Flag;
bool     User_State_Fulfill_LED_Flag;
bool     User_State_DEL_INS_Flag;

/************************ 6-Key Release Buffer Shift **************************/
/************************ 6-Key Release Buffer Shift **************************/
/************************ 6-Key Release Buffer Shift **************************/
// void General_Key_Reorder(uint8_t Spot_Index);
// void del_key_from_report(uint8_t key);

/************************ Matrix Key Related Variables **************************/
/************************ Matrix Key Related Variables **************************/
/************************ Matrix Key Related Variables **************************/
bool Key_Win_Status;

bool Key_Value_Dispose(uint16_t keycode, keyrecord_t *record);

/************************** System Functions ****************************/
/************************** System Functions ****************************/
/************************** System Functions ****************************/
#define KEYBAORD_COL                (16)
#define KEYBAORD_ROL                (6)

#define MATRIX_USER_COL_PINS        { D15, D14, C15, C14, C13, D3, D2, C12, C11, C10, A14, C9, C8, C7, C6, B15 }
#define MATRIX_USER_ROW_PINS        { B0, B3, B4, B5, B6, B7 }

bool     Save_Flash;
bool     Reset_Save_Flash;
uint16_t Save_Flash_3S_Count;
bool     Led_Rf_Pair_Flg;
bool     Usb_Change_Mode_Wakeup;
uint8_t  Temp_System_Led_Status;
bool     Mode_Synchronization_Signal;
uint16_t g_usb_sof_frame_id;
uint16_t g_usb_sof_frame_id_last;
bool     Usb_Dis_Connect;
uint16_t Usb_Suspend_Delay;
uint16_t Usb_Change_Mode_Delay;

void es_mcu_reset(void);
void bootloader_jump(void);
void mcu_reset(void);
void User_Mac_Win_Change(void);
void User_Keyboard_Reset(void);
void Save_Flash_Set(void);
void User_Systime_Init(void);
void User_Systime_Deinit(void);
void Init_Gpio_Infomation(void);
void User_Sleep(void);
void User_Wakeup(void);
void Board_Wakeup_Init(void);
void es_chibios_user_idle_loop_hook(void);
void Init_Keyboard_Infomation(void);
void es_change_qmk_nkro_mode_enable(void);
void es_change_qmk_nkro_mode_disable(void);
void User_Keyboard_Init(void);
void User_Keyboard_Post_Init(void);
/************************ USB Plugin **************************/
/************************ USB Plugin **************************/
/************************ USB Plugin **************************/
void User_Usb_Init(void);
void es_restart_usb_driver(void);
void Usb_Disconnect(void);
void User_Usb_Deinit(void);

/**************************FLASH****************************/
/**************************FLASH****************************/
/**************************FLASH****************************/
void eeprom_driver_init(void);
void eeprom_write_block_user(const void *buf, void *addr, size_t len);
void eeprom_read_block_user(void *buf, const void *addr, size_t len);

/************************* Battery ******************************/
/************************* Battery ******************************/
/************************* Battery ******************************/
#define USER_BATT_POWER_SCAN_COUNT  (10)
#define USER_BATT_SCAN_COUNT        (10)

#define USER_BATT_HIGH_POWER        (2555)      // Full battery threshold: ADC 2555 -> 2555 * 3.3 /4096 = 2.058 (ADC voltage), battery ~4.13V; note circuit voltage drop.
#define USER_BATT_LOW_POWER         (2080)      // Low battery threshold: ADC 2080 -> 2080 * 3.3 /4096 = 1.676 (ADC voltage), battery ~3.32V.
#define USER_BATT_STDOWN_POWER      (1865)      // Shutdown threshold: ADC 1865 -> 1865 * 3.3 /4096 = 1.502 (ADC voltage), battery ~3.04V; board input ~4.1V.

#define USER_BATT_DELAY_TIME        (100 * 25)  // 25 seconds
#define USER_TIME_3S_TIME           (300)       // 3 seconds
#define USER_TIME_2S_TIME           (200)       // 2 seconds
#define USER_TIME_10S_TIME          (1000)      // 10 seconds

uint16_t User_Adc_Batt[USER_BATT_SCAN_COUNT];
uint16_t User_Scan_Batt[USER_BATT_SCAN_COUNT];
uint8_t  User_Adc_Batt_Count;
uint8_t  User_Batt_BaiFen;
uint8_t  User_Batt_Old_BaiFen;
uint8_t  User_Batt_10ms_Count;
uint16_t User_Batt_Time_15S_Count;
bool     User_Batt_Power_Up;
bool     User_Batt_Send_Spi;
uint16_t User_Batt_Power_Up_Delay_100ms_Count;
bool     User_Batt_Power_Up_Delay;
bool     User_Power_Low;
uint8_t  User_Power_Low_Count;
uint8_t  es_stdby_pin_state;
bool     User_Key_Batt_Num_Show;
uint8_t  User_Key_Batt_Count;
uint8_t  Batt_Led_Count;
bool     User_Key_Batt_Num_Flag; // +
bool     User_Key_Batt_LED_Flag; // +
bool     WIN_MAC_CHANGE;         // +

const md_adc_initial adc_initStruct;

void Init_Batt_Infomation(void);
void User_Adc_Init(void);
void User_Adc_Deinit(void);
void U16_Buff_Clear(uint16_t *Buff, uint8_t Len);
void User_Adc_Batt_Power_Up_Init(void);
void User_Adc_Batt_Number(void);

/************************ Main LED Lights **************************/
/************************ Main LED Lights **************************/
/************************ Main LED Lights **************************/
#define ES_PWM_LED_SIZE         (42)
#define ES_PWM_LED_BYTE         (24)
#define ES_PWM_DMA_SIZE         (ES_PWM_LED_SIZE * ES_PWM_LED_BYTE)

#define ES_PWM_WS2812_H_VALUE   (43)
#define ES_PWM_WS2812_L_VALUE   (17)

#define U_PWM                   (RGB_MATRIX_MAXIMUM_BRIGHTNESS)

#define LED_ESC_INDEX       (0)
#define LED_GRAVE_INDEX     (17)
#define LED_CAP_INDEX       (52)
#define LED_CAP_1_INDEX     (53)
#define LED_SCR_INDEX       (15)
#define LED_WIN_L_INDEX     (81)

#define LED_BLE_1_INDEX     (18)
#define LED_BLE_2_INDEX     (19)
#define LED_BLE_3_INDEX     (20)
#define LED_2P4G_INDEX      (21)
#define LED_USB_INDEX       (17)

#define LED_START_INDEX     (0)
#define LED_STOP_INDEX      (91)

uint8_t Led_Colour_Tab[9][3];
uint8_t Led_Wave_Pwm_Tab[128];
uint8_t Led_Batt_Index_Tab[10];

uint8_t  Systick_Led_Count;
//uint8_t  Point_Flash_Count;
uint8_t  Led_Point_Count;
uint8_t  Mac_Win_Point_Count;
uint16_t Usb_If_Ok_Delay; // +
bool     Led_Flash_Busy;
bool     Led_Off_Start;
bool     Led_Power_Up;
uint16_t Led_Power_Up_Delay;
bool     Usb_If_Ok_Led;
bool     Usb_Suspend_Sig; // +
bool     Usb_If_Ok; // +
bool     Led_Point_Sleep; // +

rgb_led_t rgb_matrix_ws2812_array[RGB_MATRIX_LED_COUNT];
uint8_t g_es_pwm_rgb_matrix_array_dma_buf[(RGB_MATRIX_LED_COUNT * ES_PWM_LED_BYTE) + 2];
md_dma_channel_config_typedef DMA_list[5];

void rgb_matrix_driver_init(void);
void User_Pwm_Deinit(void);
void rgb_matrix_driver_flush_pwm_dma_start(void);
void rgb_matrix_driver_flush(void);
void rgb_matrix_driver_set_color(int index, uint8_t r, uint8_t g, uint8_t b);
void rgb_matrix_driver_set_color_all(uint8_t r, uint8_t g, uint8_t b);
const rgb_matrix_driver_t rgb_matrix_driver;

void Led_Power_Low_Show(void);
void Led_Rf_Mode_Show(void);
void Led_Batt_Number_Show(void);
void Led_Point_Flash_Show(void);
void User_Led_Show(void);
void User_Get_Led_Power_Status(void);

/************************ Logo LED Lights **************************/
/************************ Logo LED Lights **************************/
/************************ Logo LED Lights **************************/
#if LOGO_LED_ENABLE
#define LOGO_LED_PLAY_SPEED	        (0)                              // LED update speed
#define LOGO_LED_SIZE	            (5)                              // Number of LEDs

#define LOGO_LED_ON                 (0)                              // LED ON
#define LOGO_LED_OFF                (1)                              // LED OFF

#define LOGO_WAVE_RGB_MODE          (1)                              // Color wave
#define LOGO_WAVE_1_RGB_MODE        (2)                              // Color wave 1
#define LOGO_WAVE_2_RGB_MODE        (3)                              // Color wave 2
#define LOGO_WAVE_DS_MODE           (4)                              // Single-color wave
#define LOGO_WAVE_MIX_MODE          (5)                              // Color wave mix
#define LOGO_SPECTRUM_MODE          (6)                              // Spectrum
#define LOGO_BREATH_MODE            (7)                              // Single-color breathing
#define LOGO_BREATH_MIX_MODE        (8)                              // Color breathing
#define LOGO_LIGHT_MODE             (9)                              // Single-color steady
#define LOGO_OFF_MODE               (10)                             // Off

#define LOGO_MAX_COLOUR             (255)                            // Max color value
#define LOGO_MIN_COLOUR             (0)                              // Min color value
#define COLOUR_LEVEL                (15)                             // Color level steps

#define LOGO_MAX_SATURATION         (0)                              // Max saturation
#define LOGO_MIN_SATURATION         (255)                            // Min saturation
#define SATURATION_LEVEL            (15)                             // Saturation level steps

#define LOGO_MAX_BRIGHTNESS         (105)                            // Max brightness
#define LOGO_MIN_BRIGHTNESS         (0)                              // Min brightness
#define BRIGHTNESS_LEVEL            (15)                             // Brightness level steps

#define LOGO_MAX_SPEED              (4)                              // Max speed
#define LOGO_MIN_SPEED              (0)                              // Min speed
#define SPEED_LEVEL                 (1)                              // Speed level steps

#define INIT_LOGO_ON_OFF            (LOGO_LED_ON)                    // LED ON
#define INIT_LOGO_MODE              (LOGO_WAVE_RGB_MODE)             // Color wave default
#define INIT_LOGO_COLOUR            (LOGO_MIN_COLOUR)                // Default color (min)
#define INIT_LOGO_SATURATION        (LOGO_MAX_SATURATION)            // Default saturation (max)
#define INIT_LOGO_BRIGHTNESS        (60)                             // Default brightness
#define INIT_LOGO_SPEED             (2)                              // Default speed (center)

uint8_t Logo_Flash_Count;
uint8_t Logo_Led_Count;
uint8_t LED_Mix_Colour_Tab[256][3];
uint8_t Logo_Index_Tab[LOGO_LED_SIZE];
uint8_t Led_Point_buf[10];
void    Logo_Init(void);
void    Logo_Mode_Show(void);
void    User_Via_Qmk_Logo_Get_Value(uint8_t *data);
void    User_Via_Qmk_Logo_Set_Value(uint8_t *data);
void    User_Via_Qmk_Logo_Command(uint8_t *data, uint8_t length);

#endif
//--------------------------------------------------------------------------------------------------------
#if SIDE_LED_ENABLE
#define SIDE_LED_PLAY_SPEED	        (0)                                 // LED update speed
#define SIDE_LED_SIZE	            (38)                                // Number of side LEDs

#define SIDE_LED_ON                 (0)                                 // LED ON
#define SIDE_LED_OFF                (1)                                 // LED OFF

#define SIDE_WAVE_RGB_MODE          (1)                                 // Color wave
#define SIDE_SPECTRUM_MODE          (2)                                 // Spectrum
#define SIDE_LIGHT_MODE             (3)                                 // Single-color steady
#define SIDE_LIGHT_MODE_GREEN       (4)                                 // Green steady
#define SIDE_LIGHT_MODE_BLUE        (5)                                 // Blue steady
#define SIDE_LIGHT_MODE_PINK        (6)                                 // Pink steady
#define SIDE_LIGHT_MODE_YELLOW      (7)                                 // Yellow steady
#define SIDE_LIGHT_MODE_WHITE       (8)                                 // White steady
#define SIDE_OFF_MODE               (9)                                 // Off

#define SIDE_MAX_COLOUR             (255)                               // Max color value
#define SIDE_MIN_COLOUR             (0)                                 // Min color value
#define SIDE_COLOUR_LEVEL           (15)                                // Color level steps

#define SIDE_MAX_SATURATION         (0)                                 // Max saturation
#define SIDE_MIN_SATURATION         (255)                               // Min saturation
#define SIDE_SATURATION_LEVEL       (15)                                // Saturation level steps

#define SIDE_MAX_BRIGHTNESS         (70)                                // Max brightness
#define SIDE_MIN_BRIGHTNESS         (0)                                 // Min brightness
#define SIDE_BRIGHTNESS_LEVEL       (15)                                // Brightness level steps

#define SIDE_MAX_SPEED              (4)                                 // Max speed
#define SIDE_MIN_SPEED              (0)                                 // Min speed
#define SIDE_SPEED_LEVEL            (1)                                 // Speed level steps

#define INIT_SIDE_ON_OFF            (SIDE_LED_ON)                       // LED ON
#define INIT_SIDE_MODE              (SIDE_WAVE_RGB_MODE)                // Color wave default
#define INIT_SIDE_COLOUR            (SIDE_MIN_COLOUR)                   // Default color (min)
#define INIT_SIDE_SATURATION        (SIDE_MAX_SATURATION)               // Default saturation (max)
#define INIT_SIDE_BRIGHTNESS        (SIDE_MAX_BRIGHTNESS)               // Default brightness
#define INIT_SIDE_SPEED             (2)                                 // Default speed (center)

//uint8_t Side_Flash_Count;
uint8_t Side_Led_Count;
uint8_t Side_Index_Tab[SIDE_LED_SIZE];
void Side_Init(void);
void Side_Mode_Show(void);
void User_Via_Qmk_Side_Get_Value(uint8_t *data);
void User_Via_Qmk_Side_Set_Value(uint8_t *data);
void User_Via_Qmk_Side_Command(uint8_t *data, uint8_t length);

#endif