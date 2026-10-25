# HPD v2 — everyday PC / TV / gaming proposal

Discussion draft, 2026-10-24. Supersedes the **design intent** of the earlier
Corne-derived gaming proposal; does not modify its files or the connected board.
User answers are pending. This is a first pass to discuss, not a finalized mapping
or firmware build. `mapping-proposal.json` is review data, not a Vial import file.

## Review images

- [Everyday + WASD + ESDF](everyday-and-gaming.png)
- [0 Everyday](layer-00.png)
- [1 Navigation and numpad](layer-01.png)
- [2 Symbols](layer-02.png)
- [3 Function and media](layer-03.png)
- [4 Pointer](layer-04.png)
- [5 WASD gaming](layer-05.png)
- [6 ESDF gaming](layer-06.png)

Images show output codes and matrix addresses. Thumb ordering follows the previous
photo-derived matrix interpretation; physical positions must be checked on the
board before applying a layout. Schematic spacing does not represent exact reach.

## New design brief

The keyboard is a general-purpose Windows PC / gaming input device at a TV or
desk. Use the HPD's number row and additional keys. Retain an optional physical
WASD / ESDF choice without requiring Corne parity. The following choices are my
provisional recommendations, not user-approved preferences or community consensus.

- Everyday Base retains QWERTY, the number row, Tab, Shift, Ctrl, Alt, and Windows.
  Space is a plain key; layer access has dedicated controls. Editing gets three
  right thumbs, while the two right bottom extras are permanent left/right clicks.
- Navigation gives left inverted-T arrows and a right numeric keypad. This is a
  candidate placement, not a requirement inherited from the Corne. Number row is
  still available directly. Keypad Num Lock is on FN + physical N.
- Symbols include the missing minus/equal/brackets and paired delimiters. Function
  holds F1–F12, media, window controls and game selection. Base volume/mute stays
  on the encoder; NAV/Pointer use wheel scrolling; FN changes media tracks.
- WASD keeps the full alphabet, including R and T in their ordinary positions.
  The extra left keys are M and Tab; thumbs are Alt / C / V / Space. These are
  sample useful game actions, not a commitment to any game's bindings.
- ESDF moves the left block so physical E/S/D/F sends W/A/S/D. Additional left
  slots retain displaced letters. This translation changes chat typing, so return
  to Base for ordinary text. Native ESDF game bindings remain an alternative.
- Game right halves retain typing keys. Right inner thumbs give left/right click
  for trackball use; a separate mouse also works without a different profile.
  The right bottom extras are Sniper and Exit to Base. Encoder rotation scrolls.
- No separate Arc Raiders layer is proposed until the games/bindings are known.
  No tap dances, home-row mods, timed combos or one-key action sequences are
  required by this first pass.

## Feedback needed

1. **Pointer / setting:** trackball as primary pointer? Separate mouse for shooters?
   Both halves always connected? Desk, lap, armrests or another TV placement?
2. **Normal use:** priorities among browsing, coding, writing, Excel and media;
   dedicated arrows versus NAV; number row versus numpad; modifier preferences.
   Confirm Windows and US QWERTY (assumed here).
3. **Games:** titles and actions that matter; native ESDF bindings versus physical
   ESDF sending WASD; preferred thumb actions, wheel actions and push action.
4. **Reach:** which thumb/extras are comfortable? How do you roll the ball (thumb,
   index or multiple fingers)? Any accidental presses, discomfort or awkward keys?
5. **Existing board:** current Vial export and installed firmware/device name.
   A backup is still needed before replacing existing stored customizations.

## Research and firmware findings

Sources checked 2026-10-24:

- [Ergohaven HPD product/layout guidance](https://eh.industries/hpd) separates
  Base, numbers/navigation/media, symbols and cursor controls. It supports the
  general layer organization; it does not establish a universal best keymap.
- [Hardware project](https://github.com/ergohaven/hpd) documents 60 programmable
  keys, RP2040 Zero, QMK/Vial and interchangeable modules.
- [Actual encoder/trackball default keymap](https://github.com/ergohaven/vial-qmk/blob/ergohaven/keyboards/ergohaven/hpd/keymaps/v2_enc_ball/keymap.c)
  provides number row, navigation/numpad, symbols, automatic mouse layer and
  encoder mappings. The older README incorrectly places this keymap under rev2;
  the keymap is under `hpd/keymaps/`, while hardware metadata is under `hpd/rev2/`.
- [Encoder/trackball config](https://github.com/ergohaven/vial-qmk/blob/ergohaven/keyboards/ergohaven/hpd/keymaps/v2_enc_ball/config.h)
  establishes mouse layer 4, right pointing device, encoder pins and Vial UID.
- [QMK layer documentation](https://docs.qmk.fm/feature_layers) and
  [mouse-layer handling](https://docs.qmk.fm/features/pointing_device) explain layer
  priority and explicit control of auto-mouse during other modes. A lower numbered
  layer 4 does not override nontransparent game layers 5/6. The old proposal's
  explanation that the auto layer necessarily replaces game WASD keys was too
  broad. NAV/SYM/FN can still be affected unless the firmware manages pointer mode.
- [Official Vial instructions](https://docs.eh.industries/software/qmk/vial/)
  document immediate remapping and exporting the current layout.
- [Official firmware page](https://docs.eh.industries/firmware/) currently links
  4.0.6. The **live GitHub release API**, checked via the GitHub connector, reports
  [4.1.0](https://github.com/ergohaven/vial-qmk/releases/tag/4.1.0) as latest,
  published 2026-10-15. Release commit:
  `98bf33054c73619b590505853d8d8f3e0557f41e`.
  Asset: `4.1.0_hpd_v2_enc_ball.uf2`; published SHA-256:
  `b52613cca2eb2ef1c318c875a21e25d86cd5e819f32e1188171bdefba585fb07`.
  The asset was **not downloaded or verified locally** during this proposal work.
- Community searches found personal preferences, not sufficient evidence for a
  consensus mapping. For example, [owner feedback](https://t.me/s/ergohaven_reviews?before=267)
  includes index-finger ball use, and [another owner's account](https://t.me/s/ergohaven_reviews?before=404)
  favors an always-available click over relying on auto-layer behavior. These
  anecdotes motivate asking about reach and pointer habits, not prescribing them.

## Firmware path after layout feedback

1. Verify actual board/device/module variant and export current layout/settings.
2. Finalize all layer and pointer behavior from feedback; regenerate review images.
3. Use the matched Ergohaven firmware as the hardware base, keeping Vial editing.
   Produce a Vial layout if configuration suffices; build a pinned custom QMK/Vial
   keymap if requested behavior needs firmware hooks or baked-in defaults.
4. If custom firmware is built, define auto-mouse behavior explicitly for typing,
   NAV/SYM/FN and gaming, and compile the encoder/trackball target. Do not reuse
   the Corne toolchain/flash script or Cheapino firmware.
5. Verify build, UF2 identity/bounds and files, then perform board validation only
   after explicit flashing authorization. No hardware write is authorized here.

Candidate behavior: pointer mode should not obscure navigation, symbols or game
selectors; gaming should use explicit stable click locations. Exact hooks and
setting persistence must be checked against the pinned source before implementation.
