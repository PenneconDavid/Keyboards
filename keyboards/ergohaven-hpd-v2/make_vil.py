#!/usr/bin/env python3
"""Generate the Vial layout (.vil) and the key table for the Ergohaven HPD v2 (encoder left, trackball right).

Source of truth for the gaming layers. Run

    python make_vil.py

If `current.vil` (David's export: Vial > File > Save current layout) sits next to this script, its layers
0-3 and everything else it holds (macros, combos, settings) are kept, and only these things change:
  * layer 1 (Lower): three game-selector keys on the right outer column
  * layer 4 (auto mouse layer): the LEFT half becomes transparent so a trackball move never remaps WASD
  * layers 5, 6, 7: GAME, ARC, GAME_ESDF
Without `current.vil` the Ergohaven stock layout (firmware 4.1.0, keymap v2_enc_ball) is used for 0-4.

Board facts (ergohaven/vial-qmk, keyboards/ergohaven/hpd/rev2 + keymaps/v2_enc_ball):
  * matrix 12 rows x 7 cols; left keys rows 0-4, right keys rows 6-10; encoder click left (5,6), right (11,6)
  * one encoder (left) in this build; encoder_layout index 0 = counter-clockwise, 1 = clockwise
  * 16 dynamic layers, Vial UID 74 2A FA 6B EB 6E 5B 17, auto mouse layer = 4 by default
"""
import json
from pathlib import Path

ROWS, COLS, LAYER_COUNT = 12, 7, 16
UID = int.from_bytes(bytes([0x74, 0x2A, 0xFA, 0x6B, 0xEB, 0x6E, 0x5B, 0x17]), "little")

# Physical key -> matrix (row, col). Each half: 4 rows x 6 columns, written pinky -> inner for the left
# half and inner -> pinky for the right half, so a layer reads like the keyboard.
LEFT = [[(r, c) for c in range(6)] for r in range(4)]            # (0,0) = top-left corner key
RIGHT = [[(r, c) for c in (5, 4, 3, 2, 1, 0)] for r in range(6, 10)]  # (6,5) = inner top key
# Bottom row: two keys under the W/E (left) or O/P (right) columns, then the 4-key thumb arc.
LEFT_BOTTOM = [(4, 2), (4, 3)]                  # outer, inner   (caps: down-arrow, up-arrow)
LEFT_ARC = [(4, 4), (4, 5), (4, 0), (4, 1)]      # outer -> inner (caps: MENU, BS, layer, right-arrow); inner = big key by the knob
RIGHT_ARC = [(10, 1), (10, 0), (10, 5), (10, 4)]  # inner -> outer (caps: layer, Enter, DEL, ALT); inner = big key by the ball
RIGHT_BOTTOM = [(10, 3), (10, 2)]                # inner, outer   (unlabelled caps under . and /)
ENC_CLICK_L, ENC_CLICK_R = (5, 6), (11, 6)

T, N = "KC_TRNS", "KC_NO"
SNIPER, SCROLL, TEXTMODE = "USER58", "USER59", "USER60"   # Ergohaven pointing keycodes (keycodes_all.json order)


def blank():
    return [[-1] * COLS for _ in range(ROWS)]


def layer(left, right, left_bottom, left_arc, right_arc, right_bottom, enc_click=N, enc=(T, T)):
    """left/right: 4 lists of 6 keycodes. left_bottom/right_bottom: 2. left_arc/right_arc: 4 (see order above)."""
    g = blank()
    for r in range(4):
        for c in range(6):
            g[LEFT[r][c][0]][LEFT[r][c][1]] = left[r][c]
            g[RIGHT[r][c][0]][RIGHT[r][c][1]] = right[r][c]
    for pos, kc in zip(LEFT_BOTTOM + LEFT_ARC, left_bottom + left_arc):
        g[pos[0]][pos[1]] = kc
    for pos, kc in zip(RIGHT_ARC + RIGHT_BOTTOM, right_arc + right_bottom):
        g[pos[0]][pos[1]] = kc
    g[ENC_CLICK_L[0]][ENC_CLICK_L[1]] = enc_click
    g[ENC_CLICK_R[0]][ENC_CLICK_R[1]] = N
    return {"grid": g, "encoder": list(enc)}


def with_changes(base, changes, enc=None):
    """Thin overlay: copy a layer and change only the listed (row, col) -> keycode entries."""
    g = [row[:] for row in base["grid"]]
    for (r, c), kc in changes.items():
        g[r][c] = kc
    return {"grid": g, "encoder": list(enc) if enc else base["encoder"][:]}


# ------------------------------------------------------------ Ergohaven stock layers (fallback when no current.vil)
STOCK_BASE = layer(
    [["KC_GRAVE", "KC_1", "KC_2", "KC_3", "KC_4", "KC_5"],
     ["KC_ESCAPE", "KC_Q", "KC_W", "KC_E", "KC_R", "KC_T"],
     ["KC_TAB", "KC_A", "KC_S", "KC_D", "KC_F", "KC_G"],
     ["KC_LSHIFT", "KC_Z", "KC_X", "KC_C", "KC_V", "KC_B"]],
    [["KC_6", "KC_7", "KC_8", "KC_9", "KC_0", "KC_BSPACE"],
     ["KC_Y", "KC_U", "KC_I", "KC_O", "KC_P", "KC_BSLASH"],
     ["KC_H", "KC_J", "KC_K", "KC_L", "KC_SCOLON", "KC_QUOTE"],
     ["KC_N", "KC_M", "KC_COMMA", "KC_DOT", "KC_SLASH", "KC_RSHIFT"]],
    ["KC_PGDOWN", "KC_PGUP"], ["KC_LCTRL", "MO(1)", "KC_LALT", "KC_SPACE"],
    ["KC_ENTER", "KC_RGUI", "MO(2)", "KC_RCTRL"], ["KC_LBRACKET", "KC_RBRACKET"],
    enc_click="KC_MUTE", enc=("KC_VOLD", "KC_VOLU"),
)
STOCK_LOWER = layer(
    [["KC_F1", "KC_F2", "KC_F3", "KC_F4", "KC_F5", "KC_F6"],
     ["KC_VOLU", "KC_MNXT", "KC_HOME", "KC_UP", "KC_END", "KC_INSERT"],
     ["KC_MUTE", "KC_MPLY", "KC_LEFT", "KC_DOWN", "KC_RIGHT", "KC_ENTER"],
     ["KC_VOLD", "KC_MPRV", "LCTL(KC_LEFT)", T, "LCTL(KC_RIGHT)", "KC_DELETE"]],
    [["KC_F7", "KC_F8", "KC_F9", "KC_F10", "KC_F11", "KC_F12"],
     [T, "KC_7", "KC_8", "KC_9", T, T],
     [T, "KC_4", "KC_5", "KC_6", T, T],
     ["KC_PSCREEN", "KC_1", "KC_2", "KC_3", T, T]],
    [T, T], [T, T, T, T], [T, T, T, T], ["KC_0", T],
)
STOCK_RAISE = layer(
    [[T, T, "KC_AT", "KC_DLR", "KC_HASH", T],
     [T, T, "KC_LT", "KC_EQUAL", "KC_GT", "KC_GRAVE"],
     [T, "KC_BSLASH", "KC_LPRN", "KC_MINUS", "KC_RPRN", "KC_PLUS"],
     [T, T, "KC_ASTR", "KC_COLN", "KC_SLASH", T]],
    [[T, T, "KC_CIRC", T, T, T],
     [T, "KC_LBRACKET", "KC_UNDS", "KC_RBRACKET", T, T],
     ["KC_PERC", "KC_LCBR", "KC_SCOLON", "KC_RCBR", "KC_EXLM", T],
     [T, "KC_PIPE", "KC_TILD", "KC_AMPR", T, T]],
    [T, T], [T, T, T, T], [T, T, T, T], [T, T],
)
STOCK_ADJUST = layer([[T] * 6] * 4, [[T] * 6] * 4, [T, T], [T] * 4, [T] * 4, [T, T])
STOCK_MOUSE = layer(   # stock layer 4: mouse buttons on BOTH home rows (the left half is what breaks games)
    [[T] * 6, [T] * 6, [T, SCROLL, "KC_BTN3", "KC_BTN2", "KC_BTN1", SNIPER], [T, T, T, T, T, TEXTMODE]],
    [[T] * 6, [T] * 6, [SNIPER, "KC_BTN1", "KC_BTN2", "KC_BTN3", SCROLL, T], [TEXTMODE, T, T, T, T, T]],
    [T, T], [T] * 4, [T] * 4, [T, T],
)

# ------------------------------------------------------------ our changes to the typing layers
# Game selectors on LOWER: hold the Lower thumb, tap a right-hand pinky-column key.
SELECTORS = {(7, 0): "TO(5)", (8, 0): "TO(6)", (9, 0): "TO(7)"}   # physical \ ' Shift(right) keys -> GAME, ARC, ESDF


def mouse_layer_safe(mouse):
    """Auto mouse layer with the whole left half transparent (WASD side is never touched by the ball)."""
    changes = {pos: T for row in LEFT for pos in row}
    changes.update({pos: T for pos in LEFT_BOTTOM + LEFT_ARC})
    return with_changes(mouse, changes)


# ------------------------------------------------------------ layer 5: GAME (universal base, WASD posture)
GAME = layer(
    [["KC_GRAVE", "KC_1", "KC_2", "KC_3", "KC_4", "KC_5"],
     ["KC_ESCAPE", "KC_Q", "KC_W", "KC_E", "KC_R", "KC_TAB"],
     ["KC_LSHIFT", "KC_A", "KC_S", "KC_D", "KC_F", "KC_G"],
     ["KC_LCTRL", "KC_Z", "KC_X", "KC_C", "KC_V", "KC_B"]],
    [["KC_6", "KC_7", "KC_8", "KC_9", "KC_0", "KC_ESCAPE"],
     [N, N, N, N, N, N],
     [SNIPER, "KC_BTN1", "KC_BTN2", "KC_BTN3", SCROLL, N],     # = stock auto mouse row, so a ball move changes nothing
     [N, N, N, N, N, N]],
    ["KC_M", "KC_LALT"],                                   # bottom extras: M (map), Alt (dodge)
    ["KC_LCTRL", "KC_R", "KC_H", "KC_SPACE"],              # arc outer -> inner: Ctrl, R (reload), H (put away), Space
    ["KC_BTN1", "KC_BTN2", "KC_ENTER", N],                 # right arc inner -> outer: fire, ADS, Enter (chat), dead
    [N, "TO(0)"],                                          # right bottom: dead, EXIT (outer-most key, under the / column)
    enc_click="KC_MUTE", enc=("KC_WH_D", "KC_WH_U"),       # knob = weapon wheel (mouse wheel); click = mute
)

# ------------------------------------------------------------ layer 6: ARC (Arc Raiders overlay: the Corne's left block key for key)
ARC = with_changes(GAME, {
    (1, 4): "KC_M",      # physical R key sends M, as on the Corne Arc layer (R stays on the thumb)
})

# ------------------------------------------------------------ layer 7: GAME_ESDF (fingers on E S D F, keys send W A S D)
ESDF = layer(
    [["KC_GRAVE", "KC_1", "KC_2", "KC_3", "KC_4", "KC_5"],
     ["KC_TAB", "KC_ESCAPE", "KC_Q", "KC_W", "KC_E", "KC_R"],     # physical: Esc Q W E R T
     ["KC_M", "KC_LSHIFT", "KC_A", "KC_S", "KC_D", "KC_F"],       # physical: Tab A S D F G
     ["KC_LALT", "KC_LCTRL", "KC_X", "KC_C", "KC_V", "KC_B"]],    # physical: Sft Z X C V B
    GAME_RIGHT := [row[:] for row in [
        ["KC_6", "KC_7", "KC_8", "KC_9", "KC_0", "KC_ESCAPE"],
        [N, N, N, N, N, N],
        [SNIPER, "KC_BTN1", "KC_BTN2", "KC_BTN3", SCROLL, N],
        [N, N, N, N, N, N]]],
    ["KC_G", "KC_Z"],                                      # bottom extras: the two letters the shift pushed off the block
    ["KC_LCTRL", "KC_R", "KC_H", "KC_SPACE"],
    ["KC_BTN1", "KC_BTN2", "KC_ENTER", N],
    [N, "TO(0)"],
    enc_click="KC_MUTE", enc=("KC_WH_D", "KC_WH_U"),
)

EMPTY = layer([[T] * 6] * 4, [[T] * 6] * 4, [T, T], [T] * 4, [T] * 4, [T, T], enc_click=T)
LAYER_NAMES = ["BASE", "LOWER", "RAISE", "ADJUST", "MOUSE (auto)", "GAME", "ARC", "GAME_ESDF"]


def build(current=None):
    if current:
        layers = [{"grid": g, "encoder": e[0] if e else [T, T]}
                  for g, e in zip(current["layout"], current.get("encoder_layout", []) or [[[T, T]]] * LAYER_COUNT)]
        while len(layers) < LAYER_COUNT:
            layers.append(EMPTY)
    else:
        layers = [STOCK_BASE, STOCK_LOWER, STOCK_RAISE, STOCK_ADJUST, STOCK_MOUSE] + [EMPTY] * (LAYER_COUNT - 5)
    layers[1] = with_changes(layers[1], SELECTORS)
    layers[4] = mouse_layer_safe(layers[4])
    layers[5], layers[6], layers[7] = GAME, ARC, ESDF

    vil = dict(current) if current else {
        "version": 1, "uid": UID, "layout_options": -1, "macro": [], "vial_protocol": 6, "via_protocol": 9,
        "tap_dance": [], "combo": [], "key_override": [], "settings": {},
    }
    vil["uid"] = UID
    vil["layout"] = [l["grid"] for l in layers]
    vil["encoder_layout"] = [[l["encoder"]] for l in layers]
    return vil, layers


def pretty(kc):
    return {N: "·", -1: "", T: "▽", SNIPER: "Sniper", SCROLL: "Scroll", TEXTMODE: "TextMd"}.get(kc, kc)


def table(layers):
    out = ["# Key-by-key table (generated by make_vil.py)", "",
           "Hover a key in Vial to see its (row, col). Left half rows 0-4, right half rows 6-10, knob click (5,6). "
           "Columns run pinky to inner on both halves. · = KC_NO, ▽ = transparent, Sniper/Scroll = Ergohaven USER58/USER59.", ""]
    for i, (name, l) in enumerate(zip(LAYER_NAMES, layers)):
        g = l["grid"]
        out += [f"## Layer {i}: {name}", "", "| Half | Row | pinky | ring | middle | index | index+1 | inner |", "|---|---|---|---|---|---|---|---|"]
        for r, label in enumerate(["num", "top", "home", "bottom"]):
            out.append(f"| left | {label} | " + " | ".join(pretty(g[p[0]][p[1]]) for p in LEFT[r]) + " |")
        for r, label in enumerate(["num", "top", "home", "bottom"]):
            out.append(f"| right | {label} | " + " | ".join(pretty(g[p[0]][p[1]]) for p in reversed(RIGHT[r])) + " |")
        lb = [pretty(g[p[0]][p[1]]) for p in LEFT_BOTTOM]; la = [pretty(g[p[0]][p[1]]) for p in LEFT_ARC]
        ra = [pretty(g[p[0]][p[1]]) for p in RIGHT_ARC]; rb = [pretty(g[p[0]][p[1]]) for p in RIGHT_BOTTOM]
        out += ["", f"Left bottom extras (outer, inner): {lb[0]}, {lb[1]}. Left thumb arc (outer to inner): {', '.join(la)}.",
                f"Right thumb arc (inner to outer): {', '.join(ra)}. Right bottom extras (inner, outer): {rb[0]}, {rb[1]}.",
                f"Knob: CCW = {pretty(l['encoder'][0])}, CW = {pretty(l['encoder'][1])}, click = {pretty(g[5][6])}.", ""]
    return "\n".join(out)


if __name__ == "__main__":
    here = Path(__file__).parent
    cur_path = here / "current.vil"
    current = json.loads(cur_path.read_text(encoding="utf-8")) if cur_path.exists() else None
    vil, layers = build(current)
    (here / "hpd_v2_gaming.vil").write_text(json.dumps(vil), encoding="utf-8")
    (here / "key-table.md").write_text(table(layers), encoding="utf-8")
    print("wrote hpd_v2_gaming.vil and key-table.md", "(merged with current.vil)" if current else "(from Ergohaven stock layers 0-4)")
