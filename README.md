# Keyboards

Personal QMK userspace for wired boards, plus archived binaries for other keyboards. Wireless ZMK (Eyelash Corne, Sofle, Prospector) lives in `C:\Users\dseib\Documents\Projects\ZmkConfig`.

## Layout
- `keyboards/crkbd/keymaps/keebv2/`: active wired Corne keymap (QMK External Userspace path).
- `qmk.json`: userspace build target `crkbd:keebv2`.
- `layouts/via/`: VIA exports. `crkbd.layout.2026-05-10.json` is the live EEPROM snapshot; `crkbd.layout.json` is older.
- `known-good/`: recovery binaries (wired Corne hex, Sofle ZMK UF2s, Cheapino Vial UF2).
- `crkbd/`: vendored upstream board snapshot (reference only; compile uses your QMK install).
- `firmware/qmk/keymaps/keebv2/`: copy of keebv2 kept so old docs still resolve.
- `archive/`: historical sources.

## Using keebv2
1. Install the QMK CLI and run `qmk setup` once.
2. Point QMK at this repo: `qmk config user.overlay_dir="$(realpath .)"` (or the Windows equivalent path).
3. `qmk compile -kb crkbd -km keebv2` or `qmk userspace-compile`.
4. Optional: `scripts/flash-keebv2.ps1` copies the keymap into `~\qmk_firmware` then flashes Pro Micro halves (defaults COM5 / COM6).

After flashing, remap in VIA. Save a new dated JSON into `layouts/via/` before experiments.

## Wireless
Do not mix ZMK sources into this repo. Eyelash + Prospector: `ZmkConfig/profiles/prospector-eyelash/`.
