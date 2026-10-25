# HPD v2 mapping review — 2026-10-23

**Latest proposal:** [v3 — trackball-first PC / TV / gaming, 2026-10-24](proposal-v3/README.md).
It incorporates David's dedicated-arrow/modifier/numpad requirements and volume
in every mode. Approved 2026-10-25; images now default to dark mode.
The original Corne-derived draft below is retained for history.

These images render the saved `../hpd_v2_gaming.vil` draft directly. They do not
represent a verified export from the keyboard. Spacing is schematic; the right
half is mirrored into top-view order. Each key includes its Vial row,column
address. The diagrams cover all 60 keys plus the encoder push, and its rotation
assignments. No keymap or firmware was changed to produce this review.

## Images

- [Gaming comparison: WASD, Arc Raiders, ESDF](gaming-overview.png)
- [0 — Base](layer-00.png)
- [1 — Lower / game selectors](layer-01.png)
- [2 — Raise / symbols](layer-02.png)
- [3 — Adjust / pass-through](layer-03.png)
- [4 — Mouse](layer-04.png)
- [5 — Game / WASD](layer-05.png)
- [6 — Arc Raiders](layer-06.png)
- [7 — Game / ESDF](layer-07.png)

Layers 8–15 are unused transparent slots in the saved proposal and are omitted.
PASS means use the active lower layer; OFF means no action. Mouse functions are
labels for the draft's keycodes; firmware support and settings need verification.
Movement highlighting follows the output WASD codes, including on ESDF.

## Feedback prompts

Refer to a layer and its small row,column label when describing a change.

1. **Aiming:** trackball, separate mouse, or both?
2. **Fire / ADS:** keep both right home-row buttons and thumb buttons, or prefer
   one set? The draft uses J/K and the two inner right thumbs.
3. **Game encoder:** weapon-wheel scrolling, volume, or another action? Its push
   currently remains Mute.
4. **Left bottom extras:** M on the outer key and Alt on the inner key in
   WASD/ARC — comfortable, swapped, or different actions?
5. **Left thumbs:** keep Ctrl / R / H / Space from outer to inner? In particular,
   should the extra Ctrl become G, Z, Enter, or another key?
6. **ESDF trial:** keep it? The far-left column sends Tab / M / Alt, and the bottom
   extras send G / Z. Are those the preferred positions?
7. **Entry / exit:** keep Lower plus a right-pinky selector, and the right outer
   bottom-extra key for Exit to Base? Exit is matrix 10,2.
8. **Other game bindings:** any missing actions or changes for Arc Raiders?

## Information needed before applying anything

- Export **File > Save current layout** in Vial. That backup is needed to retain
  the actual Base/Lower/Raise layers and existing macros, combos and tap dances.
  Save it beside the original HPD README as `../current.vil`.
- Record the device name and firmware version shown by Vial, plus whether
  Trackball settings is available. Firmware updating has not been established
  as necessary.
- The original README references a missing `make_vil.py`. It also gives a
  conflicting backup path in its loading instructions. Use the HPD folder above;
  a merge workflow must be restored before loading the proposal.
- Your actual right-side mouse-layer mappings/settings must be checked against
  the gaming proposal. The current draft's mouse home row matches its game
  home row; the proposed left mouse layer is transparent.

The Second Brain's older HPD identity/RGB notes still need reconciliation with
the newer HPD-specific research. This is documentation follow-up, not a keymap
preference you need to decide here.

## Reproduce

Run `python firmware-out/hpd-v2/review/render_review.py` from the Keyboards repo.
Requires Pillow and the Windows Segoe UI fonts. The renderer reads the existing
draft and writes only these PNG review images.
