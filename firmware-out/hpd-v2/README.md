# Ergohaven High Plains Drifter v2 ("HPD V2" on AliExpress): gaming-first layout proposal v1

2026-10-18. Board identity confirmed by David's photo. Draft for his sign-off; nothing has been loaded on the board.

| File | What |
|---|---|
| `README.md` | this: identification, layer diagrams, how to load, knob/trackball/LED notes, open questions |
| `hpd_v2_gaming.vil` | Vial layout file, generated. **File > Load saved layout** |
| `make_vil.py` | source of truth. Run it after dropping David's `current.vil` beside it: his layers 0-3 and macros are kept, only the game layers, three selector keys and the mouse layer's left half change |
| `key-table.md` | generated key-by-key table (matrix row/col per key) for entering by hand |
| `current.vil` | **wanted from David**: Vial > File > Save current layout, so the base layer is not guessed |

---

## 1. Which board this is (confirmed)

**Ergohaven High Plains Drifter v2 (HPD v2)**: open-hardware (CC BY-NC-SA), 3D-printed, curved split by Ergohaven, with swappable side modules. The AliExpress listing (item 3256809546963642, "Wendy's 3C Store") is a third-party print of the open design; its variants (keyboard A/B, kit A/B, Trackball, Touchpad, Trackball knob, Stand) are the HPD module and case options. David's photo (`firmware-out/hpd-v2-top-down-2026-10-18.jpg`) shows the encoder on the left half's inner edge and the 38 mm trackball on the right half's inner edge, USB-C between the halves.

| | |
|---|---|
| Keys | **60**: per half 4 rows x 6 columns (24), two extra bottom-row keys and a 4-key thumb arc |
| Modules | left: EC11 encoder with one SK6812 layer-indicator LED; right: PMW3389 trackball |
| Controller | two RP2040-Zero, one per half; hand detected on a pin, either half can take the USB cable |
| Firmware | Ergohaven QMK/Vial fork, build **`hpd_v2_enc_ball`** ("encoder/trackball"), 16 layers, 109 macros, combos, tap dance, key overrides, auto shift, encoder map, pointing device with sniper/scroll/text modes and an **auto mouse layer (4)** |
| USB IDs | VID `0xE126`, PID `0x0051`, product `HPD v2 enc/ball`; Vial UID `74 2A FA 6B EB 6E 5B 17`, unchanged since at least 3.8.x |
| Matrix | 12 rows x 7 cols; left rows 0-4, right rows 6-10, knob click (5,6) |
| Flashing | double-tap Reset on the underside (or hold BOOT while plugging in), drive `RPI-RP2`, drag the UF2, **then the same for the other half** |
| Editing | Vial desktop app or https://vial.rocks. Ergohaven's own "Entropy" app is for their newer boards |
| Sources | hardware github.com/ergohaven/hpd; firmware github.com/ergohaven/vial-qmk branch `ergohaven`, `keyboards/ergohaven/hpd/rev2`, keymap `v2_enc_ball`; prebuilt **`4.1.0_hpd_v2_enc_ball.uf2`** on that repo's releases (4.1.0, 2026-10-15); docs docs.eh.industries (`device/archive/hpd`, `/firmware/`) |

The earlier identification as a Cheapino v2 (`../cheapino-hpd-v2/`, banner-marked) came from `known-good/cheapino/cheapino_vial.uf2`, which is genuine Cheapino firmware and cannot run on this board. Where that file came from is an open question (§8).

Still worth confirming from the board itself: Vial's About dialog should read *Ergohaven HPD v2 enc/ball* and show the firmware version; Device Manager should show `VID_E126&PID_0051`. If Vial shows no "Trackball settings" group under *Layouts*, the plain `hpd_v2` build is on it and the knob/ball are not driven; flash `4.1.0_hpd_v2_enc_ball.uf2` to both halves.

---

## 2. Physical layout and matrix

```
 LEFT (knob at the inner edge)                        RIGHT (ball at the inner edge)
 col:  0     1    2    3    4    5                     5    4    3    2    1    0
 r0   [`]   [1]  [2]  [3]  [4]  [5]              r6   [6]  [7]  [8]  [9]  [0]  [BS]
 r1   [Esc] [Q]  [W]  [E]  [R]  [T]              r7   [Y]  [U]  [I]  [O]  [P]  [|]
 r2   [Sft] [A]  [S]  [D]  [F]  [G]              r8   [H]  [J]  [K]  [L]  [;]  [']
 r3   [Ctl] [Z]  [X]  [C]  [V]  [B]              r9   [N]  [M]  [,]  [.]  [/]  [Sft]
 bottom extras  (4,2)↓ (4,3)↑                    bottom extras  (10,3) (10,2)   (unlabelled caps)
 thumb arc, outer -> inner:                      thumb arc, inner -> outer:
   (4,4)MENU (4,5)BS (4,0)◆ (4,1)→                 (10,1)◆ (10,0)↵ (10,5)DEL (10,4)ALT
 knob click (5,6)                                (the big arc key sits next to the module on each side)
```
Legends are the caps David's unit shipped with. They are not the Ergohaven stock keymap (stock: left arc Ctrl / Lower / Alt / Space, right arc Enter / GUI / Raise / Ctrl), so the seller may have flashed their own base layer. **That is why `current.vil` matters**: the generator keeps whatever is on layers 0-3 and only adds the game layers.

Three firmware facts that shape the design:

1. **Auto mouse layer.** Stock setting: any trackball movement switches layer 4 on for about 750 ms, and stock layer 4 puts mouse buttons on the home row of **both** halves (left: Scroll Btn3 Btn2 Btn1 Sniper on A S D F G). In a game that would turn WASD into mouse clicks the instant the ball moves. The proposal fixes it twice over: layer 4's left half is made fully transparent, and the game layers put the *same* mouse buttons on the right home row that the auto layer would, so the overlay changes nothing. Fallback: Vial > QMK Settings > Auto mouse > untick.
2. **The knob is on the left, under the hand that games.** It is an encoder-map encoder, so it can send different keys per layer: volume on Base, mouse wheel (weapon cycling) on the game layers.
3. **Modules are compile-time.** Trackball DPI, sniper/scroll modes, the encoder and its LED are all firmware; the layout only maps keys. Swapping the ball for the touchpad means flashing `enc_touch` on both halves.

---

## 3. Layers

| # | Layer | Reached by | What it is |
|---|---|---|---|
| 0 | BASE | default | David's current base (from `current.vil`), else Ergohaven stock |
| 1 | LOWER | hold Lower thumb | as today, **plus three game selectors** on the right pinky column |
| 2 | RAISE | hold Raise thumb | as today |
| 3 | ADJUST | (stock: unreachable) | as today |
| 4 | MOUSE | automatic on ball movement | as today on the right half; **left half transparent** |
| 5 | **GAME** | Lower + right `\` key | universal game base, WASD posture |
| 6 | **ARC** | Lower + right `'` key | Arc Raiders: GAME with the Corne's left block key for key |
| 7 | GAME_ESDF | Lower + right `Shift` key | same, ESDF posture, keys send WASD |
| 8-15 | free | | transparent; room for more games |

Rules for the game layers, carried over from the Corne proposal: every position bound or `KC_NO`, no transparency except on layer 4; plain keycodes only, no hold-taps; no GUI and no Lower/Raise inside a game; everything a game needs under the left hand; the right half is weapon slots, mouse buttons and the exit; one dedicated exit key, placed where the aiming hand does not rest.

### 3.1 GAME (layer 5)

```
 physical:  `    1    2    3    4    5    |   6    7    8    9    0    BS
 GAME:      `    1    2    3    4    5    |   6    7    8    9    0    Esc
            Esc  Q    W    E    R    Tab  |   ·    ·    ·    ·    ·    ·
            Sft  A    S    D    F    G    |  Snip Fire ADS  Btn3 Scrl  ·        <- same as the auto mouse layer
            Ctl  Z    X    C    V    B    |   ·    ·    ·    ·    ·    ·
 bottom extras: M  Alt                    |  right arc inner->outer: Fire  ADS  Enter  ·
 left arc outer->inner: Ctl  R  H  Space  |  right bottom extras: ·  EXIT
 knob: mouse wheel (weapon cycle), click = mute
```
Reading it with the left hand on W A S D: the pinky column is **Esc / Shift / Ctrl**, exactly as on the Corne and on the caps. R is where it always is, **Tab** (inventory) is the physical T key, **M** (map) is the outer bottom-extra key under the X column, **Alt** (dodge) the inner one under C. Weapon slots **1-5 are on the left number row**, with the backtick kept for the console; **6-0 on the right number row**. The thumb arc is **Ctrl, R, H, Space** from outer to inner, so R / H / Space are the three keys nearest the knob, the same three as every Corne game layer, and the fourth arc key is a second Ctrl for people who crouch with the thumb.

Right half, designed for aiming with the trackball (right hand on the board): home row **Sniper, Fire, ADS, Btn3, Scroll** under H J K L ; (fingers rest there while the thumb rolls the ball; identical to what the auto mouse layer would give, so the overlay is harmless), the big inner arc key by the ball is **Fire** too and the next one **ADS**, so either thumb or index can shoot. **Enter** for chat on the third arc key. **Esc** doubles on the right pinky corner. Everything else dead.

Exit: **`TO(0)` on the outer-most right bottom-extra key** (under the `/` column, pinky corner), the furthest key from the ball and from the fingers' home row, so neither the aiming thumb nor a resting hand reaches it. If David prefers a chord, Vial's Combos tab can add `Esc + <that key>`; combos are global, so the second key would need a keycode that only exists on game layers.

If David still uses a separate mouse: nothing changes on the left. On the right the mouse-button row is simply unused; the slots, Esc, Enter and EXIT stay as they are.

### 3.2 ARC (layer 6): Arc Raiders

```
 ARC = GAME with one change: the physical R key sends M.
 Result, left block:   Esc Q W E M Tab / Sft A S D F G / Ctl Z X C V B, thumbs R H Space
```
That is the Corne Arc layer key for key, so both boards feel identical in the game; R stays on the thumb as on the Corne. M on the bottom extra stays too, so the map is reachable either way. Add Arc-only keys to the `ARC = with_changes(GAME, {...})` block in `make_vil.py`.

### 3.3 GAME_ESDF (layer 7)

```
 physical:  `    1    2    3    4    5     ESDF sends:  `    1    2    3    4    5
            Esc  Q    W    E    R    T                  Tab  Esc  Q    W    E    R
            Sft  A    S    D    F    G                  M    Sft  A    S    D    F
            Ctl  Z    X    C    V    B                  Alt  Ctl  X    C    V    B
 bottom extras: G  Z   |  arc: Ctl R H Space  |  right half, knob: as GAME
```
Fingers on E S D F, which send W A S D. The pinky column is the physical Q A Z column (Esc / Shift / Ctrl) and the far-left physical column (Esc / Tab / Shift caps) becomes **Tab / M / Alt**, the Corne v4.1 ESDF arrangement with its default option A. The two letters the shift pushes off the block, G and Z, land on the bottom extras. Numbers stay literal.

### 3.4 LOWER additions and the mouse layer

Three keys are added to David's Lower layer, on the right pinky column where stock Lower is transparent: physical `\` → `TO(5)` GAME, `'` → `TO(6)` ARC, right `Shift` → `TO(7)` ESDF. Entering a game is hold Lower (left thumb) + tap a right pinky key, two hands, two steps; Lower does not exist inside a game, so there is no way to switch mid-fight. Layer 4 keeps whatever David has on the right half and gets `KC_TRNS` on every left-half key.

### 3.5 Adding a game

Copy the `ARC` block in `make_vil.py`, change the differing keys, assign it to `layers[8]`, add a `TO(8)` to `SELECTORS` (the stock Lower has free transparent spots at the right `Y`, `P` and `|` positions and on the right thumb arc), run the script, reload.

---

## 4. Diff against the Corne v4.1 game design

| | Corne (42 keys, ZMK) | HPD v2 (60 keys, Vial) |
|---|---|---|
| Esc / Shift / Ctrl | far-left column | same, physical Esc / Shift / Ctrl keys |
| Tab, M | physical T and R keys | Tab on T; M on the bottom extra, and on R in ARC |
| Alt | G key (GAME_WASD) | own key (inner bottom extra) |
| Numbers | right half 1-5 / 6-0 | left number row 1-5, right number row 6-0 |
| Thumbs | R H Space | Ctrl R H Space (arc outer to inner) |
| Mouse | separate mouse | trackball: Fire / ADS / Btn3 / Sniper / Scroll on the right home row and arc |
| Knob | volume, click mute | mouse wheel in games, volume on Base |
| Exit | right inner thumb + Esc/Esc chord | right outer bottom-extra key (`TO(0)`); chord optional |
| Entry | Fn + macro | Lower + right pinky key (`TO(n)`), overlays flattened by the generator |

---

## 5. How to load this in Vial

1. **Export first.** Plug in, open Vial (desktop or vial.rocks). Sidebar should say *HPD v2 enc/ball*. *File > Save current layout* → `keyboards/ergohaven-hpd-v2/current.vil`. This is both the rollback and the input to the generator.
2. Run `python make_vil.py` in this folder. It reports "merged with current.vil".
3. *File > Load saved layout* → `hpd_v2_gaming.vil`. Vial writes everything immediately.
4. Check layer 4 in Vial: left half all ▽, right half unchanged. Check *QMK Settings > Auto mouse* is still pointed at layer 4 (it is a board setting, not in the .vil).
5. Hold Lower, tap the right `\` key → GAME. Fingers on W A S D, thumb on R / H / Space; roll the ball and confirm the left hand still types W A S D; J fires; the knob scrolls the weapon wheel; the far right bottom key returns to Base.
6. Knob direction wrong? Swap the two entries of `enc=(…)` in the game layers and reload. Mouse buttons feel reversed (Fire on K)? Swap `KC_BTN1`/`KC_BTN2` in the GAME right home row.

If the board is on an old firmware and something does not load (missing Trackball settings, USER58/59 shown as plain "User 58"), update both halves to `4.1.0_hpd_v2_enc_ball.uf2` first, then repeat from step 1.

---

## 6. Knob, trackball and LED notes

- **Trackball in games:** DPI and sniper/scroll sensitivities are in Vial > Layouts > Trackball settings. "Sticky mode" decides whether Sniper/Scroll keys toggle or act while held; for games, held (sticky off) is the safer choice. The Text-mode key is left out of the game layers.
- **Auto mouse timeout** (QMK Settings > Auto mouse): with the game layers' right home row matching layer 4, the timeout no longer matters in games; it still governs typing.
- **LED:** the only LED is the one in the encoder module; it shows the layer colour (Ergohaven setting "LED settings" in Vial > Layouts). There are no switch LEDs on the HPD v2 PCB, so "RGB on the switches" was never possible on this hardware either. Each game layer can be given its own indicator colour in that settings group, which doubles as a "you are in a game layer" reminder.

---

## 7. Open questions for David

1. **Separate mouse in games?** Designed for the trackball. If a mouse is still used, say so and the right home row becomes dead keys (no functional loss either way).
2. **Fire on the right thumb or the index?** Both are mapped; tell me which feels right and the other can go.
3. **Knob in games:** mouse wheel (as built), volume, or something else?
4. **Bottom extras:** M outer / Alt inner, or swapped? These two keys sit low under X and C; is the thumb or the ring finger reaching them?
5. **Second Ctrl on the outer thumb-arc key:** keep, or make it something else (G, Z, Enter)?
6. **ESDF far-left column:** Tab / M / Alt (built) or one of the Corne alternatives (Tab / M / Z, Tab / Alt / M)?
7. Please export `current.vil` (§5 step 1) and say what the seller's layer 1 and 2 do, if anything useful lives there.

## 8. Loose ends for the docs/vault sessions (not touched here)

- `known-good/README.md` and `ConnectionGuide.txt` describe `known-good/cheapino/cheapino_vial.uf2` as this board's firmware. It is Cheapino v2 firmware (tompi, v2.0) and will not run on the HPD. Where it came from is unknown; the HPD's firmware comes from ergohaven/vial-qmk releases. The duplicates in `docs/dactyl-firmware/` and `archive/dactyl-firmware/` can go.
- Vault: close "HPD V2 identity" (Ergohaven High Plains Drifter v2); correct the Nov 2025 RGB note (no switch LEDs on this PCB).

## 9. Sources

- David's photo `firmware-out/hpd-v2-top-down-2026-10-18.jpg`; AliExpress item 3256809546963642 (read in the built-in browser 2026-10-17).
- github.com/ergohaven/hpd README and `images/01.png`; github.com/ergohaven/docs `content/device/archive/hpd.en.md` and `static/images/layouts/hpd_layout_en.png`.
- github.com/ergohaven/vial-qmk branch `ergohaven`: `keyboards/ergohaven/hpd/rev2/{keyboard.json,config.h,rules.mk}`, `keymaps/v2_enc_ball/{config.h,rules.mk,keymap.c,vial.json}`, `keyboards/ergohaven/{config.h,ergohaven.h}`, `src/eh_pointing.{h,c}`, `src/json/{keycodes_*.json,settings_trackball_select.json}`; releases 3.8.4 … 4.1.0 and the 4.1.0 notes.
- github.com/vial-kb/vial-gui: `protocol/keyboard_comm.py` (save/restore layout), `protocol/macro.py`, `keycodes/keycodes.py` (USERxx mapping of custom keycodes).
- Corne design: `ZmkConfig/docs/layout-proposal-2026-10.md` v4.1 changelog and §2.
