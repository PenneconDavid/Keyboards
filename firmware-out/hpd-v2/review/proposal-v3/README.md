# HPD v2 — trackball-first everyday PC and gaming

Mapping approved by David on 2026-10-25, including the left-hand map-key correction.
The custom QMK/Vial firmware has compiled and passed static verification; nothing
has been loaded or flashed. [Firmware package and remaining device steps](../../pc-tv-v3/README.md).
Diagrams use dark mode by default.

**Updated 2026-10-25:** added plain left-hand M/map to both gaming layers.
WASD uses the former duplicate-Tab bottom extra (4,3); its ordinary Tab remains.
ESDF uses the former duplicate far-left Ctrl (3,0); Ctrl remains beside it (3,1).
Right-hand M remains available in both. Other key assignments are unchanged.

## Visual proposal

- [Everyday Base — full-size](layer-00.png)
- [Base, WASD and ESDF comparison](everyday-and-gaming.png)
- [Base and all utility layers](everyday-and-utilities.png)
- [All six layers — overview](all-layers.png)
- Individual utility/game images: [NUM/NAV](layer-01.png), [Symbols](layer-02.png),
  [Function / pointer / media](layer-07.png), [WASD](layer-05.png), [ESDF](layer-06.png).

## Confirmed requirements

- Trackball is the primary pointing device; FPS gaming is optional.
- General PC / TV / desk use, primarily browsing and writing; Excel capability.
- Dedicated arrows and Ctrl / Shift / Alt / Windows; numpad-style layer.
- Volume on the knob is preferred. This proposal keeps it in every mode.
- No current Vial export or known installed firmware version yet.

## My proposed placements — for your review

Base keeps the alphabet, number row, and ordinary punctuation block. The right
thumb arc is **Left / Down / Up / Right**, from the inner key toward the outside.
This deliberately preserves the letter and punctuation keys. It is a four-key
strip, **not an inverted-T cluster**; reach and preference are still unverified.

Right bottom extras are always left/right click. Enter replaces the outer right
Shift key (matrix 9,0); left Shift remains dedicated. Backspace stays top-right.
Delete is FN + the physical backslash-position key (matrix 7,0). Windows / Alt /
NUM / Space occupy the left thumb arc, outer to inner. FN and SYM use the two
left bottom extras. Grave/tilde and missing operators are on SYM.

NUM gives a right keypad with arithmetic and Enter, plus left navigation and
clipboard actions. Its digit keys require Num Lock; FN + physical N toggles it.
The right thumb arrows remain active, including for selecting cells/text with
Shift or Ctrl. FN contains F1–F12, media, window commands and pointer modes.

Pointer behavior: keep automatic mouse-layer switching **off** initially, because
clicks are permanently available and writing should not unexpectedly turn home-row
letters into clicks. While FN is held, H gives Sniper, J/K give left/right click,
L gives middle click and semicolon gives Scroll mode. Propose held (non-sticky)
pointer modes. DPI and sensitivity require a physical trial, especially for a TV.
The actual board setting remains unknown; these are intended settings, not proof
that the attached keyboard is configured this way.

The encoder rotates volume on every layer. Push is Mute, or Play/Pause while FN is
held. FN is layer 7, higher than games 5 and 6, so the function menu can be used
from gaming too. FN + physical W enters WASD, FN + physical E enters translated
ESDF, and FN + physical B returns to Base. A dedicated FN key avoids tap/hold
timing for Space and modifiers.

WASD retains normal letters, click extras and arrows. Left thumbs are Alt / Ctrl /
C / Space, giving accessible modifiers/crouch without imposing Corne reload/heal
bindings. These are duplicates of ordinary keys; individual games may rebind them.
Translated ESDF moves the letter block so physical E/S/D/F sends W/A/S/D. T goes
to the second left bottom extra; B/G to left thumbs. M/map is on the far-left
bottom key, and also remains on the right half. WASD puts M/map on its left
bottom extra, beside FN, replacing a redundant Tab assignment.
That is a general game layout, not a guarantee that every game's action can be
operated with only the left hand. For typing/chat, return to Base. Native ESDF
with per-game rebinding is still an option if preferred.

## Approved placements; physical validation still needed

The right-thumb arrow strip, right-pinky Enter, Windows/US QWERTY and translated
ESDF are the accepted starting point. Test their comfort on the actual keyboard;
approval of the diagrams does not establish physical reach or runtime behavior.

## Get the baseline before implementation

1. Connect the HPD normally and open Vial desktop or [Vial Web](https://vial.rocks).
2. Export **File > Save current layout** using desktop Vial. For a browser UI, use
   its layout export/save action; do not load the proposal. Save the backup as
   `firmware-out/hpd-v2/current.vil` in the Keyboards repo.
3. Record the displayed device name, firmware version if exposed, and pointer
   settings available. Do not reset the board to obtain this information.
4. A read-only Windows check on 2026-10-25 found no present device matching the
   expected HPD VID/PID or HPD/Ergohaven display name. That does not exclude a
   differently identified clone and does not identify its installed firmware.

## Firmware implementation plan

Use the upstream Ergohaven HPD rev2 encoder/trackball hardware implementation,
keeping Vial support. Latest release verified by the GitHub API in this chat was
[4.1.0](https://github.com/ergohaven/vial-qmk/releases/tag/4.1.0), source commit
`98bf33054c73619b590505853d8d8f3e0557f41e`. The module-specific keymap is at
`keyboards/ergohaven/hpd/keymaps/v2_enc_ball/`; hardware metadata is under
`keyboards/ergohaven/hpd/rev2/`. This is separate from Corne AVR and ZMK builds.

The approved Vial mapping and pinned custom QMK/Vial build are now in
`firmware-out/hpd-v2/pc-tv-v3/`. Existing EEPROM overrides must be handled
deliberately: new firmware defaults do not prove that its stored mapping changed.
Compile/UF2 verification passed; hardware identification, backup and tests remain
outstanding. No flash is authorized merely by preparing the package.

Research/provenance is retained in [v2 research notes](../proposal-v2/README.md).
The original `.vil` is untouched. `mapping-proposal.json` here is deliberately
not a Vial-import format. Reproduce the images with `python
firmware-out/hpd-v2/review/propose_v3.py` from the repo root; it uses v2's saved
review data and the shared renderer, and checks movement, arrows, clicks, FN
availability and encoder assignments.
