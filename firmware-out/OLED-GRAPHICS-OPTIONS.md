# OLED graphics options for the wired Corne

David's verdict on the current OLED code (animated layer icons on the left, animated DAVID logo on
the right, hand-drawn pixel geometry): experimental, did not come out well, replace it. This note
lists what the displays can do, what the flash budget allows, and 4-6 concrete replacements. Nothing
is implemented yet. Numbers are from local builds on 2026-10-17 (QMK 0.29.12, avr-gcc 15.1.0,
`keyboards/crkbd/keymaps/keebv2/` as merged in PR #1).

## 1. The hardware

- Two **128x32 SSD1306** monochrome OLEDs over I2C, one per half. The QMK OLED driver keeps a
  512-byte frame buffer per half and sends only changed 8x8 blocks.
- Both run in **portrait** (`OLED_ROTATION_270` on the left, 90 on the right, which is the same
  picture once the right half's upside-down mounting is taken into account). Portrait means a
  canvas **32 px wide x 128 px tall**. Text mode gives **5 columns x 16 rows** of 6x8 glyphs.
  Landscape (rotation 0 / 180) would give 128x32 and 21 columns x 4 rows; the halves can differ.
- The **left** half is the master: it knows the layer, modifiers, caps state and WPM. The **right**
  half only knows what the master pushes over TRRS. Layer state is already synced
  (`SPLIT_LAYER_STATE_ENABLE` was on before and costs little); WPM needs `SPLIT_WPM_ENABLE`;
  modifiers need `SPLIT_MODS_ENABLE`. Anything on the right that reacts to typing needs one of
  those, or it has to react to its own keys only.
- No colour, no grey: 1 bit per pixel. Smooth motion is 2-10 fps in practice (the I2C bus is the
  limit, not the CPU).
- The display sleeps after `OLED_TIMEOUT` (60 s) and wakes on a keypress; nothing shows while idle
  unless that is raised.

## 2. The flash budget

The ATmega32U4 leaves 28 672 bytes for firmware. The current build is **28 554** (118 free). Every
option below has to pay for itself by replacing the current drawing code or by removing something
else. Measured by rebuilding the keymap with one change at a time:

| Change | Size | Saves vs today | Note |
|---|---|---|---|
| Today (merged keymap) | 28 554 | 0 | |
| Remove the current OLED drawing code, keep the OLED driver on | 24 622 | **3 932** | this is the real budget for new graphics |
| Turn the OLED off entirely | 22 574 | 5 980 | driver alone is about 2 KiB |
| Drop `ENABLE_RGB_MATRIX_TYPING_HEATMAP` | 27 818 | 736 | the biggest single effect |
| Drop all six RGB Matrix effects (keep lighting, solid colour only) | not measured | about 1.5-2 KiB (estimate) | that build failed because `RGB_MATRIX_STARTUP_MODE` points at `CYCLE_ALL`; set it to `SOLID_COLOR` first |
| Turn RGB Matrix off (no per-key RGB, no underglow) | 23 798 | 4 756 | not what David wants |
| Replace the Shift/Caps tap dance with plain Shift | 27 844 | 710 | |
| Turn VIA off | 25 980 | 2 574 | not an option, he remaps live |
| `KEYBOARD_SHARED_EP = yes` | 28 502 | 52 | not worth it on this build |
| `LTO_ENABLE = no` | 34 482 | **+5 928** | LTO is already on and already saving this much |

Already removed to make v4.1 fit: Magic keycodes, Grave-Escape, NKRO, Space Cadet, one-shot keys,
16-bit layer state. There is nothing else free left to drop.

Rule of thumb: **about 4.0 KiB for new graphics on both halves together** (3 932 + 118). If text is
used anywhere, the font table costs **about 1.5 KiB** of that (the current code draws pixels only, so
no font is linked today). Dropping the typing heatmap buys another 736 bytes if needed.

Cost units for the options below: a full portrait frame (32x128, 1 bit) is **512 bytes**; a 32x32
sprite is **128 bytes**; `oled_write_raw_P()` of a stored frame is about 50 bytes of code; the WPM
feature (`WPM_ENABLE = yes`) is roughly 300-400 bytes plus `SPLIT_WPM_ENABLE` if the right half
should see it.

## 3. Options

### A. Bongo Cat (without or with WPM)

The cat that slaps the desk when you type. Original art by StrayRogue and DitzyFlama; the
QMK-ready implementation most people copy is https://github.com/nwii/oledbongocat (frames stored
as diffs against the idle frame to save flash; speed follows typing). WPM-driven variants exist in
many keymaps and use QMK's WPM feature (https://docs.qmk.fm/features/wpm).

- Cost: roughly 2.0-2.5 KiB for the frames and the player (the compressed nwii set is around
  1.9 KiB plus code); add about 400 bytes for the WPM version. Fits, but it is most of the budget
  for one half.
- Orientation: the art is **landscape 128x32**. On a portrait half it is sideways. Either run that
  half at rotation 0 (then it is the only landscape thing on the board) or re-draw the frames for
  32x128, which nobody has published.
- License: the code is GPL (check `LICENSE` in the repo; QMK keymaps are GPL-2.0-or-later). The
  art is a meme; fine for personal firmware, not for selling.

### B. Luna / Felix keyboard pet

A 32x32 dog that sits, walks, runs, barks and sneaks depending on WPM and modifiers. Luna is by
HellSingCoder, written for a Sofle OLED at rotation 270, i.e. **portrait, exactly our layout**:
https://github.com/HellSingCoder/qmk_firmware/tree/master/keyboards/sofle/keymaps/helltm. Felix is
the same code with a different dog; it circulates in Sofle and Kyria keymaps (search GitHub for
`felix` with `keyboards/sofle/keymaps`); license follows whichever keymap it is taken from, which
is GPL-2.0 in QMK forks.

- Cost: 10 sprites x 128 bytes = 1 280 bytes, plus about 500 bytes of state code, plus WPM
  (about 400). **About 2.2 KiB**, fits with room for a layer indicator on the other half.
- Needs `WPM_ENABLE` and, if the dog lives on the right half, `SPLIT_WPM_ENABLE` and
  `SPLIT_MODS_ENABLE` (sneak = Ctrl held, bark = Space held).
- Portrait-native, so no orientation work. The dog sits in a 32x32 box; the rest of the 128 px
  column is free for a layer name or logo.

### C. Static custom logo (image2cpp)

Draw or export a 32x128 (or 128x32) 1-bit PNG, convert with https://javl.github.io/image2cpp/
("Vertical - 1 bit per pixel", "Arduino code" output, byte order as the QMK docs say:
https://docs.qmk.fm/features/oled_driver#raw-image-data), paste the 512-byte array into the keymap
and call `oled_write_raw_P()` once.

- Cost: **512 bytes + about 60 bytes of code per half.** The cheapest option by far.
- image2cpp is MIT; the image is yours.
- Any logo works: initials, a Keebmaker mark, the Corne outline, the ZMK/QMK logos (both are
  trademarks; personal use is fine).
- Bonus: two static frames (one per layer group, e.g. "typing" and "GAME") still cost only 1 KiB and
  give a visible game-mode indicator.

### D. Layer and modifier status readout (text)

The classic: `oled_write_P(PSTR("GAME"), false)`, one row per thing: layer name, Shift / Ctrl / Alt /
GUI, Caps Lock, maybe the active RGB mode. Portrait gives 5 characters per row and 16 rows, which is
enough for `BASE`, `SYM`, `WASD`, `ESDF`, `ARC`, `ARC-E`, `FN` and `S C A G` modifier letters.

- Cost: the **font table, about 1.5 KiB**, plus 300-500 bytes of logic. Around 2 KiB.
- Useful rather than pretty. Combines well with B or C on the other half.
- The crkbd keyboard-level code already has `oled_render_layer_state()` and a keylogger in
  `crkbd.c`, but it is written for the default 4 layers and landscape; a keymap version is small.
- QMK docs: https://docs.qmk.fm/features/oled_driver. No external code needed.

### E. Lightweight slideshow of 32x128 frames

Several static frames (C above), cycled every few seconds or on each layer change. Good for a set
of 3-4 pictures: a logo, the Corne silhouette, a layer map, a "GAME" card.

- Cost: **512 bytes per frame + about 150 bytes** for the timer. Four frames = 2.2 KiB.
- Frame changes are slow-ish (a full 512-byte redraw over I2C takes a few frames of the OLED task);
  fine at one picture every 5 s, not for motion.
- Same tooling and license story as C.

### F. The vendor-style "crkbd" logo

What the stock Corne shows on the slave half: the word `corne` with the foostan keyboard glyph,
drawn from 63 custom characters (`0x80-0xD4`) in `keyboards/crkbd/lib/glcdfont.c`, written three
rows of 21 glyphs by `oled_render_logo()` in `crkbd.c` (`lib/logo_reader.c` has the same table).

- It is **landscape** (3 rows x 21 columns = 126x24 px). On a portrait half only the first 5
  glyphs of each row fit, so the half showing it has to run at rotation 0 / 180.
- Cost: the crkbd font table (about 1.5 KiB, the same table option D needs) + 70 bytes. If you
  already pay for the font for a text readout, the logo is nearly free.
- The glyph table is in QMK itself (GPL-2.0-or-later); the "corne" wordmark is foostan's.
- Alternative: render it once, rotate the PNG 90 degrees, and ship it as a 512-byte raw frame (C),
  no font needed and portrait is fine.

## 4. Recommendation

**Right half: Luna / Felix (B). Left half: a static layer card per layer group (C), no text.**

- Budget: B about 2.2 KiB + three 512-byte cards for the left (typing / symbols-fn / game) 1.5 KiB
  + glue = roughly 3.9 KiB. That is inside the 4.0 KiB freed by removing the current drawing code;
  if it lands over, drop the typing heatmap (736 bytes) or go to two cards.
- Why not A: landscape art on a portrait board, and it eats the whole budget for one half.
- Why not D: 1.5 KiB of font for five-character words; the cards say the same thing in pictures.
- Why not F alone: landscape too, and the glyph font is as expensive as the text option.
- Cheapest acceptable fallback if David wants zero risk: C on both halves (about 1.2 KiB total),
  which leaves 2.8 KiB spare for the next layout change.

Order of work when this is picked up: (1) delete the current `#ifdef OLED_ENABLE` block,
(2) rebuild to confirm 24 6xx bytes, (3) add the raw frames with `oled_write_raw_P`, (4) add the
pet, (5) check the size line after each step. Keep the `oled_init_user()` rotation fix from PR #1
as is.
