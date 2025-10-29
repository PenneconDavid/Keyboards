#pragma once

// Split transport (Pro Micro + TRRS)
#define SOFT_SERIAL_PIN D2
#define MASTER_LEFT
#define SPLIT_LAYER_STATE_ENABLE
#define SPLIT_MODS_ENABLE
#define SPLIT_LED_STATE_ENABLE

// OLED (generic params; use board defaults for geometry/IC)
#define OLED_TIMEOUT       60000         // 60 sec inactivity -> off
#define OLED_BRIGHTNESS 90
#define OLED_UPDATE_INTERVAL 100

// RGB Matrix (brightness cap only; Corne info already sets counts/split)
#ifdef RGB_MATRIX_ENABLE
#  ifdef RGB_MATRIX_MAXIMUM_BRIGHTNESS
#    undef RGB_MATRIX_MAXIMUM_BRIGHTNESS
#  endif
#  define RGB_MATRIX_MAXIMUM_BRIGHTNESS 110
#  define RGB_DISABLE_WHEN_USB_SUSPENDED true

#  define ENABLE_RGB_MATRIX_RAINBOW_MOVING_CHEVRON
#  define ENABLE_RGB_MATRIX_CYCLE_ALL
#  define ENABLE_RGB_MATRIX_BREATHING
#  define ENABLE_RGB_MATRIX_BAND_SAT
#  define ENABLE_RGB_MATRIX_PIXEL_FRACTAL
#  define ENABLE_RGB_MATRIX_TYPING_HEATMAP
#  define RGB_MATRIX_SLEEP                 // off on USB suspend
#  define RGB_MATRIX_TIMEOUT 600000        // 10 min inactivity -> off
#  define SPLIT_ACTIVITY_ENABLE
#  define SPLIT_OLED_ENABLE
#endif
