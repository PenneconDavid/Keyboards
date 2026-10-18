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

The ATmega32U4 has 28,672 bytes of application flash. See
`docs/OLED-FIRMWARE-BUILD.md` for the measured size of the layer-card/Luna build.
`rules.mk` and `config.h` retain the existing trims (`LAYER_STATE_8BIT`, no one-shot, Magic,
Grave-Escape, NKRO or Space Cadet). VIA, tap dance, combos and all six RGB effects remain enabled.

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

Left (master): a distinct static card for the highest active layer: BASE, SYM, WASD, FN, ARC,
ESDF or ARC-E. Cards change on layer transitions rather than cycling independently. ARC and
ARC-E include an arc icon and identify the underlying WASD/ESDF hand position.

Right (slave): Luna, with the original two-frame sit/walk/run/bark/sneak sprites. Speed thresholds
are 10 and 40 WPM. Caps Lock selects bark; Ctrl selects sneak. Space jumps while held and for
at least 400 ms after a press; a Symbols layer hold does not jump. Typing speed and pet flags
are sent together in a two-byte RPC snapshot, retried after failures, at most every 100 ms
with a 500 ms heartbeat. Volatile state and an atomic snapshot protect AVR interrupt delivery.

Both screens sleep after 60 seconds of input inactivity and wake for typing. Pixel changes alone
do not keep the animation awake. Suspend hooks still turn the OLEDs and RGB off with the PC.

Cards and Luna use lossless shared bitmap streams (1,489 bytes total), without a linked text font.
The decoder uses the existing OLED framebuffer as its history ring. It reconstructs a complete
selected card or sprite before QMK sends dirty display blocks; intermediate images are not shown.
`oled_assets.h` is generated by `docs/oled-previews/build_assets.py`;
`OLED-ASSETS-LICENSE.md` records Luna attribution. The exact pixel preview is
`docs/oled-previews/final/firmware-preview.png`.

Rotation: `crkbd.c` passes `oled_init_user()` 0 on the left and 180 on the right (the right OLED is
mounted upside-down). The keymap adds `ROT_LEFT` (270) / `ROT_RIGHT` (90) on top, so both logical
rotations are now 270. Hardware feedback on 2026-10-18 established that the first Luna build's
effective right rotation of 90 put Luna upside-down at the north end. The right offset was
changed from 270 to 90, rotating only that display 180 degrees. The left cards are unchanged.

If a side still needs a 180-degree adjustment, change only that side's `ROT_*` between 90 and
270 and rebuild. The alternative `OLED_LEFT_FLIP` / `OLED_RIGHT_FLIP` software knobs also turn a
side 180 degrees; do not apply both corrections together. The user confirmed the corrected
orientation looks good on 2026-10-18.

## Known issues

- Corrected Luna orientation and build approved by the user on 2026-10-18.
- Host checks and compilation cannot establish real OLED appearance or serial timing. Run the
  hardware checklist in `firmware-out/FLASH-WIRED-CORNE.md` after flashing both halves.
