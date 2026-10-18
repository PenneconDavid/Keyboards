#!/usr/bin/env python3
"""Generate the Vial layout (.vil) and the key-by-key table for the HPD V2 / Cheapino v2.

Source of truth for the gaming layout. Edit the layer definitions below, run

    python make_vil.py

and load the resulting cheapino_hpd_v2_gaming.vil in Vial (File > Load saved layout).

Board facts (from the seller UF2, which is tompi's cheapino_vial.uf2 v2.0):
  * matrix 8 rows x 12 cols, duplex; right half = rows 0-2 cols 0-5, left half = rows 4-6 cols 6-11
  * encoder click = (3,0); encoder rotation = encoder 0 (index 0 = counter-clockwise, 1 = clockwise)
  * 14 dynamic layers, Vial UID 0x9EC18B09BB40ED79 (11439577402410331513)
"""
import json
from pathlib import Path

ROWS, COLS, LAYER_COUNT = 8, 12, 14
UID = int.from_bytes(bytes([0x79, 0xED, 0x40, 0xBB, 0x09, 0x8B, 0xC1, 0x9E]), "little")

# Physical position -> matrix (row, col). Left half listed pinky -> index, right half index -> pinky,
# so that a layer written as 5 + 5 keys per row reads like the keyboard.
LEFT = [
    [(4, 10), (4, 9), (4, 8), (4, 7), (4, 6)],
    [(5, 10), (5, 9), (5, 8), (5, 7), (5, 6)],
    [(6, 10), (6, 9), (6, 8), (6, 7), (6, 6)],
]
RIGHT = [
    [(0, 0), (0, 1), (0, 2), (0, 3), (0, 4)],
    [(1, 0), (1, 1), (1, 2), (1, 3), (1, 4)],
    [(2, 0), (2, 1), (2, 2), (2, 3), (2, 4)],
]
LEFT_THUMBS = [(6, 11), (5, 11), (4, 11)]      # outer, middle, inner
RIGHT_THUMBS = [(2, 5), (1, 5), (0, 5)]        # inner, middle, outer
ENCODER_CLICK = (3, 0)

T = "KC_TRNS"
N = "KC_NO"


def layer(rows_left, rows_right, thumbs_left, thumbs_right, enc_click, enc=("KC_VOLD", "KC_VOLU")):
    """Build one layer. rows_* are 3 lists of 5 keycodes, thumbs_* 3 keycodes (left: outer->inner,
    right: inner->outer), enc = (counter-clockwise, clockwise)."""
    grid = [[-1] * COLS for _ in range(ROWS)]
    for r in range(3):
        for c in range(5):
            grid[LEFT[r][c][0]][LEFT[r][c][1]] = rows_left[r][c]
            grid[RIGHT[r][c][0]][RIGHT[r][c][1]] = rows_right[r][c]
    for pos, kc in zip(LEFT_THUMBS, thumbs_left):
        grid[pos[0]][pos[1]] = kc
    for pos, kc in zip(RIGHT_THUMBS, thumbs_right):
        grid[pos[0]][pos[1]] = kc
    grid[ENCODER_CLICK[0]][ENCODER_CLICK[1]] = enc_click
    return {"grid": grid, "encoder": list(enc)}


def with_changes(base, changes):
    """Thin overlay: copy a layer and change only the listed (row, col) -> keycode entries."""
    grid = [row[:] for row in base["grid"]]
    for (r, c), kc in changes.items():
        grid[r][c] = kc
    return {"grid": grid, "encoder": base["encoder"][:]}


# ---------------------------------------------------------------- layer 0: BASE (seller default, lightly edited)
BASE = layer(
    [["KC_Q", "KC_W", "KC_E", "KC_R", "KC_T"],
     ["KC_A", "KC_S", "KC_D", "KC_F", "KC_G"],
     ["LGUI_T(KC_Z)", "LALT_T(KC_X)", "LCTL_T(KC_C)", "LSFT_T(KC_V)", "KC_B"]],
    [["KC_Y", "KC_U", "KC_I", "KC_O", "KC_P"],
     ["KC_H", "KC_J", "KC_K", "KC_L", "KC_SCOLON"],
     ["KC_N", "RSFT_T(KC_M)", "RCTL_T(KC_COMMA)", "RALT_T(KC_DOT)", "KC_SLASH"]],
    ["KC_ESCAPE", "KC_SPACE", "LT2(KC_TAB)"],          # left: Esc | Space | Tab (hold = FN)
    ["MO(1)", "KC_BSPACE", "KC_ENTER"],                 # right: NUM/SYM (hold) | Bksp | Enter
    "KC_MUTE",
)

# ---------------------------------------------------------------- layer 1: NUM / SYM (hold right inner thumb)
NUMSYM = layer(
    [["KC_EXLM", "KC_AT", "KC_HASH", "KC_DLR", "KC_PERC"],
     ["KC_QUOTE", "KC_DQUO", "KC_LPRN", "KC_RPRN", "KC_UNDS"],
     ["KC_GRAVE", "KC_TILD", "KC_LBRACKET", "KC_RBRACKET", "KC_BSLASH"]],
    [["KC_7", "KC_8", "KC_9", "KC_MINUS", "KC_EQUAL"],
     ["KC_4", "KC_5", "KC_6", "KC_PLUS", "KC_ASTR"],
     ["KC_1", "KC_2", "KC_3", "KC_0", "KC_DOT"]],
    ["KC_LSHIFT", "KC_SPACE", "KC_TAB"],                # Shift on the thumb gives { } | < > etc.
    [T, "KC_BSPACE", "KC_ENTER"],
    "KC_MUTE",
    enc=("KC_WH_D", "KC_WH_U"),
)

# ---------------------------------------------------------------- layer 2: FN / NAV (hold left inner thumb = Tab)
FN = layer(
    [["KC_F1", "KC_F2", "KC_F3", "KC_F4", "KC_F5"],
     ["KC_F6", "KC_F7", "KC_F8", "KC_F9", "KC_F10"],
     ["KC_F11", "KC_F12", "KC_PSCREEN", "RGB_TOG", "RGB_MOD"]],
    [["KC_HOME", "KC_PGDOWN", "KC_PGUP", "KC_END", "KC_DELETE"],
     ["KC_LEFT", "KC_DOWN", "KC_UP", "KC_RIGHT", "KC_CAPSLOCK"],
     ["TO(3)", "TO(4)", "TO(5)", "KC_INSERT", "QK_BOOT"]],   # game selectors: GAME, ARC, WASD
    ["KC_LGUI", "KC_LALT", T],
    ["KC_MUTE", "KC_MPLY", "KC_MNXT"],
    "KC_MPLY",
    enc=("RGB_VAD", "RGB_VAI"),
)

# ---------------------------------------------------------------- layer 3: GAME (universal base; ESDF posture sending WASD)
# Physical keys (left half):   Q  W  E  R  T  /  A  S  D  F  G  /  Z  X  C  V  B
GAME = layer(
    [["KC_ESCAPE", "KC_Q", "KC_W", "KC_E", "KC_TAB"],
     ["KC_LSHIFT", "KC_A", "KC_S", "KC_D", "KC_M"],
     ["KC_LCTRL", "KC_X", "KC_C", "KC_V", "KC_LALT"]],
    [["KC_1", "KC_2", "KC_3", "KC_4", "KC_5"],
     ["KC_6", "KC_7", "KC_8", "KC_9", "KC_0"],
     ["KC_F", "KC_G", "KC_B", "KC_Z", N]],
    ["KC_R", "KC_H", "KC_SPACE"],                       # thumbs: R (reload) | H (put away) | Space
    ["TO(0)", "KC_ENTER", N],                           # right inner thumb = EXIT to typing, middle = Enter
    "KC_MUTE",
)

# ---------------------------------------------------------------- layer 4: ARC (Arc Raiders overlay on GAME)
# Everything Arc needs that fits on this half is already in GAME; add Arc-only keys here.
ARC = with_changes(GAME, {
    # example: (2, 4): "KC_T",   # right-half bottom pinky -> T
})

# ---------------------------------------------------------------- layer 5: GAME_WASD (literal letters; mods on thumbs)
WASD = layer(
    [["KC_Q", "KC_W", "KC_E", "KC_R", "KC_TAB"],
     ["KC_A", "KC_S", "KC_D", "KC_F", "KC_M"],
     ["KC_Z", "KC_X", "KC_C", "KC_V", "KC_H"]],
    [["KC_1", "KC_2", "KC_3", "KC_4", "KC_5"],
     ["KC_6", "KC_7", "KC_8", "KC_9", "KC_0"],
     ["KC_G", "KC_B", "KC_T", "KC_LALT", "KC_ESCAPE"]],
    ["KC_LSHIFT", "KC_LCTRL", "KC_SPACE"],              # thumbs: Shift (sprint) | Ctrl (crouch) | Space
    ["TO(0)", "KC_ENTER", N],
    "KC_MUTE",
)

LAYERS = [BASE, NUMSYM, FN, GAME, ARC, WASD]
LAYER_NAMES = ["BASE", "NUM/SYM", "FN/NAV", "GAME", "ARC", "GAME_WASD"]

EMPTY = layer([[T] * 5] * 3, [[T] * 5] * 3, [T] * 3, [T] * 3, T, enc=(T, T))
while len(LAYERS) < LAYER_COUNT:
    LAYERS.append(EMPTY)


def build_vil():
    return {
        "version": 1,
        "uid": UID,
        "layout": [l["grid"] for l in LAYERS],
        "encoder_layout": [[l["encoder"]] for l in LAYERS],
        "layout_options": -1,
        "macro": [[] for _ in range(16)],
        "vial_protocol": 6,
        "via_protocol": 9,
        "tap_dance": [],
        "combo": [],
        "key_override": [],
        "settings": {},
    }


def pretty(kc):
    if kc in (N, -1):
        return "·"
    if kc == T:
        return "▽"
    return kc


def build_table():
    out = ["# Key-by-key table (generated by make_vil.py)", "",
           "Matrix (row,col) as Vial shows it when you hover a key: left half rows 4-6, "
           "right half rows 0-2, encoder click (3,0). Columns run pinky to inner for both halves. "
           "· = KC_NO (dead key), ▽ = transparent.", ""]
    for i, (name, l) in enumerate(zip(LAYER_NAMES, LAYERS)):
        out += [f"## Layer {i}: {name}", "",
                "| Half | Row | pinky | ring | middle | index | inner |", "|---|---|---|---|---|---|---|"]
        for r, label in enumerate(["top", "home", "bottom"]):
            out.append(f"| left | {label} | " + " | ".join(pretty(l['grid'][p[0]][p[1]]) for p in LEFT[r]) + " |")
        for r, label in enumerate(["top", "home", "bottom"]):
            out.append(f"| right | {label} | " + " | ".join(pretty(l['grid'][p[0]][p[1]]) for p in reversed(RIGHT[r])) + " |")
        lt = [pretty(l["grid"][p[0]][p[1]]) for p in LEFT_THUMBS]
        rt = [pretty(l["grid"][p[0]][p[1]]) for p in RIGHT_THUMBS]
        out += ["", f"Left thumbs (outer, middle, inner): {lt[0]}, {lt[1]}, {lt[2]}. "
                    f"Right thumbs (inner, middle, outer): {rt[0]}, {rt[1]}, {rt[2]}.",
                f"Encoder: turn CCW = {pretty(l['encoder'][0])}, CW = {pretty(l['encoder'][1])}, "
                f"click = {pretty(l['grid'][3][0])}.", ""]
    return "\n".join(out)


if __name__ == "__main__":
    here = Path(__file__).parent
    (here / "cheapino_hpd_v2_gaming.vil").write_text(json.dumps(build_vil()), encoding="utf-8")
    (here / "key-table.md").write_text(build_table(), encoding="utf-8")
    print("wrote cheapino_hpd_v2_gaming.vil and key-table.md")
