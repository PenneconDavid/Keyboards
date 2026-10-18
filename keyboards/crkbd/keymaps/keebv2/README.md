# keebv2: wired Corne keymap

QMK keymap for the wired Keebmaker Corne (sold as "v4.1", but it is an ATmega32U4 Pro Micro /
Caterina board: build target `crkbd` = `crkbd/rev1`, **not** `rev4_1`). Two 128x32 SSD1306 OLEDs,
per-key RGB + underglow (RGB Matrix), TRRS, `MASTER_LEFT`, one `.hex` for both halves.

It mirrors the wireless Eyelash Corne ZMK keymap **v4.1** (`ZmkConfig` main,
`config/eyelash_corne.keymap`; documented in `ZmkConfig/docs/layout-proposal-2026-10.md` and the
cheat sheet in `docs/eyelash-prospector-bringup.md` section 11; renders in
`docs/layout-proposal/final_*.svg`). Keep the two in step: change the ZMK file first, then this one.

## Build and flash

```
qmk compile -kb crkbd -km keebv2
```

Either run `scripts/flash-keebv2.ps1` from the repo root (copies this folder into
`~\qmk_firmware`, compiles, flashes left then right), or open `~\qmk_firmware\crkbd_rev1_keebv2.hex`
in QMK Toolbox, select ATmega32U4, put a half into the bootloader (double-tap reset) and flash. Flash
the same hex to both halves. Never flash without David asking.

The build is at 99 % of the 28 KiB flash (118 bytes free as of 2026-10-17). `rules.mk` and
`config.h` list the invisible trims that made it fit (`LAYER_STATE_8BIT`, no one-shot, Magic,
Grave-Escape, NKRO or Space Cadet). If a future change does not fit, the next candidates are
`KEYBOARD_SHARED_EP = yes` (a few hundred bytes, may hide the keyboard from some BIOS screens) or
dropping an RGB Matrix effect from `config.h`.

## Layers

Thumbs are written outer -> inner on the left and inner -> outer on the right.

| # | Layer | Reach | Keys |
|---|---|---|---|
| 0 | `_BASE` | default | QWERTY. Home-row pinky is `TD(0)`: tap Shift, double-tap Caps Lock. Thumbs: GUI, **Backspace (hold = Fn)**, **Space (hold = Symbols)** / Delete, Enter, RAlt |
| 1 | `_SYM` | hold Space | Left `! @ # $ %`, `` ` [ ] ( ) \ ``, `~ { } - - \|`. Right numpad `7 8 9 / *`, `4 5 6 - + =`, `1 2 3 0 . _`. Right thumbs Tab, Enter, Shift+Tab |
| 2 | `_GAME_WASD` | Fn + `WASD` | `Esc Q W E M Tab` / `Shift A S D F Alt` / `Ctrl Z X C V B`; thumbs R, H, Space. Right half: `1 2 3 4 5 Esc` / `6 7 8 9 0 -` / inert; thumbs `TO(0)`, Enter, inert |
| 3 | `_FN` | hold Backspace | F1-F12; `Home PgDn PgUp End PrtSc Bksp`; `Left Down Up Right Del Caps`; bottom left `RM_TOGG RM_NEXT - Mute Vol- Vol+` (RGB Matrix toggle / next effect); bottom right `Ins Alt+= ARC WASD ESDF ARC-E`; right thumbs Tab, (Enter), Shift+Tab |
| 4 | `_ARC` | Fn + `ARC` | Overlay on 2: one key, `G` back on the G key. Result is the original gaming layer key for key |
| 5 | `_GAME_ESDF` | Fn + `ESDF` | `Tab Esc Q W E R` / `M Shift A S D F` / `Alt Ctrl X C V B`; thumbs R, H, Space; right half as layer 2. Hand sits on ESDF, keys send plain WASD |
| 6 | `_ARC_E` | Fn + `ARC-E` | Overlay on 5: empty today |

- **Enter a game**: hold Backspace, tap one of `ARC` / `WASD` / `ESDF` / `ARC-E` (Fn, right-hand
  bottom row). `ARC` and `ARC-E` are custom keycodes `USER00` / `USER01` (`layer_move` to the game
  base, then the overlay on top). `WASD` / `ESDF` are plain `TO()`.
- **Leave a game**: right inner thumb (`TO(0)`), or **left Esc + right Esc (top-right corner)**
  together. The combo needs both keys within 50 ms after 150 ms with no other key press, and only
  exists on the game layers (so Esc, Q and Backspace have no combo delay anywhere else).
- Game layers are plain keycodes only: no layer-taps, no GUI, no way into Fn or Symbols.

### What differs from the ZMK board

The wired Corne has 42 keys; the Eyelash has 48. These ZMK bindings have no key here and are dropped:

| ZMK position | Binding | Here |
|---|---|---|
| encoder turn | volume up / down (every layer) | use Fn `Vol-` / `Vol+` |
| encoder click (34) | mute | use Fn `Mute` |
| 5-way stick (6, 19, 20, 21, 35) | arrows + Enter on Base; mouse move + left click elsewhere | arrows on Fn; no mouse keys |

Everything else is 1:1, including the Fn thumb `Tab / (Enter) / Shift+Tab` Excel helpers and
`Alt+=` AutoSum.

## VIA

VIA stays enabled with seven dynamic layers. After the first flash, **VIA's EEPROM copy wins over
`keymap.c`**, so:

1. After flashing, open https://usevia.app, check all seven layers, then **Save** a dated export to
   `layouts/via/crkbd.layout.<yyyy-MM-dd>.json` and commit it.
2. After every live edit in VIA, export again. If an edit is meant to stay, mirror it into
   `keymap.c` (and into the ZMK keymap).
3. VIA shows the custom bits as `TD(0)` (Shift/Caps), `USER00` / `USER01` (ARC / ARC-E) and
   `RM_TOGG` / `RM_NEXT` (older VIA builds print the raw hex for these).

The last export before this port is `layouts/via/crkbd.layout.2026-05-10.json` (old 4-layer map).

## OLEDs

Left (master): animated icon of the active layer. Right (slave): animated DAVID logo.

Rotation: `crkbd.c` passes `oled_init_user()` 0 on the left and 180 on the right (the right OLED is
mounted upside-down). The keymap adds `ROT_LEFT` / `ROT_RIGHT` (both 270) on top, so the left runs
at 270 and the right at 90, and both should read the same way. The previous build subtracted the
180 instead, which is why the right half rendered upside-down.

If, after flashing, **both** halves are upside-down: set `ROT_LEFT` and `ROT_RIGHT` to 90 in
`config.h`. If only one is: set that side's `OLED_LEFT_FLIP` / `OLED_RIGHT_FLIP` to `true`. This
has not been confirmed on the board yet (2026-10-17).

## Known issues

- OLED orientation fix above is untested on hardware.
- 118 bytes of flash left; see "Build and flash" for what to trim next.
