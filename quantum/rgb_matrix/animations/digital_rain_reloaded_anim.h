#if defined(RGB_MATRIX_FRAMEBUFFER_EFFECTS) && !defined(DISABLE_RGB_MATRIX_DIGITAL_RAIN_RELOADED)
RGB_MATRIX_EFFECT(DIGITAL_RAIN_RELOADED)
#    ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

#        ifndef RGB_DIGITAL_RAIN_DROPS
// Adjust this to change density. Lower = more rain, Higher = less rain.
#            define RGB_DIGITAL_RAIN_DROPS 18
#        endif

bool DIGITAL_RAIN_RELOADED(effect_params_t* params) {

    // Grab global HSV settings so we respect VIA configuration
    HSV hsv = rgb_matrix_config.hsv;
    uint8_t global_val = hsv.v; // This is the master brightness from VIA
    
    // Fix the speed overflow issue
    // Map speed (0-255) to a delay tick. High speed = low delay.
    // Divisor of 5 gives a good range. +1 ensures we never hit 0 delay.
    const uint8_t drop_ticks = (255 - rgb_matrix_config.speed) / 5 + 1;

    // Constants for the rain physics
    const uint8_t max_intensity = 255;    // The "head" of the drop
    const uint8_t tail_intensity = 200;   // The cut-off where it stops being white and becomes colored

    static uint8_t drop_timer = 0;

    // --- INITIALIZATION ---
    if (params->init) {
        // Clear the buffer to black on startup
        memset(g_rgb_frame_buffer, 0, sizeof(g_rgb_frame_buffer));
        drop_timer = 0;
    }

    // --- RENDER LOOP (Draw the current state) ---
    for (uint8_t col = 0; col < MATRIX_COLS; col++) {
        for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
            
            uint8_t val = g_rgb_frame_buffer[row][col];
            
            // Map the matrix coordinate to the specific LED index
            uint8_t led[LED_HITS_TO_REMEMBER];
            uint8_t led_count = rgb_matrix_map_row_column_to_led(row, col, led);

            if (led_count > 0) {
                if (val == 0) {
                    // Turn LED off (Black background for high contrast)
                    rgb_matrix_set_color(led[0], 0, 0, 0);
                } 
                else if (val > tail_intensity) {
                    // THE HEAD: Render as White (but respect global brightness)
                    rgb_matrix_set_color(led[0], global_val, global_val, global_val);
                } 
                else {
                    // THE TAIL: Render as Color (User Hue)
                    // We calculate the brightness of this specific pixel based on how "faded" the rain is
                    // Then we scale that by the Global Brightness (global_val)
                    uint8_t pixel_val = (val * global_val) / 255;
                    
                    // Create a temporary HSV with the specific brightness for this pixel
                    HSV pixel_hsv = {hsv.h, hsv.s, pixel_val};
                    RGB rgb = hsv_to_rgb(pixel_hsv);
                    
                    rgb_matrix_set_color(led[0], rgb.r, rgb.g, rgb.b);
                }
            }
        }
    }

    // --- PHYSICS LOOP (Move the drops down) ---
    // Only update the positions once the timer hits the limit (determined by speed)
    if (++drop_timer > drop_ticks) {
        drop_timer = 0;

        // Move everything down and decay
        for (uint8_t row = MATRIX_ROWS - 1; row > 0; row--) {
            for (uint8_t col = 0; col < MATRIX_COLS; col++) {
                
                // If the pixel above is bright (Head or strong Tail), pull it down
                if (g_rgb_frame_buffer[row - 1][col] > 0) {
                    g_rgb_frame_buffer[row][col] = g_rgb_frame_buffer[row - 1][col];
                    
                    // Natural Decay: As it moves down, it loses brightness
                    // We decay faster if it's the head to create a separation
                    uint8_t decay = (g_rgb_frame_buffer[row][col] > tail_intensity) ? 10 : 5;
                    
                    if(g_rgb_frame_buffer[row][col] >= decay) {
                        g_rgb_frame_buffer[row][col] -= decay;
                    } else {
                        g_rgb_frame_buffer[row][col] = 0;
                    }
                } else {
                    // If pixel above is black, this one becomes black (unless it was already fading)
                     if(g_rgb_frame_buffer[row][col] > 5) {
                        g_rgb_frame_buffer[row][col] -= 5; // Fade out remaining tail
                     } else {
                        g_rgb_frame_buffer[row][col] = 0;
                     }
                }
            }
        }

        // Spawn new drops on the top row (Row 0)
        for (uint8_t col = 0; col < MATRIX_COLS; col++) {
             // Decay the top row first
            if(g_rgb_frame_buffer[0][col] > 5) {
                g_rgb_frame_buffer[0][col] -= 5;
            } else {
                g_rgb_frame_buffer[0][col] = 0;
            }

            // Random chance to spawn a new drop
            // We only spawn if the spot is currently black (prevent overlapping blobs)
            if (g_rgb_frame_buffer[0][col] == 0 && (rand() % RGB_DIGITAL_RAIN_DROPS == 0)) {
                g_rgb_frame_buffer[0][col] = max_intensity;
            }
        }
    }

    return false;
}

#    endif  // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#endif      // defined(RGB_MATRIX_FRAMEBUFFER_EFFECTS)
