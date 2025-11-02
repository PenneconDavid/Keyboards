#pragma once

// Split transport (Pro Micro + TRRS)
#define SOFT_SERIAL_PIN D2
#define MASTER_LEFT
#define SPLIT_LAYER_STATE_ENABLE
#define SPLIT_MODS_ENABLE
#define SPLIT_LED_STATE_ENABLE

// OLED configuration
#define OLED_TIMEOUT 60000        // 60s inactivity -> off
#define OLED_BRIGHTNESS 90
#define OLED_UPDATE_INTERVAL 100

#ifdef RGB_MATRIX_ENABLE
#    ifdef RGB_MATRIX_MAXIMUM_BRIGHTNESS
#        undef RGB_MATRIX_MAXIMUM_BRIGHTNESS
#    endif
#    define RGB_MATRIX_MAXIMUM_BRIGHTNESS 110
#    define RGB_DISABLE_WHEN_USB_SUSPENDED true

#    define ENABLE_RGB_MATRIX_RAINBOW_MOVING_CHEVRON
#    define ENABLE_RGB_MATRIX_CYCLE_ALL
#    define ENABLE_RGB_MATRIX_BREATHING
#    define ENABLE_RGB_MATRIX_BAND_SAT
#    define ENABLE_RGB_MATRIX_PIXEL_FRACTAL
#    define ENABLE_RGB_MATRIX_TYPING_HEATMAP

#    define RGB_MATRIX_SLEEP
#    define RGB_MATRIX_TIMEOUT 600000  // 10 minutes inactivity -> off
#    define SPLIT_ACTIVITY_ENABLE
#    define SPLIT_OLED_ENABLE
#endif

// ----------------------- User-tunable defaults -----------------------
#define OLED_MODE_GRID 0
#define OLED_MODE_DAISY 1
#define OLED_MODE_QMK 2

#ifndef RGB_DIAG_FORCE
#    define RGB_DIAG_FORCE 0
#endif

#ifndef ROT_LEFT
#    define ROT_LEFT 90
#endif

#ifndef ROT_RIGHT
#    define ROT_RIGHT 90
#endif

#ifndef DAISY_R_CENTER
#    define DAISY_R_CENTER 5
#endif

#ifndef DAISY_RX_PETAL
#    define DAISY_RX_PETAL 7
#endif

#ifndef DAISY_RY_PETAL
#    define DAISY_RY_PETAL 9
#endif

#ifndef DAISY_OFF_X
#    define DAISY_OFF_X 8
#endif

#ifndef DAISY_OFF_Y
#    define DAISY_OFF_Y 14
#endif

#ifndef DAISY_MARGIN
#    define DAISY_MARGIN 2
#endif

#ifndef OLED_LEFT_MODE
#    define OLED_LEFT_MODE OLED_MODE_QMK
#endif

#ifndef OLED_RIGHT_MODE
#    define OLED_RIGHT_MODE OLED_MODE_DAISY
#endif

#ifndef RGB_MATRIX_STARTUP_MODE
#    define RGB_MATRIX_STARTUP_MODE RGB_MATRIX_CYCLE_ALL
#endif

#ifndef RGB_MATRIX_STARTUP_HUE
#    define RGB_MATRIX_STARTUP_HUE 128
#endif

#ifndef RGB_MATRIX_STARTUP_SAT
#    define RGB_MATRIX_STARTUP_SAT 255
#endif

#ifndef RGB_MATRIX_STARTUP_VAL
#    define RGB_MATRIX_STARTUP_VAL 160
#endif

