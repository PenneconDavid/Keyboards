#pragma once

// Split transport (Pro Micro + TRRS)
#define SOFT_SERIAL_PIN D2
#define MASTER_LEFT
#define SPLIT_OLED_ENABLE

// Flash budget (ATmega32U4, 28 KiB usable): the 7-layer map with VIA, combos, tap dance, per-key
// RGB and the OLED animations only fits with these invisible trims (see also rules.mk).
#define LAYER_STATE_8BIT      // 7 layers fit in 8 bits; smaller layer code throughout
#define NO_ACTION_ONESHOT     // no one-shot keys in this layout

// VIA: seven dynamic layers (Base, Symbols, GAME_WASD, Fn, ARC, GAME_ESDF, ARC_E).
// 7 x 96 bytes of the 1 KiB EEPROM; the rest is the VIA macro buffer.
#define DYNAMIC_KEYMAP_LAYER_COUNT 7

// Game exit combo (keymap.c): keys are matched by their Base-layer keycodes, must land within
// 50 ms, and combo_should_trigger() gates them to the game layers (+150 ms prior idle).
#define COMBO_ONLY_FROM_LAYER 0
#define COMBO_TERM 50
#define COMBO_SHOULD_TRIGGER

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
#endif

// ----------------------- User-tunable defaults -----------------------
#ifndef RGB_DIAG_FORCE
#    define RGB_DIAG_FORCE 0
#endif

// Portrait rotation added on top of what crkbd.c hands oled_init_user() (0 on the left/master,
// 180 on the right/slave, which already corrects the right OLED's upside-down mounting).
// Left: 270 + 0 = 270 (the same picture the left showed before: 90 + a software flip).
// Right: 270 + 180 = 90, i.e. the opposite of the old build, which is the fix.
// If BOTH halves come up upside-down, set both to 90. If only one does, flip that side below.
#ifndef ROT_LEFT
#    define ROT_LEFT 270
#endif

#ifndef ROT_RIGHT
#    define ROT_RIGHT 270
#endif

// Per-side 180-degree software flip of the drawn picture (last-resort knob, see ROT_* above).
#undef OLED_LEFT_FLIP
#define OLED_LEFT_FLIP false

#undef OLED_RIGHT_FLIP
#define OLED_RIGHT_FLIP false

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
