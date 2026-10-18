> **NOT DAVID'S BOARD (2026-10-17, superseded).** David's AliExpress "HPD V2" has 60 keys, a knob on the left half and a trackball on the right: it is an **Ergohaven High Plains Drifter v2**, documented in [`../ergohaven-hpd-v2/`](../ergohaven-hpd-v2/README.md). This folder is kept only because the Vial UF2 in `known-good/cheapino/` really is Cheapino v2 firmware (the analysis in §1 of this file stands for that file), so it documents a firmware file whose origin is now an open question, not David's keyboard. Do not load the .vil here on the HPD.

# HPD V2 wired Vial split (Cheapino v2 PCB): gaming-first layout proposal v1

Draft 2026-10-17 for David's sign-off. Nothing has been loaded on the board yet.

Files in this folder:

| File | What |
|---|---|
| `README.md` | this proposal: identification, layer diagrams, how to load, RGB, open questions |
| `cheapino_hpd_v2_gaming.vil` | Vial layout file, generated. **File > Load saved layout** in Vial |
| `make_vil.py` | source of truth for the layout; edit, run `python make_vil.py`, reload the .vil |
| `key-table.md` | generated key-by-key table (matrix row/col per key) for entering by hand |
| `cheapino2.vial.json` | the Vial keyboard definition extracted from the seller UF2 (reference) |

---

## 1. Which board this is

**Confirmed: the PCB is a Cheapino v2 (Thomas Haukland, "tompi"), running tompi's own Vial release firmware.** "HPD V2" is the seller's name for the kit, not a different keyboard.

Evidence, strongest first:

1. The seller UF2 in this repo (`known-good/cheapino/cheapino_vial.uf2`, identical copies in `docs/dactyl-firmware/` and `archive/dactyl-firmware/`, md5 `dab93efa…`) contains the USB strings `Thomas Haukland` / `Cheapino2`, the Vial serial `vial:f64c2b3c`, the Cheapino v2 Vial keyboard UID `79 ED 40 BB 09 8B C1 9E`, and an embedded Vial definition named `Cheapino2` (8 x 12 duplex matrix, 36 keys, one encoder). The file is 104,448 bytes, the same size as `cheapino_vial.uf2` in tompi's v2.0 release (2024-03-23).
2. The AliExpress listing (item 3256809546963642, store "Wendy's 3C Store") is titled *"HPD V2 Split Keyboard Stand Curved Wired Hot Swap Touchpad Trackball QMK Knob Gaming Keyboard Kit Vial MX Switch"*. Its variants are: Stand white/black 1pc, White/Black kit A/B, White/Black keyboard A/B, Trackball, Touchpad, Touchpad knob, Trackball knob. A review names the variant "White keyboard A, Axis Body: HPD V2 Wired". Specs say **Backlight Type: No backlight**. The listing's "More to love" strip shows the Cheapino V2 kit as a sibling product. A user manual PDF is linked from the listing (ae-pic-a1.aliexpress-media.com/kf/S407a9cff7db14b80801ceb9800fe1a2aV.pdf); not downloaded here.
3. David's vault calls it a "wired HPD V2 dactyl-style split, Vial, Nov 2025" and the firmware folder in this repo was named `dactyl-firmware`. The "Curved" in the listing title and the "Stand" variants (a magnetic tenting bracket) explain the dactyl impression: it is a flat Cheapino v2 PCB in a curved/tented case, not a Dactyl Manuform.

What that means in hardware terms:

| | |
|---|---|
| Keys | **36** (3 x 5 per half + 3 thumbs per half). That is 6 fewer than a Corne, not more. |
| Extras | one rotary encoder (knob) on the **right** half, click is a mappable key; no OLED, no per-key LEDs |
| Controller | one RP2040-Zero, on the left half (the half with USB); halves linked by RJ45 (8P8C) |
| Flashing | hold BOOT on the RP2040-Zero while plugging in USB, drive `RPI-RP2` appears, drag the UF2 |
| Firmware | QMK/Vial, 14 dynamic layers, 16 macros, encoder map, Vial unlock = hold both inner thumbs |
| Editing | Vial desktop app (get.vial.today) or https://vial.rocks in Chrome/Edge (WebHID); both work, desktop is the safer one for loading .vil files |
| USB IDs | VID `0xFEE3`; Vial shows the board as "Thomas Haukland Cheapino2" |
| Source | https://github.com/tompi/cheapino (hardware, docs, releases), https://github.com/tompi/vial-qmk branch `cheapinov2` (the Vial build) |

**What David should still check (two minutes):**

- Plug it in, open Vial, menu *About Cheapino2…*: the name confirms the PCB. If it says something else, stop and send me the name.
- Count the keys: 18 per half including thumbs, knob on the right. If the board has a trackball or touchpad, that is one of the other listing variants and this firmware does not drive it (see §7).
- Pop one keycap: is there an SMD LED at the switch's edge? The Cheapino v2 PCB has none. If there is one, the board is a modified PCB and §6 changes.

The rest of this document assumes the Cheapino v2 as confirmed above.

---

## 2. Physical layout and the design constraint

```
 LEFT (RP2040, USB)                 RIGHT (knob)
 col: 10   9   8   7   6             0   1   2   3   4
 row4 [Q] [W] [E] [R] [T]     row0  [Y] [U] [I] [O] [P]
 row5 [A] [S] [D] [F] [G]     row1  [H] [J] [K] [L] [;]
 row6 [Z] [X] [C] [V] [B]     row2  [N] [M] [,] [.] [/]
          thumbs (6,11)(5,11)(4,11)   (2,5)(1,5)(0,5)  knob click = (3,0)
                 outer  mid  inner    inner mid  outer
```
Legends are the Base-layer letters so the game layers below can say "the physical T key".

The brief for this board assumed it had *more* keys than the Corne, so the extras could take a dedicated Esc, Tab, Alt and a number row. It has fewer: **five columns per half, no far-left column.** On the Corne the far-left column holds Esc / Shift / Ctrl beside W A S D. Here the pinky column is the Q A Z column itself, so "Esc, Shift, Ctrl directly left of the movement keys" is only possible when the movement keys sit one column in, on **E S D F**. That is the ESDF posture David already tested on the Corne (v4.1 GAME_ESDF), and on this board it is not an option but the natural base. The firmware does the translation: the physical E S D F keys send W A S D, every game sees ordinary WASD, nothing is rebound in-game.

Consequences, compared with the Corne Arc layer (`Esc Q W E M Tab / Sft A S D F G / Ctl Z X C V B`, thumbs `R H Spc`):

- Thumbs stay **R, H, Space**. Tab and M get the two best keys that are left: the **inner column, top and home** (physical T and G), an index-finger reach to the right.
- Alt (dodge) takes the inner bottom key (physical B).
- Z, G, B and F have no left-hand key. They go to the right half's bottom row, where the hand is free only occasionally. If F (flashlight) turns out to matter more than Alt, swap them (§3.5).
- The top-row R of the Corne ESDF layer is dropped because the thumb already sends R; that key is Tab instead.

---

## 3. Layers

| # | Layer | Reached by | What it is |
|---|---|---|---|
| 0 | BASE | default | seller QWERTY default, lightly edited |
| 1 | NUM/SYM | hold right inner thumb | symbols left, numpad right |
| 2 | FN/NAV | hold left inner thumb (Tab) | F-keys, arrows, media, lighting, **game selectors**, bootloader |
| 3 | **GAME** | FN + `GAME` | universal game base, ESDF posture sending WASD, left half only |
| 4 | **ARC** | FN + `ARC` | Arc Raiders: GAME plus Arc-only changes (none yet) |
| 5 | GAME_WASD | FN + `WASD` | literal letters under the fingers, Shift/Ctrl on thumbs; a fallback, not recommended |
| 6-13 | unused | | all transparent; room for more games |

Rules for every game layer, carried over from the Corne proposal: every position is bound or `KC_NO`, no transparency (nothing falls through to Base's hold-taps); plain keycodes only, no hold-taps or layer-taps; no GUI and no way to reach FN or NUM/SYM inside a game; everything a game needs is on the left half; the right half is numbers plus the few spill-over letters, and the right inner thumb is the only exit.

### 3.1 BASE (layer 0)

```
 Q    W    E    R    T    |  Y    U    I    O    P
 A    S    D    F    G    |  H    J    K    L    ;
 Z    X    C    V    B    |  N    M    ,    .    /
 GUI  Alt  Ctl  Sft       |       Sft  Ctl  Alt            <- hold the key above
 thumbs: Esc  Space  Tab/FN   |   NUM  Bksp  Enter
 knob: volume, click = mute
```
This is the seller default with three changes: the left inner thumb is **Tab on tap, FN on hold** (was plain Tab, FN was nested inside layer 1), the knob turns volume instead of scrolling (click mute instead of play/pause, like the Corne), and the ' key, which the default puts on layer 1, stays there (see NUM/SYM). The bottom-row hold-mods (GUI/Alt/Ctrl/Shift under Z X C V, Shift/Ctrl/Alt under M , .) are the seller's and are the only Shift on this 36-key base; if they annoy while typing, the alternative is a Shift tap-dance on the outer thumb as on the Corne.

### 3.2 NUM/SYM (layer 1, hold right inner thumb)

```
 !    @    #    $    %    |  7    8    9    -    =
 '    "    (    )    _    |  4    5    6    +    *
 `    ~    [    ]    \    |  1    2    3    0    .
 thumbs: Shift  Space  Tab   |   (held)  Bksp  Enter
 knob: mouse wheel
```
Shift on the left outer thumb while the layer is held gives `{ } |` from `[ ] \` and `< >` from the Base `, .` keys on the next tap. Excel entry works as on the Corne: `7 8 9 - =`, `4 5 6 + *`, `1 2 3 0 .`, `( ) $ % ! @ #` on the left.

### 3.3 FN/NAV (layer 2, hold left inner thumb)

```
 F1   F2   F3   F4   F5   |  Home PgDn PgUp End  Del
 F6   F7   F8   F9   F10  |  ←    ↓    ↑    →    Caps
 F11  F12  PrtSc RGBtog RGBmod | GAME ARC  WASD Ins  BOOT
 thumbs: GUI  Alt  (held)   |   Mute  Play  Next
 knob: onboard LED brightness
```
The three **game selectors** sit on the right bottom row (physical N M ,): `GAME` = `TO(3)`, `ARC` = `TO(4)`, `WASD` = `TO(5)`. Entering a game is hold-FN-then-tap, two deliberate steps on two hands, and FN does not exist inside a game layer, so you cannot switch or re-enter mid-fight. `BOOT` (`QK_BOOT`, physical `/`) drops the board into the UF2 bootloader for reflashing; it is the far pinky corner under a held layer, and the hardware button does the same, so remove it if you would rather not have it.

### 3.4 GAME (layer 3): the universal base

```
 physical:  Q    W    E    R    T    |  Y    U    I    O    P
 GAME:      Esc  Q    W    E    Tab  |  1    2    3    4    5
            Sft  A    S    D    M    |  6    7    8    9    0
            Ctl  X    C    V    Alt  |  F    G    B    Z    ·
 thumbs:    R    H    Space          |  EXIT Enter ·
 knob: volume, click = mute
```
Read it with the fingers on E S D F: middle finger up is **W** (forward), ring/middle/index on the home row are **A S D**, pinky column is **Esc / Shift / Ctrl** (menu, sprint, crouch), ring up is **Q**, index up is **E** (interact). **Tab** (inventory) is the index stretch up-right, **M** (map) the index reach right, **Alt** (dodge) the index reach down-right. Thumbs are **R** (reload), **H** (put away), **Space**, the same three as every Corne game layer, so muscle memory carries across all three boards.

Right half: weapon slots `1-5` on the top row, `6-0` on the home row, the spill-over letters `F G B Z` on the bottom row, pinky corner dead. Right thumbs: inner = **EXIT** (`TO(0)`, back to typing), middle = Enter (chat), outer dead. The exit is a dedicated key under the hand that is on the mouse, far from the left hand, so it cannot be hit mid-fight; a stray palm on the right half hits a number or a dead key, never a layer change. If David wants a chord instead, Vial's Combos tab can add `Esc + <right-thumb key>` → `TO(0)`; Vial combos are global, so the second key must be a keycode that exists only on game layers (for example put `KC_F24` on the right outer thumb and combo `KC_ESC + KC_F24`). Not included in the .vil because QMK combos emitting layer keycodes should be tried on the board first.

### 3.5 ARC (layer 4): Arc Raiders

```
 ARC = GAME, no changes yet.
```
Everything Arc needs that fits on a 5-column half is already in GAME (Esc Q W E Tab / Sft A S D M / Ctl X C V Alt, thumbs R H Space, F G B Z and 1-0 on the right). The layer exists so Arc-only changes can be made without touching GAME; in `make_vil.py` it is `with_changes(GAME, {...})`, a thin overlay in the source that is written out as a full layer because Vial cannot run two layer commands from one key (a `TO(GAME)` + `TG(ARC)` macro is not possible in Vial, and a transparent overlay toggled on its own would fall through to BASE, not GAME).

Inner-column alternatives (pick one; A is in the .vil):

| | inner column top / home / bottom | what moves to the right half |
|---|---|---|
| **A (default)** | Tab, M, Alt | F, G, B, Z |
| B | Tab, M, F | Alt (dodge), G, B, Z |
| C | Tab, F, M | Alt, G, B, Z; M on the bottom key instead of the home key |

### 3.6 GAME_WASD (layer 5): literal WASD, the fallback

```
 Q    W    E    R    Tab  |  1    2    3    4    5
 A    S    D    F    M    |  6    7    8    9    0
 Z    X    C    V    H    |  G    B    T    Alt  Esc
 thumbs: Shift  Ctrl  Space   |  EXIT Enter ·
```
Every letter is under its own cap, so the keycap legends match, and R is where it always was. The price is the thing David asked for: there is no column left of A, so Shift (sprint) and Ctrl (crouch) move to the thumbs, which pushes **H** off the thumbs onto the inner bottom key, and **Esc and Alt move to the right half**. Dodge and the pause menu under the mouse hand are poor, which is why GAME (3.4) is the recommendation and this is a layer to compare against for a session, not to live on.

### 3.7 Adding a game

Copy the `ARC` block in `make_vil.py`, change the keys that differ, append it to `LAYERS` and `LAYER_NAMES`, put a `TO(n)` for it on the FN right bottom row (replace `Ins` on physical `.`), run the script, reload the .vil. Or, in Vial directly: select layer 6, set every key by hand from `key-table.md` (GAME) and change what differs, then edit the FN key.

---

## 4. Diff against the Corne v4.1 game design

| | Corne (ZMK, 42 keys) | this board (Vial, 36 keys) |
|---|---|---|
| Game base | GAME_WASD, far-left column Esc/Sft/Ctl | GAME = ESDF posture; pinky column Esc/Sft/Ctl |
| Tab, M | physical T and R keys | inner column top and home (physical T and G) |
| Alt | G key | inner bottom (physical B) |
| Z G B F | on the left half | right half bottom row |
| Thumbs | R H Space | R H Space |
| Numbers | right half 1-5 / 6-0 | right half 1-5 / 6-0 |
| Exit | right inner thumb `&to 0` + Esc/Esc combo | right inner thumb `TO(0)`; combo optional |
| Entering | Fn + macro `&to 2 &tog 4` | FN + `TO(n)`, overlays flattened at generation time |
| Knob | volume, click mute | volume, click mute (encoder is on the right half) |

---

## 5. How to load this in Vial

1. Plug the board in (left half, USB-C). Open the **Vial desktop app** (https://get.vial.today) or https://vial.rocks in Chrome or Edge. The sidebar should say **Cheapino2**. If Vial asks to unlock, hold the two inner thumb keys (left inner and right inner) until the bar fills.
2. **Back up what is on the board first:** *File > Save current layout*, name it `cheapino_hpd_v2_backup_<date>.vil`, and put it in this folder.
3. *File > Load saved layout*, pick `cheapino_hpd_v2_gaming.vil`. Vial writes every key, encoder and layer immediately; there is no apply step.
4. Check the knob: on BASE it should be volume. If it goes the wrong way, swap the two entries of every `enc=(…)` in `make_vil.py` (the first is counter-clockwise) and reload.
5. Hold the left inner thumb (Tab) and tap the right-half **N** key to enter GAME; fingers on E S D F; the right inner thumb brings BASE back. Test in a game's keybind screen that E S D F register as W A S D and the thumbs as R, H, Space.
6. Anything you change in Vial's UI is live on the board but not in this folder. Either re-save a .vil here, or make the change in `make_vil.py` and regenerate, so the two stay in step.

If a .vil load is refused or garbled, fall back to `key-table.md` and set the keys by hand; the table gives the Vial keycode name per physical key, and hovering a key in Vial shows its (row, col).

The firmware on the board does not change for any of this. The seller UF2 in `known-good/cheapino/` is the rollback: BOOT-button + plug in, drag the UF2 onto `RPI-RP2`.

---

## 6. RGB: what is and is not possible

- **The Cheapino v2 PCB has no switch LEDs and no underglow.** The only LED is the one on the RP2040-Zero module itself (data pin GP16, `RGBLED_NUM 1` in tompi's config). The listing's spec sheet agrees ("Backlight Type: No backlight").
- **The seller firmware does have RGB enabled** (`RGBLIGHT_ENABLE = yes`); the stock layout even has RGB hue/saturation/brightness keys on its layer 3, and the board flashes the module LED through the colours at power-up. So the November 2025 diagnosis ("firmware built without RGB") was wrong in direction: the RGB keycodes work, there is just nothing wired to light except the module LED, which is usually hidden inside the case.
- **To try it:** FN + physical **V** (`RGB_TOG`) and FN + physical **B** (`RGB_MOD`), or turn the knob while FN is held for brightness, and look at the RP2040-Zero through the case opening or with the case off. Vial's *Lighting* tab also controls it.
- **A firmware rebuild would not add switch lighting** because there are no LEDs to drive. Switch RGB would need a different PCB (or hand-wired LED strips on GP16, after which a rebuild with the right `RGBLED_NUM` would be needed). If David's unit turns out to have LEDs under the caps (§1 check), send a photo of the PCB underside and the LED count and the build recipe can be written then: tompi/vial-qmk branch `cheapinov2`, `keyboards/cheapino/config.h`, change `RGBLED_NUM`, `qmk compile -kb cheapino -km vial`.

---

## 7. Open questions for David

1. **Confirm the board:** Vial's *About* dialog says Cheapino2, 18 keys per half, knob on the right? Any trackball or touchpad on your unit? (The listing sells those as variants; this firmware does not know about them.)
2. **Inner column:** Tab / M / Alt (A, default) or Tab / M / F (B)? Which of Alt (dodge) and F (flashlight) do you reach for mid-fight in Arc Raiders?
3. **Do you use Z, G or B in Arc?** They are on the right half now. If one matters, it can replace Alt or F on the left.
4. **Base hold-mods:** keep the seller's hold-GUI/Alt/Ctrl/Shift under Z X C V (and Shift/Ctrl/Alt under M , .), or go to plain letters plus a Shift tap-dance on the outer thumb like the Corne? Plain letters would remove the only Shift on the base layer, so a thumb would have to give.
5. **Exit:** dedicated right inner thumb (as built) or an Esc + right-thumb chord as well?
6. **BOOT key** on FN: keep or drop?
7. **GAME_WASD:** keep layer 5 as a comparison, or delete it to leave the FN row with just GAME and ARC?
8. **Repo housekeeping (not done here, the sibling session owns top-level docs):** `docs/dactyl-firmware/` and `archive/dactyl-firmware/` are byte-identical copies of `known-good/cheapino/cheapino_vial.uf2` and could be removed; `known-good/README.md` says "No source in this repo" for the Cheapino, and could point at tompi/vial-qmk `cheapinov2` and this folder. The vault's "HPD V2 identity" open question can be closed with §1.

---

## 8. Sources

- Seller UF2 analysis: `known-good/cheapino/cheapino_vial.uf2` (UF2 family `0xe48bff56` RP2040, 204 blocks, flash image 52 KB, Vial definition at `0x1000b8b7`, UID at `0x1000b8ae`).
- AliExpress listing 3256809546963642 (read 2026-10-17 in the built-in browser).
- https://github.com/tompi/cheapino: readme, `doc/firmware.md`, `doc/buildguide_v2.md`, release v2.0 assets.
- https://github.com/tompi/vial-qmk branch `cheapinov2`: `keyboards/cheapino/{config.h,rules.mk,info.json,encoder.c}`, `keymaps/vial/{config.h,rules.mk,vial.json,keymap.c}`.
- https://github.com/vial-kb/vial-gui: `protocol/keyboard_comm.py` (`save_layout` / `restore_layout`), `keycodes/keycodes.py` (keycode names); tompi's `miryoku.vil` used as a format reference.
- Corne design: `ZmkConfig/docs/layout-proposal-2026-10.md` v4.1 changelog and §2.
- David's notes: vault `40 Resources/Custom Keyboards Reference.md`, transcript `2025-11-07 RGB not working fix`.
