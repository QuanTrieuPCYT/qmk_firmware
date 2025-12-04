#ifdef ENABLE_RGB_MATRIX_STARLIGHT_SMOOTH
RGB_MATRIX_EFFECT(STARLIGHT_SMOOTH)
#    ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

static uint8_t smooth_phase_offsets[RGB_MATRIX_LED_COUNT];

static void set_starlight_smooth_color(int i, effect_params_t* params) {
    if (smooth_phase_offsets[i] == 0) {
        smooth_phase_offsets[i] = rand() % 255;
    }

    HSV hsv = rgb_matrix_config.hsv;

    uint8_t time = scale16by8(g_rgb_timer, rgb_matrix_config.speed / 4);

    uint8_t math_val = abs8(sin8((time + smooth_phase_offsets[i]) / 2) - 128) * 2;
    hsv.v = scale8(math_val, hsv.v);

    RGB rgb = hsv_to_rgb(hsv);
    rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
}

bool STARLIGHT_SMOOTH(effect_params_t* params) {
    if (params->init) {
        memset(smooth_phase_offsets, 0, sizeof(smooth_phase_offsets));
        return false;
    }

    RGB_MATRIX_USE_LIMITS(led_min, led_max);
    for (int i = led_min; i < led_max; i++) {
        set_starlight_smooth_color(i, params);
    }

    return rgb_matrix_check_finished_leds(led_max);
}

#    endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#endif     // ENABLE_RGB_MATRIX_STARLIGHT_SMOOTH
