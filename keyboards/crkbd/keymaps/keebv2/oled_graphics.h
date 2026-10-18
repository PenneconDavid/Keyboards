/* SPDX-License-Identifier: GPL-2.0-or-later */
#pragma once

#include "transactions.h"
#include "wpm.h"
#include "atomic_util.h"
#include "oled_assets.h"

enum { PET_CTRL = 1, PET_CAPS = 2, PET_JUMP = 4, PET_SLEEP = 8 };
typedef struct {
    uint8_t wpm;
    uint8_t flags;
} pet_state_t;

static volatile pet_state_t pet_state;
static bool space_held;
static bool jump_latched;
static uint32_t jump_started;

static void oled_track_space(uint16_t keycode, keyrecord_t *record) {
    if (IS_QK_LAYER_TAP(keycode)) {
        if (!record->tap.count) return; // Holding Symbols is not a Space press.
        keycode = QK_LAYER_TAP_GET_TAP_KEYCODE(keycode);
    }
    if (keycode == KC_SPC) {
        space_held = record->event.pressed;
        if (space_held) {
            jump_latched = true;
            jump_started = timer_read32();
        }
    }
}

static void pet_receive(uint8_t size, const void *data, uint8_t reply_size, void *reply) {
    (void)reply_size;
    (void)reply;
    if (size == sizeof(pet_state)) {
        const pet_state_t *next = data;
        pet_state.wpm = next->wpm;
        pet_state.flags = next->flags;
    }
}

void housekeeping_task_user(void) {
    if (!is_keyboard_master()) return;
    static uint32_t last_attempt;
    static uint32_t last_success;
    static pet_state_t sent;
    static bool synced;
    if (timer_elapsed32(last_attempt) < 100) return;
    last_attempt = timer_read32();
    if (jump_latched && !space_held && timer_elapsed32(jump_started) >= 400) jump_latched = false;
    pet_state_t next = {
        .wpm = get_current_wpm(),
        .flags = ((get_mods() | get_weak_mods()) & MOD_MASK_CTRL ? PET_CTRL : 0) |
                 (host_keyboard_led_state().caps_lock ? PET_CAPS : 0) |
                 (space_held || jump_latched ? PET_JUMP : 0) |
                 (last_input_activity_elapsed() >= OLED_TIMEOUT ? PET_SLEEP : 0),
    };
    if (!synced || memcmp(&sent, &next, sizeof(next)) || timer_elapsed32(last_success) >= 500) {
        if (transaction_rpc_send(KEEB_PET_STATE, sizeof(next), &next)) {
            sent = next;
            synced = true;
            last_success = timer_read32();
        }
    }
}

static oled_rotation_t rot_from_deg(uint16_t degrees) {
    switch (degrees) {
        case 0: return OLED_ROTATION_0;
        case 90: return OLED_ROTATION_90;
        case 180: return OLED_ROTATION_180;
        default: return OLED_ROTATION_270;
    }
}

static uint16_t deg_from_rot(oled_rotation_t rotation) {
    switch (rotation) {
        case OLED_ROTATION_0: return 0;
        case OLED_ROTATION_90: return 90;
        case OLED_ROTATION_180: return 180;
        default: return 270;
    }
}

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    // Keep crkbd's right-side mounting correction, then add the portrait offset.
    return rot_from_deg(((is_keyboard_master() ? ROT_LEFT : ROT_RIGHT) + deg_from_rot(rotation)) % 360u);
}

static uint8_t reverse_bits(uint8_t value) {
    value = (value >> 4) | (value << 4);
    value = ((value & 0xcc) >> 2) | ((value & 0x33) << 2);
    return ((value & 0xaa) >> 1) | ((value & 0x55) << 1);
}

static void write_oled_byte(uint8_t value, uint16_t index, bool flip) {
    if (flip) {
        index = 511 - index;
        value = reverse_bits(value);
    }
    oled_write_raw_byte(value, index);
}

static uint8_t read_oled_byte(uint16_t index, bool flip) {
    uint8_t value = *oled_read_raw(flip ? 511 - index : index).current_element;
    return flip ? reverse_bits(value) : value;
}

static void decode_graphic(const uint8_t *stream, uint16_t remaining, uint16_t window, uint16_t base, bool flip) {
    uint16_t index = 0;
    // The OLED framebuffer itself is the history ring. Only the final frame is sent
    // to the display, after oled_task_user returns; no second framebuffer is needed.
    while (remaining) {
        uint8_t token = pgm_read_byte(stream++);
        uint8_t length = (token & 0x80) ? (token & 0x3f) + 3 : token + 1;
        uint16_t distance = 0;
        if (token & 0x80) distance = (((uint16_t)(token & 0x40) << 2) | pgm_read_byte(stream++)) + 1;
        while (length-- && remaining) {
            uint8_t value;
            if (distance) {
                uint16_t from = index + window - distance;
                if (from >= window) from -= window;
                value = read_oled_byte(base + from, flip);
            } else {
                value = pgm_read_byte(stream++);
            }
            write_oled_byte(value, base + index, flip);
            if (++index == window) index = 0;
            --remaining;
        }
    }
}

static void render_card(uint8_t layer) {
    if (layer > _ARC_E) layer = _BASE;
    decode_graphic(card_stream, (uint16_t)(layer + 1) * 512, 512, 0, OLED_LEFT_FLIP);
}

bool oled_task_user(void) {
    pet_state_t state;
    // AVR software serial invokes the RPC callback in an interrupt.
    ATOMIC_BLOCK_RESTORESTATE {
        state.wpm = pet_state.wpm;
        state.flags = pet_state.flags;
    }
    // Rendering dirty pixels wakes the OLED driver: stop animation before sleeping.
    if (is_keyboard_master() ? last_input_activity_elapsed() >= OLED_TIMEOUT : state.flags & PET_SLEEP) {
        oled_off();
        return false;
    }
    oled_on();
    if (is_keyboard_master()) {
        static uint8_t previous_layer = 0xff;
        uint8_t layer = get_highest_layer(layer_state | default_layer_state);
        if (layer != previous_layer) {
            render_card(layer);
            previous_layer = layer;
        }
    } else {
        static uint32_t last_frame;
        static uint8_t frame;
        static uint8_t previous_pose = 0xff;
        static bool previous_jump;
        uint8_t flags = state.flags;
        uint8_t pose = flags & PET_CAPS ? 3 : flags & PET_CTRL ? 4 : state.wpm <= 10 ? 0 : state.wpm <= 40 ? 1 : 2;
        bool jump = flags & PET_JUMP;
        if (pose != previous_pose || jump != previous_jump || timer_elapsed32(last_frame) >= 200) {
            frame ^= 1;
            last_frame = timer_read32();
            previous_pose = pose;
            previous_jump = jump;
            decode_graphic(luna_stream, (uint16_t)(pose * 2 + frame + 1) * 96, 96, 352, OLED_RIGHT_FLIP);
            // Reverse copy handles the overlapping source/destination when landing.
            if (!jump) {
                for (uint8_t i = 96; i > 0; --i) write_oled_byte(read_oled_byte(351 + i, OLED_RIGHT_FLIP), 383 + i, OLED_RIGHT_FLIP);
            }
            for (uint8_t i = 0; i < 64; ++i) {
                uint16_t index = jump ? 448 + i : i < 32 ? 352 + i : 448 + i;
                write_oled_byte(0, index, OLED_RIGHT_FLIP);
            }
        }
    }
    return false;
}

void suspend_power_down_user(void) {
#ifdef RGB_MATRIX_ENABLE
    rgb_matrix_set_suspend_state(true);
#endif
    oled_off();
}

void suspend_wakeup_init_user(void) {
#ifdef RGB_MATRIX_ENABLE
    rgb_matrix_set_suspend_state(false);
#endif
    oled_on();
}
