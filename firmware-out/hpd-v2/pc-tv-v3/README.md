# HPD v2 PC / TV / gaming — approved v3

Mapping approved 2026-10-25. **Device identification, a current-layout backup, and
hardware testing remain outstanding. No hardware has been flashed.**

Build status: **passed**. Zero compiler warnings; all 1,344 matrix and 32 encoder
defaults match the parsed Vial mapping. UF2 structure, RP2040 family, flash bounds
and ELF load-segment contents passed verification. See the manifest and audit
report for hashes and exact results.

## Files

- `hpd_v2_pc_tv_v3.uf2`: custom firmware for the RP2040 HPD v2 encoder/trackball
  variant. Only use after confirming the actual device matches that target.
- `hpd_v2_pc_tv_v3.vil`: the corresponding 16-layer Vial import. It replaces the
  mapped layers and encoder assignments; back up all existing customizations first.
- `source-manifest.json`: pinned upstream revision, source hashes and build status.
- `SHA256SUMS`: final UF2 and Vial file hashes, written after successful verification.

Consult the manifest for actual compilation/verification status. A file listed
here is not proof the build succeeded; the verification report is under
`docs/builds/hpd-v2/2026-10-25/verification.json` in the Keyboards repo.

## Mapping

[Dark all-layer image](../review/proposal-v3/all-layers.png) ·
[Large Base/WASD/ESDF comparison](../review/proposal-v3/everyday-and-gaming.png) ·
[Full mapping explanation](../review/proposal-v3/README.md)

- Base: QWERTY, number row, dedicated Ctrl/Shift/Alt/Win, right-thumb arrows,
  permanent left/right clicks on the right extras. Enter is the right pinky key.
- Hold NUM: right numpad and left navigation/editing. Num Lock is FN + physical N.
- Hold SYM: punctuation, brackets and operators.
- Hold FN: F1–F12, pointer modes and utilities. FN + physical W = WASD;
  FN + physical E = translated ESDF; FN + physical B = Base.
- Game map key: WASD left extra 4,3; ESDF far-left bottom key 3,0. Right M remains.
- Encoder: volume down/up on every layer; push = Mute, or Play/Pause with FN.
- Automatic mouse switching and sticky modes are disabled at startup. Pointer
  precision/scroll actions are held. DPI, sensitivity and orientation are preserved.
  Existing Vial Auto Shift settings should be off for predictable held game keys.

## Next device steps

1. Connect normally, open Vial, and record the displayed device name and firmware
   version if available. Expected hardware family: HPD v2 enc/ball; expected VID/PID
   `E126:0051`. A clone may differ, so record differences rather than assuming.
2. **File > Save current layout** in desktop Vial. Save a dated original export in
   `firmware-out/hpd-v2/` and retain it as rollback. The current export is not yet
   available. Record pointer settings, especially DPI, orientation, sensitivity,
   sticky mode and auto-mouse, separately as they may not all travel in a `.vil`.
3. Only after identification and backup, choose whether to apply a Vial mapping
   to compatible existing firmware or use the custom build. Compiling does not
   authorize flashing; no automatic flash script is included.
4. If a firmware update is chosen, follow the matched HPD process for both halves.
   Do not use the Corne HEX/flash script, Cheapino UF2 or a settings-reset image.
5. Existing Vial mappings normally persist through flashing. Explicitly load the
   paired `.vil` after the backup; loading writes immediately. Do not use broad
   EEPROM reset merely to make new defaults appear.
6. Check every position in Vial's matrix tester, typing, both mouse clicks,
   dragging, dedicated arrows with Ctrl/Shift, number layer/Num Lock, all FN
   selectors/exits, both left-hand M keys, knob volume/mute, held pointer modes,
   suspend/wake and power reconnect. Test FPS holds and jump only after normal
   typing and pointing work. Export a new dated live layout afterward.

## Rebuild

The build uses QMK MSYS, ARM GCC 13.3.0 and Python UTF-8 mode. Initial preparation
for a missing isolated checkout (run from this repo) is:

```powershell
git clone --depth 1 --branch 4.1.0 --filter=blob:none https://github.com/ergohaven/vial-qmk.git .build/hpd-v2/vial-qmk
git -C .build/hpd-v2/vial-qmk submodule update --init --depth 1 lib/chibios lib/chibios-contrib lib/pico-sdk lib/printf lib/lufa
scripts/build-hpd-v2.ps1
```

The build script checks source revision
`98bf33054c73619b590505853d8d8f3e0557f41e`, generates canonical sources and the
import, compiles, compares the ELF default arrays with independently decoded Vial
codes, and checks UF2 structure, family, flash bounds and ELF load-segment bytes.
If the mapping changes, the independently decoded Vial reference must be refreshed
before verification can pass. Rebuilding does not flash or access USB devices.
