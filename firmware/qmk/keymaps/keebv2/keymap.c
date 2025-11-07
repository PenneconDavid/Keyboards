#include QMK_KEYBOARD_H

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
        SC_LSPO,   S(KC_0), KC_LBRC, KC_RBRC, S(KC_7), KC_EQL,                KC_4,    KC_5,    KC_6,    KC_PMNS, KC_PPLS, KC_DEL,
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
    rgb_matrix_mode_noeeprom(RGB_MATRIX_STARTUP_MODE);
    rgb_matrix_sethsv_noeeprom(RGB_MATRIX_STARTUP_HUE, RGB_MATRIX_STARTUP_SAT, RGB_MATRIX_STARTUP_VAL);
#endif
}
#endif

#ifdef OLED_ENABLE

// helper macros
#define SQI(x)    ((int32_t)(x) * (int32_t)(x))
#define MINI(a,b) ((a) < (b) ? (a) : (b))
#define MAXI(a,b) ((a) > (b) ? (a) : (b))

#define LAYER_PULSE_STEPS 24

static inline uint8_t wave_amplitude(uint8_t phase, uint8_t steps) {
    uint8_t idx = phase % steps;
    uint8_t half = steps / 2;
    return (idx <= half) ? idx : (steps - idx);
}

static inline oled_rotation_t rot_from_deg(uint16_t d) {
    switch (d) {
        case 0:   return OLED_ROTATION_0;
        case 90:  return OLED_ROTATION_90;
        case 180: return OLED_ROTATION_180;
        default:  return OLED_ROTATION_270;
    }
}

static inline uint16_t deg_from_rot(oled_rotation_t r) {
    switch (r) {
        case OLED_ROTATION_0:   return 0;
        case OLED_ROTATION_90:  return 90;
        case OLED_ROTATION_180: return 180;
        default:                return 270;
    }
}

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    const bool is_master = is_keyboard_master();
    const uint16_t target_deg = is_master ? ROT_LEFT : ROT_RIGHT;
    const uint16_t base_deg = deg_from_rot(rotation);
    const uint16_t return_deg = (uint16_t)((target_deg + 360u - base_deg) % 360u);
    return rot_from_deg(return_deg);
}

static inline void get_rotated_dims_for_side(bool is_master, uint8_t *rw, uint8_t *rh) {
    const uint8_t base_w = OLED_DISPLAY_WIDTH;
    const uint8_t base_h = OLED_DISPLAY_HEIGHT;
    const uint16_t deg = is_master ? ROT_LEFT : ROT_RIGHT;
    const bool rot90or270 = ((deg % 180) != 0);
    *rw = rot90or270 ? base_h : base_w;
    *rh = rot90or270 ? base_w : base_h;
}

static inline void write_pixel_safe(uint8_t w, uint8_t h, int16_t x, int16_t y, bool on, bool flip) {
    if (flip) {
        x = (int16_t)(w - 1 - x);
        y = (int16_t)(h - 1 - y);
    }
    if (x >= 0 && x < w && y >= 0 && y < h) {
        oled_write_pixel((uint8_t)x, (uint8_t)y, on);
    }
}

static void draw_rect_outline(uint8_t w, uint8_t h, int16_t left, int16_t top, int16_t right, int16_t bottom, uint8_t thickness, bool flip) {
    for (uint8_t t = 0; t < thickness; t++) {
        for (int16_t x = left + t; x <= right - t; x++) {
            write_pixel_safe(w, h, x, top + t, true, flip);
            write_pixel_safe(w, h, x, bottom - t, true, flip);
        }
        for (int16_t y = top + t; y <= bottom - t; y++) {
            write_pixel_safe(w, h, left + t, y, true, flip);
            write_pixel_safe(w, h, right - t, y, true, flip);
        }
    }
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

static void draw_daisy_overlap(bool is_master, bool flip) {
    uint8_t w, h;
    get_rotated_dims_for_side(is_master, &w, &h);
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
                write_pixel_safe(w, h, x, y, true, flip);
            }
        }
    }
}

static void draw_rotaware_grid(bool is_master, bool flip) {
    uint8_t w, h;
    get_rotated_dims_for_side(is_master, &w, &h);
    oled_clear();
    for (uint8_t y = 0; y < h; y++) {
        bool hline = (y == 0) || (y == h - 1) || (y % 8 == 0);
        for (uint8_t x = 0; x < w; x++) {
            bool vline = (x == 0) || (x == w - 1) || (x % 8 == 0);
            if (hline || vline) {
                write_pixel_safe(w, h, x, y, true, flip);
            }
        }
    }
}

static void draw_qmk_logo(bool is_master, bool flip) {
    uint8_t w, h;
    get_rotated_dims_for_side(is_master, &w, &h);
    oled_clear();

    draw_rect_outline(w, h, 2, 6, w - 3, h - 7, 1, flip);

    int16_t q_left = 5;
    int16_t q_right = w - 6;
    int16_t q_top = 10;
    int16_t q_bottom = q_top + 24;
    draw_rect_outline(w, h, q_left, q_top, q_right, q_bottom, 2, flip);
    for (uint8_t t = 0; t < 4; t++) {
        write_pixel_safe(w, h, q_right - 3 + t, q_bottom - 2 + t, true, flip);
        if (t < 3) {
            write_pixel_safe(w, h, q_right - 4 + t, q_bottom - 3 + t, true, flip);
        }
    }

    int16_t m_top = q_bottom + 8;
    int16_t m_bottom = m_top + 28;
    int16_t m_left = q_left;
    int16_t m_right = q_right;
    for (int16_t y = m_top; y <= m_bottom; y++) {
        for (uint8_t t = 0; t < 2; t++) {
            write_pixel_safe(w, h, m_left + t, y, true, flip);
            write_pixel_safe(w, h, m_right - t, y, true, flip);
        }
    }
    int16_t diag_height = (m_bottom - m_top) / 2;
    for (int16_t y = 0; y <= diag_height; y++) {
        int16_t y_abs = m_top + y;
        int16_t offset = y / 2;
        for (uint8_t t = 0; t < 2; t++) {
            write_pixel_safe(w, h, m_left + offset + t, y_abs, true, flip);
            write_pixel_safe(w, h, m_right - offset - t, y_abs, true, flip);
        }
    }

    int16_t k_top = m_bottom + 8;
    int16_t k_bottom = h - 12;
    int16_t k_left = q_left;
    for (int16_t y = k_top; y <= k_bottom; y++) {
        for (uint8_t t = 0; t < 2; t++) {
            write_pixel_safe(w, h, k_left + t, y, true, flip);
        }
    }
    int16_t center = (k_top + k_bottom) / 2;
    for (int16_t y = 0; y <= (center - k_top); y++) {
        int16_t y_abs = center - y;
        int16_t x = k_left + 5 + y / 2;
        for (uint8_t t = 0; t < 2; t++) {
            write_pixel_safe(w, h, x + t, y_abs, true, flip);
        }
    }
    for (int16_t y = 0; y <= (k_bottom - center); y++) {
        int16_t y_abs = center + y;
        int16_t x = k_left + 5 + y / 2;
        for (uint8_t t = 0; t < 2; t++) {
            write_pixel_safe(w, h, x + t, y_abs, true, flip);
        write_pixel_safe(w, h, x + t, y_abs, true, flip);
        }
    }

    for (int16_t x = q_left; x <= q_right; x += 4) {
        write_pixel_safe(w, h, x, h - 10, true, flip);
        write_pixel_safe(w, h, x + 1, h - 11, true, flip);
    }
}

static inline void fill_rect(uint8_t w, uint8_t h, int16_t left, int16_t top, int16_t right, int16_t bottom, bool flip) {
    if (left > right) {
        int16_t tmp = left; left = right; right = tmp;
    }
    if (top > bottom) {
        int16_t tmp = top; top = bottom; bottom = tmp;
    }
    for (int16_t y = top; y <= bottom; y++) {
        for (int16_t x = left; x <= right; x++) {
            write_pixel_safe(w, h, x, y, true, flip);
        }
    }
}

static const int8_t symbol_orbit_offsets[8][2] = {
    {0, -16}, {5, -12}, {8, -6}, {9, 0}, {6, 7}, {0, 11}, {-6, 7}, {-9, 0}
};

static void draw_layer_icon_base(uint8_t w, uint8_t h, int16_t top, int16_t bottom, uint8_t phase, uint8_t amplitude, bool flip) {
    const uint8_t max_amp = LAYER_PULSE_STEPS / 2;
    const bool emphasize = amplitude > (max_amp / 2);
    const int16_t left = 4;
    const int16_t right = w - 5;

    const uint8_t frame_thickness = (uint8_t)(1 + amplitude / 6);
    draw_rect_outline(w, h, left, top, right, bottom, frame_thickness, flip);

    const int16_t key_width = 3 + amplitude / 8;
    const int16_t key_height = 4 + amplitude / 6;
    const int16_t key_gap_x = 2;
    const int16_t key_gap_y = 3;
    const int16_t layout_height = (key_height * 3) + (key_gap_y * 2);
    int16_t start = top + ((bottom - top - layout_height) / 2);
    if (start < top + 2) {
        start = top + 2;
    }

    for (uint8_t row = 0; row < 3; row++) {
        int16_t row_top = start + (int16_t)row * (key_height + key_gap_y);
        for (uint8_t col = 0; col < 4; col++) {
            int16_t col_left = left + 3 + (int16_t)col * (key_width + key_gap_x);
            fill_rect(w, h, col_left, row_top, col_left + key_width - 1, row_top + key_height, flip);
        }
        if (row == (uint8_t)((phase / (LAYER_PULSE_STEPS / 3)) % 3)) {
            int16_t glow_top = row_top - 1;
            int16_t glow_bottom = row_top + key_height + 1;
            if (glow_top < top + 1) glow_top = top + 1;
            if (glow_bottom > bottom - 1) glow_bottom = bottom - 1;
            fill_rect(w, h, left + 2, glow_top, right - 2, glow_bottom, flip);
        }
    }

    uint8_t sweep_phase = phase % LAYER_PULSE_STEPS;
    int16_t sweep_x = left + 2 + (int16_t)((int32_t)sweep_phase * (right - left - 4) / LAYER_PULSE_STEPS);
    for (int16_t y = top + 2; y <= bottom - 2; y++) {
        write_pixel_safe(w, h, sweep_x, y, true, flip);
        if (emphasize) {
            write_pixel_safe(w, h, sweep_x + 1, y, true, flip);
        }
    }

    if (emphasize) {
        fill_rect(w, h, left + 1, bottom - 7, right - 1, bottom - 4, flip);
    }
}

static void draw_layer_icon_symbols(uint8_t w, uint8_t h, int16_t top, int16_t bottom, uint8_t phase, uint8_t amplitude, bool flip) {
    const uint8_t max_amp = LAYER_PULSE_STEPS / 2;
    const bool emphasize = amplitude > (max_amp / 2);
    const int16_t cx = w / 2;
    const int16_t cy = (top + bottom) / 2;
    const int16_t arm_len = (bottom - top) / 2 - 5 + amplitude / 2;
    const int16_t bar_width = 1 + amplitude / 10;

    fill_rect(w, h, cx - bar_width, cy - arm_len, cx + bar_width, cy + arm_len, flip);
    fill_rect(w, h, cx - arm_len, cy - bar_width, cx + arm_len, cy + bar_width, flip);

    if (emphasize) {
        for (int16_t offset = 0; offset <= arm_len / 2; offset++) {
            write_pixel_safe(w, h, cx + offset, cy + offset / 2, true, flip);
            write_pixel_safe(w, h, cx - offset, cy + offset / 2, true, flip);
            write_pixel_safe(w, h, cx + offset, cy - offset / 2, true, flip);
            write_pixel_safe(w, h, cx - offset, cy - offset / 2, true, flip);
        }
    }

    uint8_t orbit_index = (phase / 3) % 8;
    int16_t px = cx + symbol_orbit_offsets[orbit_index][0];
    int16_t py = cy + symbol_orbit_offsets[orbit_index][1];
    fill_rect(w, h, px - 1, py - 1, px + 1, py + 1, flip);
}

static void draw_layer_icon_nav(uint8_t w, uint8_t h, int16_t top, int16_t bottom, uint8_t phase, uint8_t amplitude, bool flip) {
    const uint8_t max_amp = LAYER_PULSE_STEPS / 2;
    const int16_t cx = w / 2;
    int16_t bob = (int16_t)((int32_t)(phase % LAYER_PULSE_STEPS) * 6 / LAYER_PULSE_STEPS) - 3;
    int16_t head_top = top + 6 + bob;
    int16_t shaft_top = head_top + 14;
    int16_t shaft_bottom = bottom - 14 + bob;
    int16_t shaft_half = 1 + amplitude / 5;

    for (int16_t i = 0; i < 14 + amplitude; i++) {
        int16_t width = shaft_half + 1 + (14 + amplitude - i);
        fill_rect(w, h, cx - width, head_top + i, cx + width, head_top + i, flip);
    }

    fill_rect(w, h, cx - shaft_half, shaft_top, cx + shaft_half, shaft_bottom, flip);

    int16_t trail_base = shaft_bottom + 3;
    for (uint8_t i = 0; i < 8; i++) {
        int16_t trail_y = trail_base + (int16_t)i * 3;
        int16_t wiggle = ((int16_t)((phase + i * 2) % LAYER_PULSE_STEPS) - (LAYER_PULSE_STEPS / 2)) / 6;
        int16_t trail_x = cx + wiggle;
        write_pixel_safe(w, h, trail_x, trail_y, true, flip);
        if (amplitude > (max_amp / 2)) {
            write_pixel_safe(w, h, trail_x + 1, trail_y, true, flip);
        }
    }
}

static void draw_layer_icon_fn(uint8_t w, uint8_t h, int16_t top, int16_t bottom, uint8_t phase, uint8_t amplitude, bool flip) {
    const uint8_t max_amp = LAYER_PULSE_STEPS / 2;
    const int16_t left = 6;
    const int16_t right = w - 6;
    const uint8_t stroke = 3 + amplitude / 6;

    fill_rect(w, h, left, top, left + stroke - 1, bottom, flip);
    fill_rect(w, h, left + stroke, top, right, top + stroke - 1, flip);
    fill_rect(w, h, left + stroke, (top + bottom) / 2, right - 2, (top + bottom) / 2 + stroke - 1, flip);

    uint8_t sweep_phase = phase % LAYER_PULSE_STEPS;
    int16_t sweep_height = 2 + amplitude / 6;
    int16_t sweep_y = top + 4 + (int16_t)((int32_t)sweep_phase * (bottom - top - 8 - sweep_height) / LAYER_PULSE_STEPS);
    fill_rect(w, h, left + stroke + 1, sweep_y, right - 1, sweep_y + sweep_height, flip);

    if (amplitude > (max_amp / 2)) {
        fill_rect(w, h, right - 4, top + stroke + 2, right - 1, (top + bottom) / 2 - 1, flip);
    }
}

static void draw_layer_pulse_frame(bool is_master, uint8_t layer, uint8_t phase, bool flip) {
    uint8_t w, h;
    get_rotated_dims_for_side(is_master, &w, &h);
    oled_clear();

    const int16_t icon_top = 16;
    const int16_t icon_bottom = h - 20;
    uint8_t amplitude = wave_amplitude(phase, LAYER_PULSE_STEPS);

    switch (layer) {
        case _BASE:
            draw_layer_icon_base(w, h, icon_top, icon_bottom, phase, amplitude, flip);
            break;
        case _SYM:
            draw_layer_icon_symbols(w, h, icon_top, icon_bottom, phase, amplitude, flip);
            break;
        case _NAV:
            draw_layer_icon_nav(w, h, icon_top, icon_bottom, phase, amplitude, flip);
            break;
        case _FN:
            draw_layer_icon_fn(w, h, icon_top, icon_bottom, phase, amplitude, flip);
            break;
        default:
            draw_rotaware_grid(is_master, flip);
            break;
    }
}

static void draw_letter_D(uint8_t w, uint8_t h, int16_t left, int16_t top, int16_t width, int16_t height, uint8_t stroke, bool flip) {
    int16_t right = left + width;
    int16_t bottom = top + height;
    fill_rect(w, h, left, top, left + stroke - 1, bottom, flip);
    fill_rect(w, h, left + stroke, top, right - stroke, top + stroke - 1, flip);
    fill_rect(w, h, left + stroke, bottom - stroke + 1, right - stroke, bottom, flip);
    fill_rect(w, h, right - stroke, top + stroke, right, bottom - stroke, flip);
}

static void draw_letter_A(uint8_t w, uint8_t h, int16_t left, int16_t top, int16_t width, int16_t height, uint8_t stroke, bool flip) {
    int16_t right = left + width;
    for (int16_t y = 0; y <= height; y++) {
        int16_t offset = (int16_t)((int32_t)y * (width / 2)) / (height ? height : 1);
        fill_rect(w, h, left + offset, top + y, left + offset + stroke - 1, top + y + stroke - 1, flip);
        fill_rect(w, h, right - offset - stroke + 1, top + y, right - offset, top + y + stroke - 1, flip);
    }
    fill_rect(w, h, left + stroke, top + height / 2, right - stroke, top + height / 2 + stroke - 1, flip);
}

static void draw_letter_V(uint8_t w, uint8_t h, int16_t left, int16_t top, int16_t width, int16_t height, uint8_t stroke, bool flip) {
    int16_t right = left + width;
    for (int16_t y = 0; y <= height; y++) {
        int16_t offset = (int16_t)((int32_t)y * (width / 2)) / (height ? height : 1);
        fill_rect(w, h, left + offset, top + y, left + offset + stroke - 1, top + y + stroke - 1, flip);
        fill_rect(w, h, right - offset - stroke + 1, top + y, right - offset, top + y + stroke - 1, flip);
    }
}

static void draw_letter_I(uint8_t w, uint8_t h, int16_t left, int16_t top, int16_t width, int16_t height, uint8_t stroke, bool flip) {
    int16_t right = left + width;
    int16_t bottom = top + height;
    int16_t center_left = left + (width / 2) - (stroke / 2);
    int16_t center_right = center_left + stroke - 1;
    fill_rect(w, h, left, top, right, top + stroke - 1, flip);
    fill_rect(w, h, center_left, top, center_right, bottom, flip);
    fill_rect(w, h, left, bottom - stroke + 1, right, bottom, flip);
}

static void draw_david_logo_frame(bool is_master, bool flip, uint8_t phase, uint8_t amplitude) {
    uint8_t w, h;
    get_rotated_dims_for_side(is_master, &w, &h);
    oled_clear();

    const uint8_t max_amp = LAYER_PULSE_STEPS / 2;
    int16_t margin = 4 - (int16_t)(amplitude / 6);
    if (margin < 1) {
        margin = 1;
    }

    uint8_t stroke = 2 + amplitude / 4;
    if (stroke > 5) {
        stroke = 5;
    }

    int16_t letter_height = 18 + amplitude / 3;
    if (letter_height > 24) {
        letter_height = 24;
    }

    int16_t spacing = 4 + amplitude / 6;
    if (spacing > 6) {
        spacing = 6;
    }

    int16_t left = margin + 1;
    int16_t width = w - (margin * 2) - 2;
    if (width < 10) {
        width = w - 6;
    }

    int16_t top = 6 + (int16_t)((max_amp - amplitude) / 2);
    int16_t total_height = (letter_height * 5) + (spacing * 4);
    if (top + total_height > h - 8) {
        top = (h - 8) - total_height;
    }
    if (top < 4) {
        top = 4;
    }

    bool aura = amplitude > (max_amp * 3) / 4;
    if (aura) {
        int16_t aura_left = MAXI(0, margin - 1);
        int16_t aura_right = MINI((int16_t)w - 1, w - margin);
        draw_rect_outline(w, h, aura_left, 4, aura_right, h - 6, 1, flip);
        for (uint8_t i = 0; i < 4; i++) {
            if (((phase / 2) % 4) == i) {
                int16_t halo_x = aura_left + 2 + (int16_t)i * (aura_right - aura_left - 4) / 3;
                fill_rect(w, h, halo_x, 6, halo_x + 1, 10, flip);
                fill_rect(w, h, aura_right - (halo_x - aura_left), h - 11, aura_right - (halo_x - aura_left) + 1, h - 7, flip);
            }
        }
    }

    int16_t shimmer_y = top - 3 + (int16_t)((int32_t)(phase % LAYER_PULSE_STEPS) * (letter_height + spacing) / LAYER_PULSE_STEPS);
    if (shimmer_y >= 4 && shimmer_y <= h - 6) {
        int16_t shimmer_left = MAXI(0, left - 1);
        int16_t shimmer_right = MINI((int16_t)w - 1, left + width + 1);
        fill_rect(w, h, shimmer_left, shimmer_y, shimmer_right, shimmer_y, flip);
    }

    int16_t cursor_y = top + total_height + 4 - (int16_t)((int32_t)(phase % LAYER_PULSE_STEPS) * (spacing + 2) / LAYER_PULSE_STEPS);
    if (cursor_y < top + total_height + 6 && cursor_y > top + total_height - 18) {
        int16_t accent_left = MINI((int16_t)w - 3, left + width + 1);
        fill_rect(w, h, accent_left, cursor_y, accent_left + 2, cursor_y + 2, flip);
    }

    int16_t draw_top = top;
    draw_letter_D(w, h, left, draw_top, width, letter_height, stroke, flip);
    draw_top += letter_height + spacing;
    draw_letter_A(w, h, left, draw_top, width, letter_height, stroke, flip);
    draw_top += letter_height + spacing;
    draw_letter_V(w, h, left, draw_top, width, letter_height, stroke, flip);
    draw_top += letter_height + spacing;
    draw_letter_I(w, h, left, draw_top, width, letter_height, stroke, flip);
    draw_top += letter_height + spacing;
    draw_letter_D(w, h, left, draw_top, width, letter_height, stroke, flip);

    if (amplitude > (max_amp / 2)) {
        int16_t band_left = MAXI(0, left - 1);
        int16_t band_right = MINI((int16_t)w - 1, left + width + 1);
        fill_rect(w, h, band_left, top - 2, band_right, top - 1, flip);
        fill_rect(w, h, band_left, draw_top + letter_height + 1, band_right, draw_top + letter_height + 2, flip);
    }
}

static void draw_static_mode(bool is_master, uint8_t mode, bool flip) {
    switch (mode) {
        case OLED_MODE_DAISY:
            draw_daisy_overlap(is_master, flip);
            break;
        case OLED_MODE_QMK:
            draw_qmk_logo(is_master, flip);
            break;
        case OLED_MODE_DAVID:
            draw_david_logo_frame(is_master, flip, 0, LAYER_PULSE_STEPS / 4);
            break;
        default:
            draw_rotaware_grid(is_master, flip);
            break;
    }
}

bool oled_task_user(void) {
    const bool is_master = is_keyboard_master();
    const bool flip = is_master ? OLED_LEFT_FLIP : OLED_RIGHT_FLIP;
    static uint32_t left_last_frame = 0;
    static uint8_t left_phase = 0;
    static uint8_t left_cached_layer = 0;
    static bool left_initialized = false;
    static uint8_t left_last_static_mode = 0xFF;
    static uint32_t right_last_frame = 0;
    static uint8_t right_phase = 0;
    static bool right_initialized = false;
    static uint8_t right_last_static_mode = 0xFF;

    if (is_master) {
        const uint8_t mode = OLED_LEFT_MODE;
        if (mode == OLED_MODE_QMK) {
            layer_state_t current_state = layer_state | default_layer_state;
            uint8_t active_layer = get_highest_layer(current_state);
            uint32_t now = timer_read32();
            bool needs_redraw = false;

            if (!left_initialized) {
                left_initialized = true;
                left_cached_layer = active_layer;
                left_last_frame = now;
                left_phase = 0;
                needs_redraw = true;
            }

            if (active_layer != left_cached_layer) {
                left_cached_layer = active_layer;
                left_phase = 0;
                needs_redraw = true;
            }

            if ((uint32_t)(now - left_last_frame) >= 120) {
                left_last_frame = now;
                left_phase = (left_phase + 1) % LAYER_PULSE_STEPS;
                needs_redraw = true;
            }

            if (needs_redraw) {
                draw_layer_pulse_frame(true, active_layer, left_phase, flip);
            }

            left_last_static_mode = 0xFF;
        } else {
            left_initialized = false;
            if (left_last_static_mode != mode) {
                draw_static_mode(true, mode, flip);
                left_last_static_mode = mode;
            }
        }
    } else {
        const uint8_t mode = OLED_RIGHT_MODE;
        if (mode == OLED_MODE_DAVID) {
            uint32_t now = timer_read32();
            bool needs_redraw = false;

            if (!right_initialized) {
                right_initialized = true;
                right_phase = 0;
                right_last_frame = now;
                needs_redraw = true;
            }

            if ((uint32_t)(now - right_last_frame) >= 140) {
                right_last_frame = now;
                right_phase = (right_phase + 1) % LAYER_PULSE_STEPS;
                needs_redraw = true;
            }

            if (needs_redraw) {
                uint8_t amplitude = wave_amplitude(right_phase, LAYER_PULSE_STEPS);
                draw_david_logo_frame(false, flip, right_phase, amplitude);
            }

            right_last_static_mode = 0xFF;
        } else {
            right_initialized = false;
            if (right_last_static_mode != mode) {
                draw_static_mode(false, mode, flip);
                right_last_static_mode = mode;
            }
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

#endif // OLED_ENABLE

