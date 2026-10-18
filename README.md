# Keyboards

Personal QMK userspace for the wired Corne, plus recovery binaries for other keyboards. Wireless
ZMK (Eyelash Corne, Sofle) lives in `C:\Users\dseib\Documents\Projects\ZmkConfig`; the current
Eyelash layout is `config/eyelash_corne.keymap` on its `main` branch (keymap v4.1, 2026-10-17).

## Layout
- `keyboards/crkbd/keymaps/keebv2/`: the wired Corne keymap (QMK External Userspace path). Mirrors
  the ZMK v4.1 layout; see its README for layers, VIA and OLED notes.
- `qmk.json`: userspace build target `crkbd:keebv2`.
- `layouts/via/`: VIA exports. `crkbd.layout.2026-05-10.json` is the last live snapshot before the
  v4.1 port (old 4-layer map); `crkbd.layout.json` is older still. Save a new dated file after
  flashing and after every live VIA edit.
- `known-good/`: recovery binaries (wired Corne hex, Sofle ZMK UF2s, Cheapino Vial UF2). The only
  copies in the repo.
- `crkbd/`: vendored upstream board snapshot (reference only; compile uses your QMK install).
- `archive/crkbd_qmk_msys/`: the pre-keebv2 keymap source and the rev1 `keyboard.json` it was
  built with.
- `scripts/flash-keebv2.ps1`: copy, compile, flash.
- `context/`: hardware identification notes.

## Using keebv2
1. Install QMK MSYS (or `pip install qmk`) and run `qmk setup` once (`~\qmk_firmware`).
2. `.\scripts\flash-keebv2.ps1 -BuildOnly` copies the keymap into `~\qmk_firmware` and compiles,
   or do it by hand: copy `keyboards\crkbd\keymaps\keebv2` there and run
   `qmk compile -kb crkbd -km keebv2`. (From a plain PowerShell the QMK MSYS CLI needs
   `MSYSTEM=MINGW64`, `SHELL` and its `mingw64\bin` on PATH; the script sets them.)
3. Flash: `.\scripts\flash-keebv2.ps1` (asks before each half, auto-detects the Caterina COM port,
   `-LeftPort` / `-RightPort` to override), or QMK Toolbox with `~\qmk_firmware\crkbd_rev1_keebv2.hex`.
   The same hex goes on both halves. The board is `crkbd/rev1` (ATmega32U4), never `rev4_1`.
4. After flashing, export the live layout from VIA into `layouts/via/` before any experiments.

## Wireless
Do not mix ZMK sources into this repo. Eyelash Corne rollback UF2s are in
`ZmkConfig/known-good/eyelash-corne/`; Sofle rollback UF2s are in `known-good/sofle-dongle/` here.
