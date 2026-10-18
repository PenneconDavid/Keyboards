# Ergohaven High Plains Drifter v2 ("HPD V2" on AliExpress): identification

2026-10-17. Identification write-up only; the gaming layout follows once David confirms the checks in §4 and answers §5. The earlier Cheapino identification in `../cheapino-hpd-v2/` was wrong for this board and is marked superseded.

## 1. What the board is

**Ergohaven High Plains Drifter v2 (HPD v2)**: an open-hardware (CC BY-NC-SA), 3D-printed, curved split keyboard by Ergohaven (Yurii Q), with interchangeable side modules. The AliExpress listing (item 3256809546963642, "Wendy's 3C Store", title *"HPD V2 Split Keyboard Stand Curved Wired Hot Swap Touchpad Trackball QMK Knob Gaming Keyboard Kit Vial MX Switch"*) is a third-party print of the open design; the listing's variants (keyboard A/B, kit A/B, Trackball, Touchpad, Trackball knob, Touchpad knob, Stand) are the HPD's module and case options. Several AliExpress "wiki" pages for the same item call it "Ergohaven HPD V2".

| | |
|---|---|
| Keys | **60**: each half is 4 rows x 6 columns (24) plus a 6-key thumb arc. Matches David's count. |
| Modules | left half: encoder (knob) with one SK6812 layer-indicator LED; right half: 38 mm trackball (PMW3389 sensor). Matches David's "knob left, trackball right". Other modules exist (touchpad, joystick, second encoder) but need different firmware. |
| Controller | **two** RP2040-Zero, one per half; halves linked by USB-C (serial), either half can be the USB side (hand detected on a pin) |
| Switches | MX hot-swap, 1N4148 diodes, COL2ROW |
| Firmware | Ergohaven QMK/Vial fork, build **`hpd_v2_enc_ball`** ("encoder/trackball"), 16 dynamic layers, 109 macros, tap dance, combos, key overrides, caps word, auto shift, encoder map, pointing device with Ergohaven's sniper/scroll/text modes and an **auto mouse layer (layer 4 by default)** |
| USB IDs | VID `0xE126`, PID `0x0051`; product string `HPD v2 enc/ball`; Vial UID `74 2A FA 6B EB 6E 5B 17` (same in every release since at least 3.8.x) |
| Matrix | 12 rows x 7 cols: left keys rows 0-4, right keys rows 6-10, encoder click left = (5,6), right = (11,6) |
| Flashing | double-tap Reset on the underside (or BOOT while plugging in), drive `RPI-RP2` appears, drag the UF2; **repeat for the other half**, both halves run the same file |
| Editing | Vial desktop app or https://vial.rocks (Ergohaven also ships its own "Entropy" configurator; Vial is the documented path for QMK boards) |
| Sources | hardware https://github.com/ergohaven/hpd (README, build guides, STLs; PCB on oshwlab.com/yuriiq/hpdv2); firmware source https://github.com/ergohaven/vial-qmk (branch `ergohaven`, `keyboards/ergohaven/hpd/rev2`, keymap `v2_enc_ball`); prebuilt UF2s on that repo's releases (latest **4.1.0, 2026-10-15**, file `4.1.0_hpd_v2_enc_ball.uf2`); docs https://docs.eh.industries (device page archived at `device/archive/hpd`, firmware page `/firmware/`) |

Why the first pass went wrong: the only Vial UF2 in this repo (`known-good/cheapino/cheapino_vial.uf2`, also under `docs/dactyl-firmware/` and `archive/dactyl-firmware/`) really is Cheapino v2 firmware (its USB strings, UID and embedded Vial definition say so), and the AliExpress page blocked the first fetch, so the firmware file was taken as the board. That file cannot run David's HPD (different matrix, no pointing device, no split serial). **Where it came from is now an open question** (§5).

## 2. Physical layout (Ergohaven stock labels, from `keyboard.json` and the docs image)

```
 LEFT (knob, right of the thumb arc)                 RIGHT (trackball, left of the thumb arc)
 col: 0     1    2    3    4    5                     5    4    3    2    1    0
 r0  [`]   [1]  [2]  [3]  [4]  [5]              r6   [6]  [7]  [8]  [9]  [0]  [Bksp]
 r1  [Esc] [Q]  [W]  [E]  [R]  [T]              r7   [Y]  [U]  [I]  [O]  [P]  [\]
 r2  [Tab] [A]  [S]  [D]  [F]  [G]              r8   [H]  [J]  [K]  [L]  [;]  [']
 r3  [Sft] [Z]  [X]  [C]  [V]  [B]              r9   [N]  [M]  [,]  [.]  [/]  [Sft]
 thumb arc, outer -> inner:                           thumb arc, inner -> outer:
     (4,2) PgDn  (4,3) PgUp  (4,4) Ctrl                 (10,1) Enter  (10,0) GUI  (10,5) Raise
     (4,5) Lower (4,0) Alt   (4,1) Space                (10,4) Ctrl   (10,3) [    (10,2) ]
 encoder click (5,6) = Mute          encoder: CCW Vol-, CW Vol+
```
The thumb arc runs under the W, E, R, T columns and then sweeps down and inward; the inner-most key (Space on the left, Enter on the right) is the big one next to the module. Stock layers: 0 Base, 1 Lower (F-keys, nav, numpad), 2 Raise (symbols), 3 Adjust (empty), **4 = auto mouse layer** with mouse buttons on the home row of **both** halves (left: `Scroll Btn3 Btn2 Btn1 Sniper` on A S D F G).

Two firmware facts that shape the gaming design:

1. **The auto mouse layer.** With the stock setting, any trackball movement switches layer 4 on for about 750 ms, and layer 4 turns the left home row A S D F G into mouse buttons. In a game that would kill WASD the moment the ball is touched. Fix options, in order of preference: (a) keep auto mouse on for typing but make layer 4's **left half fully transparent** and put its mouse buttons only on the right half (works on every layer, no settings change); (b) turn it off in Vial's QMK Settings (Auto mouse tab) or the Layouts tab; (c) point it at a different layer per Vial setting. The proposal will do (a) and keep (b) as a one-click fallback.
2. **Modules are compile-time.** Volume on the knob, trackball DPI, sniper/scroll modes etc. are firmware features; the layout only maps keys. If David ever swaps the trackball for the touchpad, both halves need the `enc_touch` UF2.

## 3. Firmware David is probably running

Unknown. Ergohaven publishes a release roughly monthly; 4.1.0 is current. A seller print from Nov 2025 would have shipped whatever the seller flashed, likely a 3.8.x or 4.0.x build, possibly the plain `hpd_v2` (no modules) build if the knob or trackball did nothing out of the box. The Vial UID and matrix have been stable across releases, so a `.vil` made now loads on any of them; Ergohaven-specific keycodes (sniper, scroll, text mode, layer next/prev) and the Trackball settings tab appear in Vial only on module builds.

Because every release carries the same UID, the **safe route is to update both halves to `4.1.0_hpd_v2_enc_ball.uf2` first**, then load the layout. Save the current layout from Vial before flashing.

## 4. What David should check or send (in this order)

1. **A top-down photo of both halves**, keycaps on, knob and trackball visible, plus one of the underside of either half if the reset button and module edge connector are easy to catch.
2. **Vial sidebar name / About dialog.** Expected: *Ergohaven HPD v2 enc/ball* (or *HPD v2* if the plain build is on it). The About dialog also shows the firmware version; please send it.
3. **Windows Device Manager → the keyboard's Hardware Ids.** Expected `VID_E126&PID_0051`. If the VID is `FEE3`, it is not Ergohaven firmware and we need to know what it is.
4. **Do the knob and trackball work today?** Volume on the knob, cursor from the ball, and does the left home row ever "go dead" or click things right after moving the ball (that is the auto mouse layer).
5. **Does Vial show a "Trackball settings" group under Layouts and an "Auto mouse" tab under QMK Settings?** Yes means a module build.

## 5. Open questions for David (answers shape the layout)

1. **Mouse in games:** with the trackball under the right thumb, do you still use a separate mouse for games? If yes, the right half stays "numbers plus a few spill-over keys" as on the Corne. If no, the right half becomes the aim/fire side: mouse buttons on the right thumb arc, sniper/scroll modes, and the auto mouse layer has to be designed around rather than suppressed.
2. **Knob in games:** volume (same as Base), or weapon-slot scroll (knob sends `1`..`0` is not possible, but it can send mouse wheel, which most shooters use to cycle weapons), or layer-cycle for the per-game overlays?
3. Where did `cheapino_vial.uf2` come from? Is there a second Vial board in the house, or did the seller send the wrong file? (It is the only "seller firmware" in this repo and it is not for the HPD.)
4. Everything from the Cheapino draft still applies in spirit and will be re-asked against the real geometry: inner-column choices, exit key vs chord, bootloader key on FN.

## 6. What the gaming layout will look like (preview, not final)

The HPD has what the Corne lacked: a number row and a sixth column, so the Corne's Arc layer fits the left half **with room to spare**, in the WASD posture, no ESDF translation needed:

```
 GAME base, left half (sketch)             right half (sketch, if a separate mouse is used)
 `     1    2    3    4    5     | knob    6    7    8    9    0   Esc
 Esc   Q    W    E    R    Tab   |         ·    ·    ·    ·    ·   ·
 Sft   A    S    D    F    G     |         ·    ·    ·    ·    ·   ·
 Ctl   Z    X    C    V    B     |         ·    ·    ·    ·    ·   ·
 thumbs: M  Alt  Ctl  H  R  Space |         EXIT  Enter  ·  ·  ·  ·
```
Dedicated Esc and Tab, a real number row for slots 1-5 under the left hand (6-0 on the right), Alt and a second Ctrl on the thumb arc, R/H/Space on the three big thumb keys nearest the knob, M on the outer thumb. An ESDF variant shifts the block one column right and uses the Esc/Tab/Shift/Ctrl column that is already there. Entry from FN, exit on the right inner thumb (and optionally a chord). This will be written up with a `.vil`, generated from a script as in the Cheapino folder, once §4/§5 are answered.
