#include QMK_KEYBOARD_H

// --------------------------- USER CONFIG ---------------------------
// Daisy OLED art toggle
#define OLED_SHOW_DAISY 1
// VIA owns RGB Matrix animations; set to 1 to force a brief test pattern.
#define RGB_DIAG_FORCE 0

// Portrait panels usually like 90°; if yours are sideways, change both to 270°.
#define ROT_LEFT 90
#define ROT_RIGHT 90

// Daisy shape controls (safe defaults for 32px wide portrait displays)
#define DAISY_R_CENTER 5
#define DAISY_RX_PETAL 7
#define DAISY_RY_PETAL 9
#define DAISY_OFF_X 8
#define DAISY_OFF_Y 14
#define DAISY_MARGIN 2
// -------------------------------------------------------------------

enum custom_layers {
    _BASE = 0,
    _SYM,
    _NAV,
    _FN,
};

enum custom_keycodes {
    M_1992 = SAFE_RANGE,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT_split_3x6_3(
        KC_ESC,  KC_Q,   KC_W,   KC_E,   KC_R,   KC_T,                         KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC,
        KC_LSFT, KC_A,   KC_S,   KC_D,   KC_F,   KC_G,                         KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
        KC_LCTL, KC_Z,   KC_X,   KC_C,   KC_V,   KC_B,                         KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_TAB,
                                             KC_LGUI, KC_BSPC, LT(_SYM, KC_SPC),  LT(_NAV, KC_ENT), LT(_FN, KC_DEL), KC_RALT
    ),

    [_SYM] = LAYOUT_split_3x6_3(
        S(KC_GRV), S(KC_1), S(KC_2), S(KC_3), S(KC_4), S(KC_5),               KC_7,    KC_8,    KC_9,    KC_SLSH, KC_PAST, KC_BSPC,
        KC_LSPO,   S(KC_0), KC_LBRC, KC_RBRC, S(KC_7), KC_EQL,                KC_4,    KC_5,    KC_6,    KC_PMNS, KC_PPLS, KC_DEL,
        S(KC_LBRC),S(KC_RBRC),S(KC_COMM),S(KC_DOT), S(KC_6), KC_PMNS,         KC_1,    KC_2,    KC_3,    KC_0,    KC_PDOT, KC_RCTL,
                                             KC_LGUI, KC_TRNS, KC_SPC,        KC_ENT,  KC_TRNS, KC_RALT
    ),

    [_NAV] = LAYOUT_split_3x6_3(
        S(KC_GRV), KC_HOME, KC_UP,   KC_END,  S(KC_4),   S(KC_5),             S(KC_6), KC_HOME, KC_END,  S(KC_BSLS), S(KC_SCLN), KC_BSPC,
        KC_LSFT,   KC_LEFT, KC_DOWN, KC_RGHT, S(KC_2),   KC_EQL,              KC_QUOT, KC_LSFT, KC_LCTL, KC_LALT,   KC_LGUI,   KC_GRV,
        KC_LCTL,   KC_PGUP, M_1992,  KC_PGDN, KC_PMNS,   S(KC_MINS),          KC_PCMM, S(KC_SCLN), KC_VOLU, KC_VOLD, S(KC_BSLS), S(KC_GRV),
                                             KC_LGUI, KC_TRNS, KC_SPC,        KC_ENT,  KC_TRNS, KC_BSPC
    ),

    [_FN] = LAYOUT_split_3x6_3(
        KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,                   KC_VOLD, KC_VOLU, KC_MNXT, KC_MPLY, KC_MPRV, KC_BSPC,
        KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,                  KC_PGUP, KC_HOME, KC_UP,   RGB_VAI, RGB_VAD, KC_DEL,
        KC_CAPS, RGB_MOD, KC_CALC, KC_MYCM, KC_FIND, KC_MSTP,                 KC_PGDN, KC_LEFT, KC_DOWN, KC_RGHT, RGB_HUI, KC_ESC,
                                             KC_LGUI, KC_TRNS, KC_TAB,        KC_ENT,  KC_TRNS, KC_BSPC
    ),
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case M_1992:
            if (record->event.pressed) {
                SEND_STRING("1992");
            }
            return false;
    }
    return true;
}

#ifdef RGB_MATRIX_ENABLE
void keyboard_post_init_user(void) {
#if RGB_DIAG_FORCE
    rgb_matrix_enable_noeeprom();
    rgb_matrix_mode_noeeprom(RGB_MATRIX_RAINBOW_MOVING_CHEVRON);
    rgb_matrix_sethsv_noeeprom(128, 255, 160);
#else
    rgb_matrix_enable_noeeprom(); // Let VIA manage effects
#endif
}
#endif

#ifdef OLED_ENABLE

// helper macros
#define SQI(x)    ((int32_t)(x) * (int32_t)(x))
#define MINI(a,b) ((a) < (b) ? (a) : (b))
#define MAXI(a,b) ((a) > (b) ? (a) : (b))

static inline oled_rotation_t rot_from_deg(uint16_t d) {
    switch (d) {
        case 0:   return OLED_ROTATION_0;
        case 90:  return OLED_ROTATION_90;
        case 180: return OLED_ROTATION_180;
        default:  return OLED_ROTATION_270;
    }
}

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return is_keyboard_master() ? rot_from_deg(ROT_LEFT) : rot_from_deg(ROT_RIGHT);
}

static inline void get_rotated_dims(uint8_t *rw, uint8_t *rh) {
    const uint8_t base_w = OLED_DISPLAY_WIDTH;
    const uint8_t base_h = OLED_DISPLAY_HEIGHT;
    const bool rot90or270 = ((ROT_LEFT % 180) != 0);
    *rw = rot90or270 ? base_h : base_w;
    *rh = rot90or270 ? base_w : base_h;
}

static inline bool in_circle(int16_t x, int16_t y, int16_t cx, int16_t cy, int16_t r) {
    int32_t dx = x - cx;
    int32_t dy = y - cy;
    return (SQI(dx) + SQI(dy) <= SQI(r));
}

static inline bool in_ellipse(int16_t x, int16_t y, int16_t cx, int16_t cy, int16_t rx, int16_t ry) {
    int32_t dx = x - cx;
    int32_t dy = y - cy;
    return (SQI(dx) * SQI(ry) + SQI(dy) * SQI(rx) <= SQI(rx) * SQI(ry));
}

static void draw_daisy_overlap(void) {
    uint8_t w, h;
    get_rotated_dims(&w, &h);
    const int16_t cx = w / 2;
    const int16_t cy = h / 2;

    const int16_t rC  = DAISY_R_CENTER;
    const int16_t rxP = DAISY_RX_PETAL;
    const int16_t ryP = DAISY_RY_PETAL;
    const int16_t offx = DAISY_OFF_X;
    const int16_t offy = DAISY_OFF_Y;

    const int16_t x_min = MAXI(0,             cx - (offx + rxP) - DAISY_MARGIN);
    const int16_t x_max = MINI((int16_t)w - 1, cx + (offx + rxP) + DAISY_MARGIN);
    const int16_t y_min = MAXI(0,             cy - (offy + ryP) - DAISY_MARGIN);
    const int16_t y_max = MINI((int16_t)h - 1, cy + (offy + ryP) + DAISY_MARGIN);

    oled_clear();
    for (int16_t y = y_min; y <= y_max; y++) {
        for (int16_t x = x_min; x <= x_max; x++) {
            bool on = false;
            if (in_circle(x, y, cx, cy, rC)) on = true;
            if (!on && in_ellipse(x, y, cx + offx, cy,        rxP, ryP)) on = true;
            if (!on && in_ellipse(x, y, cx - offx, cy,        rxP, ryP)) on = true;
            if (!on && in_ellipse(x, y, cx,        cy + offy, rxP, ryP)) on = true;
            if (!on && in_ellipse(x, y, cx,        cy - offy, rxP, ryP)) on = true;

            if (on) {
                oled_write_pixel((uint8_t)x, (uint8_t)y, true);
            }
        }
    }
}

#if OLED_SHOW_DAISY
bool oled_task_user(void) {
    static bool left_done = false;
    static bool right_done = false;
    if (is_keyboard_master()) {
        if (!left_done) {
            draw_daisy_overlap();
            left_done = true;
        }
    } else {
        if (!right_done) {
            draw_daisy_overlap();
            right_done = true;
        }
    }
    return false;
}
#else
static void draw_rotaware_grid(void) {
    uint8_t w, h;
    get_rotated_dims(&w, &h);
    oled_clear();
    for (uint8_t y = 0; y < h; y++) {
        bool hline = (y == 0) || (y == h - 1) || (y % 8 == 0);
        for (uint8_t x = 0; x < w; x++) {
            bool vline = (x == 0) || (x == w - 1) || (x % 8 == 0);
            if (hline || vline) {
                oled_write_pixel(x, y, true);
            }
        }
    }
}

bool oled_task_user(void) {
    static bool left_done = false;
    static bool right_done = false;
    if (is_keyboard_master()) {
        if (!left_done) {
            draw_rotaware_grid();
            left_done = true;
        }
    } else {
        if (!right_done) {
            draw_rotaware_grid();
            right_done = true;
        }
    }
    return false;
}

void suspend_power_down_user(void) {
#    ifdef RGB_MATRIX_ENABLE
    rgb_matrix_set_suspend_state(true);
#    endif
    oled_off();
}

void suspend_wakeup_init_user(void) {
#    ifdef RGB_MATRIX_ENABLE
    rgb_matrix_set_suspend_state(false);
#    endif
    oled_on();
}
#endif // OLED_SHOW_DAISY

#endif // OLED_ENABLE

