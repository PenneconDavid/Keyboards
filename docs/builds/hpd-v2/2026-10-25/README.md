# HPD v2 PC/TV v3 build verification

2026-10-25. Approved mapping compiled as `ergohaven/hpd/rev2:pc_tv_v3` on
Ergohaven QMK/Vial release 4.1.0, commit
`98bf33054c73619b590505853d8d8f3e0557f41e`. ARM GCC 13.3.0; QMK MSYS.

`verification.json` records the final output hashes and checks. `build.log` is
the successful incremental compile/link log; the ELF is retained here for audit.
`source-manifest.json` in the output folder records the canonical source hashes.

Validation completed:

- Official Vial GUI protocol-6 parser decoded all 1,344 matrix and 32 encoder
  values. Parser source Git blob hashes are in `parser-provenance.json`; the
  decoded reference is in `vial-decoded-keycodes.json` and includes its input hash.
- Compiled ELF `keymaps` bytes match all decoded matrix values. `encoder_map`
  matches after normalizing Vial's CCW/CW wire order to QMK's CW/CCW storage order.
- Both left-hand M/map keys, retained Tab/Ctrl, translated ESDF movement,
  dedicated arrows/clicks, FN selectors/exit and encoder volume were checked.
- All 342 UF2 blocks have valid headers/trailers, sequencing and RP2040 family ID.
  Flash spans `0x10000000` to `0x10015600` exclusive, below the reserved-storage
  boundary. All 87,380 file-backed ELF flash bytes match the UF2 payload.
- No compiler warnings occurred. Hardware tests and flashing were not performed.

Build setup needed two corrections: `PYTHONUTF8=1` for the vendor's UTF-8 Vial
definition generator on Windows, and the pinned LUFA headers alongside the ARM
libraries. The reproducible build instructions include both. Failed setup logs
remain in the ignored `.build/hpd-v2/` folder; these were not firmware defects.

A present-device enumeration found no match for expected `E126:0051` or the
HPD/Ergohaven device name. This does not exclude a differently identified clone.
Export/identify the actual device before loading or flashing anything.
