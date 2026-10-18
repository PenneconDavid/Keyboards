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

#ifdef OLED_ENABLE
#    include "oled_graphics.h"
#endif

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
#ifdef OLED_ENABLE
    oled_track_space(keycode, record);
#endif
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

void keyboard_post_init_user(void) {
#ifdef OLED_ENABLE
    transaction_register_rpc(KEEB_PET_STATE, pet_receive);
#endif
#ifdef RGB_MATRIX_ENABLE
#if RGB_DIAG_FORCE
    rgb_matrix_enable_noeeprom();
    rgb_matrix_mode_noeeprom(RGB_MATRIX_RAINBOW_MOVING_CHEVRON);
    rgb_matrix_sethsv_noeeprom(128, 255, 160);
#else
    rgb_matrix_enable_noeeprom(); // Let VIA manage effects
    rgb_matrix_mode_noeeprom(RGB_MATRIX_STARTUP_MODE);
    rgb_matrix_sethsv_noeeprom(RGB_MATRIX_STARTUP_HUE, RGB_MATRIX_STARTUP_SAT, RGB_MATRIX_STARTUP_VAL);
#endif
#endif
}
