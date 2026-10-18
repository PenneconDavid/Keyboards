/*
 * keebv2: wired Keebmaker Corne (crkbd rev1, Pro Micro / ATmega32U4, two 128x32 OLEDs).
 *
 * Mirrors the wireless Eyelash Corne ZMK keymap v4.1 (ZmkConfig main, config/eyelash_corne.keymap,
 * signed off and hardware-tested 2026-10-17). The wired board has no encoder and no 5-way stick,
 * so the 48 ZMK positions map onto 42 keys: the six stick/encoder bindings (volume knob, mute,
 * arrows + Enter on Base, mouse movement + left click elsewhere) have no home here and are dropped.
 * Everything else is 1:1.
 *
 * Layers
 *   0 _BASE       QWERTY. Thumbs: GUI | Fn/Bksp | Symbols/Space || Del | Enter | RAlt
 *   1 _SYM        hold Space. Symbols left, numpad right, Tab / Enter / Shift+Tab on the right thumbs.
 *   2 _GAME_WASD  gaming left half (Tab on T, M on R, thumbs R H Space) with Alt on the G key;
 *                 right half = slots 1-0, Esc, exit, Enter.
 *   3 _FN         hold Backspace. F-keys, nav, lighting/media, Excel helpers, game selectors.
 *   4 _ARC        Arc Raiders overlay on _GAME_WASD: G back on the G key.
 *   5 _GAME_ESDF  hand on ESDF (keys send WASD); Esc/Shift/Ctrl on the pinky column;
 *                 far-left Tab, M, Alt; thumbs R H Space.
 *   6 _ARC_E      Arc Raiders overlay on _GAME_ESDF: empty today, exists so a game can add keys.
 *
 * Enter a game: hold Backspace (Fn) and tap ARC / WASD / ESDF / ARC-E on the right bottom row.
 * Leave a game: right inner thumb (TO(0)), or the left Esc + the right Esc (top-right corner)
 * together (combo, 50 ms window, 150 ms prior idle, game layers only).
 *
 * VIA stays enabled (7 dynamic layers). VIA overrides this file after the first flash, so export a
 * dated JSON into layouts/via/ after flashing and after every live edit.
 */
#include QMK_KEYBOARD_H

enum custom_layers {
    _BASE = 0,
    _SYM,
    _GAME_WASD,
    _FN,
    _ARC,
    _GAME_ESDF,
    _ARC_E,
};

// VIA shows these as USER00 / USER01.
enum custom_keycodes {
    ENTER_ARC = QK_USER_0, // TO(_GAME_WASD) then _ARC on top  (ZMK: &to 2, &tog 4)
    ENTER_ARC_E,           // TO(_GAME_ESDF) then _ARC_E on top (ZMK: &to 5, &tog 6)
};

// Tap once = Shift (hold works), tap twice = Caps Lock. VIA shows it as TD(0).
enum tap_dances {
    TD_SFT_CAPS,
};

tap_dance_action_t tap_dance_actions[] = {
    [TD_SFT_CAPS] = ACTION_TAP_DANCE_DOUBLE(KC_LSFT, KC_CAPS),
};

#define SFT_CAP TD(TD_SFT_CAPS)

// Right half in every game base: slots 1-5 on the top row, 6-0 on the home row, Esc in the
// top-right corner (also half of the exit combo), bottom row inert. Right thumbs: exit (inner),
// Enter (middle), inert (outer). Left thumbs: R (reload), H (put item away), Space.
// (Written out per layer: the LAYOUT macro cannot take comma-containing macros as arguments.)
// Game layers use plain keycodes only: no layer-taps, no GUI, no way to Fn/Symbols inside a game.
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT_split_3x6_3(
        KC_ESC,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                      KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC,
        SFT_CAP, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                      KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
        KC_LCTL, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                      KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_TAB,
                                   KC_LGUI, LT(_FN, KC_BSPC), LT(_SYM, KC_SPC),    KC_DEL,  KC_ENT,  KC_RALT
    ),

    [_SYM] = LAYOUT_split_3x6_3(
        KC_ESC,  KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC,                   KC_7,    KC_8,    KC_9,    KC_SLSH, KC_ASTR, KC_BSPC,
        KC_GRV,  KC_LBRC, KC_RBRC, KC_LPRN, KC_RPRN, KC_BSLS,                   KC_4,    KC_5,    KC_6,    KC_MINS, KC_PLUS, KC_EQL,
        KC_TILD, KC_LCBR, KC_RCBR, KC_NO,   KC_NO,   KC_PIPE,                   KC_1,    KC_2,    KC_3,    KC_0,    KC_DOT,  KC_UNDS,
                                            KC_TRNS, KC_TRNS, KC_TRNS,          KC_TAB,  KC_ENT,  S(KC_TAB)
    ),

    [_GAME_WASD] = LAYOUT_split_3x6_3(
        KC_ESC,  KC_Q,    KC_W,    KC_E,    KC_M,    KC_TAB,                    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_ESC,
        KC_LSFT, KC_A,    KC_S,    KC_D,    KC_F,    KC_LALT,                   KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_NO,
        KC_LCTL, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                      KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
                                            KC_R,    KC_H,    KC_SPC,           TO(_BASE), KC_ENT, KC_NO
    ),

    [_FN] = LAYOUT_split_3x6_3(
        KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,                     KC_HOME, KC_PGDN, KC_PGUP, KC_END,  KC_PSCR, KC_BSPC,
        KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,                    KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_DEL,  KC_CAPS,
        RM_TOGG, RM_NEXT, KC_NO,   KC_MUTE, KC_VOLD, KC_VOLU,                   KC_INS,  A(KC_EQL), ENTER_ARC, TO(_GAME_WASD), TO(_GAME_ESDF), ENTER_ARC_E,
                                            KC_TRNS, KC_TRNS, KC_TRNS,          KC_TAB,  KC_TRNS, S(KC_TAB)
    ),

    // Overlay on _GAME_WASD: G (emote) back on the G key in place of Alt. With it the left half is
    // the original gaming layer key for key: Esc Q W E M Tab / Shift A S D F G / Ctrl Z X C V B.
    [_ARC] = LAYOUT_split_3x6_3(
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_G,                      KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                                            KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS, KC_TRNS, KC_TRNS
    ),

    // Hand on ESDF; the keys SEND vanilla WASD so no game needs rebinding. Esc, Shift and Ctrl sit
    // on the pinky column (physical Q/A/Z). Far-left column: Tab (inventory), M (map), Alt (dodge).
    [_GAME_ESDF] = LAYOUT_split_3x6_3(
        KC_TAB,  KC_ESC,  KC_Q,    KC_W,    KC_E,    KC_R,                      KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_ESC,
        KC_M,    KC_LSFT, KC_A,    KC_S,    KC_D,    KC_F,                      KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_NO,
        KC_LALT, KC_LCTL, KC_X,    KC_C,    KC_V,    KC_B,                      KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
                                            KC_R,    KC_H,    KC_SPC,           TO(_BASE), KC_ENT, KC_NO
    ),

    // Overlay on _GAME_ESDF: nothing differs today. Kept so Arc-only keys can be added later.
    [_ARC_E] = LAYOUT_split_3x6_3(
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                                            KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS, KC_TRNS, KC_TRNS
    ),
};

// ----------------------------------------------------------------------------------------------
// Game exit combo: left Esc + right Esc (top-right corner) = TO(_BASE).
// COMBO_ONLY_FROM_LAYER 0 (config.h) identifies the keys by their Base keycodes, so the two Esc
// keys are distinct: the left corner is KC_ESC on the WASD layers and KC_Q on the ESDF layers,
// the right corner is KC_BSPC. combo_should_trigger() limits each combo to its game layers, so
// Esc, Q and Backspace are not delayed anywhere else. ZMK's require-prior-idle-ms (150) is
// reproduced with last_press: a combo key pressed within 150 ms of any other key is just a key.
// ----------------------------------------------------------------------------------------------
enum combos {
    CB_EXIT_WASD,
    CB_EXIT_ESDF,
};

const uint16_t PROGMEM combo_exit_wasd[] = {KC_ESC, KC_BSPC, COMBO_END};
const uint16_t PROGMEM combo_exit_esdf[] = {KC_Q,   KC_BSPC, COMBO_END};

combo_t key_combos[] = {
    [CB_EXIT_WASD] = COMBO(combo_exit_wasd, TO(_BASE)),
    [CB_EXIT_ESDF] = COMBO(combo_exit_esdf, TO(_BASE)),
};

#define COMBO_PRIOR_IDLE_MS 150
static uint16_t last_press = 0;

bool combo_should_trigger(uint16_t combo_index, combo_t *combo, uint16_t keycode, keyrecord_t *record) {
    if (timer_elapsed(last_press) < COMBO_PRIOR_IDLE_MS) {
        return false;
    }
    switch (combo_index) {
        case CB_EXIT_WASD:
            return layer_state_is(_GAME_WASD);
        case CB_EXIT_ESDF:
            return layer_state_is(_GAME_ESDF);
    }
    return false;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        last_press = timer_read();
    }
    switch (keycode) {
        case ENTER_ARC:
            if (record->event.pressed) {
                layer_move(_GAME_WASD); // drops every other layer, including the Fn layer this key lives on
                layer_on(_ARC);
            }
            return false;
        case ENTER_ARC_E:
            if (record->event.pressed) {
                layer_move(_GAME_ESDF);
                layer_on(_ARC_E);
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

// Final rotation of this half's OLED, set once in oled_init_user() and used for the canvas size.
static oled_rotation_t side_rotation = OLED_ROTATION_0;

// `rotation` arrives from crkbd.c's oled_init_kb(): 0 on the master, 180 on the slave, because the
// right half's OLED is mounted upside-down relative to the left. The old code subtracted that 180
// again, which cancelled it, so both halves ended up logically identical and physically opposite
// (the "right OLED upside-down" issue). Add the per-side portrait rotation on top instead.
// Both halves then read the same way. If BOTH are upside-down, set ROT_LEFT and ROT_RIGHT to 90;
// if only one is, flip that side's OLED_*_FLIP in config.h.
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    const uint16_t target_deg = is_keyboard_master() ? ROT_LEFT : ROT_RIGHT;
    const uint16_t final_deg = (uint16_t)((target_deg + deg_from_rot(rotation)) % 360u);
    side_rotation = rot_from_deg(final_deg);
    return side_rotation;
}

static inline void get_rotated_dims_for_side(bool is_master, uint8_t *rw, uint8_t *rh) {
    (void)is_master; // dims follow the rotation actually applied to this half
    const uint8_t base_w = OLED_DISPLAY_WIDTH;
    const uint8_t base_h = OLED_DISPLAY_HEIGHT;
    const bool rot90or270 = ((deg_from_rot(side_rotation) % 180) != 0);
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

// Game layers: the old NAV arrow, kept as the "in a game" marker.
static void draw_layer_icon_game(uint8_t w, uint8_t h, int16_t top, int16_t bottom, uint8_t phase, uint8_t amplitude, bool flip) {
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
        case _GAME_WASD:
        case _ARC:
        case _GAME_ESDF:
        case _ARC_E:
            draw_layer_icon_game(w, h, icon_top, icon_bottom, phase, amplitude, flip);
            break;
        case _FN:
            draw_layer_icon_fn(w, h, icon_top, icon_bottom, phase, amplitude, flip);
            break;
        default:
            draw_layer_icon_base(w, h, icon_top, icon_bottom, phase, amplitude, flip);
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

// Left (master): animated icon of the active layer. Right (slave): the animated DAVID logo.
bool oled_task_user(void) {
    static uint32_t last_frame   = 0;
    static uint8_t  phase        = 0;
    static uint8_t  cached_layer = 0xFF; // 0xFF = first frame not drawn yet
    const uint32_t  now          = timer_read32();
    bool            redraw       = false;

    if (is_keyboard_master()) {
        const uint8_t active_layer = get_highest_layer(layer_state | default_layer_state);
        if (active_layer != cached_layer) {
            cached_layer = active_layer;
            phase        = 0;
            redraw       = true;
        }
        if ((uint32_t)(now - last_frame) >= 120) {
            last_frame = now;
            phase      = (phase + 1) % LAYER_PULSE_STEPS;
            redraw     = true;
        }
        if (redraw) {
            draw_layer_pulse_frame(true, active_layer, phase, OLED_LEFT_FLIP);
        }
    } else {
        if (cached_layer == 0xFF) {
            cached_layer = 0;
            redraw       = true;
        }
        if ((uint32_t)(now - last_frame) >= 140) {
            last_frame = now;
            phase      = (phase + 1) % LAYER_PULSE_STEPS;
            redraw     = true;
        }
        if (redraw) {
            draw_david_logo_frame(false, OLED_RIGHT_FLIP, phase, wave_amplitude(phase, LAYER_PULSE_STEPS));
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

