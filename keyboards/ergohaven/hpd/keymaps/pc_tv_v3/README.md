# HPD v2 PC/TV v3

Approved mapping: 2026-10-25. Target: `ergohaven/hpd/rev2:pc_tv_v3`.

This keymap is for the **left encoder / right PMW3389 trackball** HPD v2 family.
It uses Ergohaven QMK/Vial 4.1.0 at
`98bf33054c73619b590505853d8d8f3e0557f41e`. The five supporting config/definition
files are copied verbatim from upstream `hpd/keymaps/v2_enc_ball`; `keymap.c` is
generated from the approved review data.

Build only from the Keyboards repo:

```powershell
scripts/build-hpd-v2.ps1
```

This requires the pinned upstream checkout and libraries under `.build/hpd-v2/`.
It does not flash. See `firmware-out/hpd-v2/pc-tv-v3/README.md` for outputs and the
remaining device verification and backup steps.

Bindings: Base 0, number/navigation 1, symbols 2, WASD 5, translated ESDF 6,
function/pointer/media 7. Other layers are unused. Volume rotation stays on every
layer. Both game layers have left-hand M/map. All modes retain dedicated arrows
and click extras. FN + physical W/E selects WASD/ESDF; FN + physical B returns Base.

At startup, the user hook turns automatic mouse switching and sticky pointer
modes off, and starts pointer mode at Normal. These two preferences are enforced
again at reboot; Vial changes to them last only until then. DPI, sensitivity and
orientation are preserved. The vendor setters write only changed values.

Dynamic Vial EEPROM mappings survive a firmware flash. The new compiled defaults
alone therefore do not establish that the running keyboard has this mapping.
Back up the device and explicitly load the paired `.vil` after identifying it.
No boot-time layout reset or broad settings erase is included.

Regenerate bindings with `python scripts/generate-hpd-v2.py`; regeneration does
not access a device. Review imagery is under
`firmware-out/hpd-v2/review/proposal-v3/`, using dark mode by default.
