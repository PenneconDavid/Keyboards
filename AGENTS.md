# Agent Instructions — Keyboards

## Project

- **Name:** Keyboards
- **Purpose:** Personal QMK userspace for the wired Corne (and archived VIA/Vial binaries)
- **Stack:** QMK firmware, crkbd/Corne rev1 (ATmega32U4 Pro Micro, never `rev4_1`), PowerShell flash script
- **Wireless ZMK:** `C:\Users\dseib\Documents\Projects\ZmkConfig` (do not mix sources here). The current Eyelash Corne layout is `config/eyelash_corne.keymap` on ZmkConfig `main` (keymap v4.1); `keyboards/crkbd/keymaps/keebv2/` mirrors it, so layout changes go to ZMK first, then here.

## Build / flash

- **Compile:** `qmk compile -kb crkbd -km keebv2` inside `~\qmk_firmware` after copying `keyboards/crkbd/keymaps/keebv2/` there (`scripts/flash-keebv2.ps1 -BuildOnly` does both). From a plain PowerShell the QMK MSYS CLI needs `MSYSTEM=MINGW64`, `SHELL=C:\Program Files\QMK_MSYS\usr\bin\bash.exe` and `C:\Program Files\QMK_MSYS\mingw64\bin` on PATH; the script sets them.
- **Flash budget:** the build sits at 99 % of 28 KiB. Check the size line of every compile; trims already applied are listed in the keymap's `rules.mk` / `config.h`.
- **No web dev server** — Tier 2 browser QA skills do not apply.

## Commands

| Task | Command |
|------|---------|
| Compile | `qmk compile -kb crkbd -km keebv2` |
| Flash | `scripts/flash-keebv2.ps1` (ask first; it writes to a connected board) |

No test runner, lint, or web server. Tier 2 browser QA skills do not apply.

## Definition of done

- `qmk compile -kb crkbd -km keebv2` succeeds with no new warnings and still fits.
- Layer or key changes are described in the commit body so they can be verified on the board.
- Never flash without the user asking.

## Shared config

- **Skills:** `.agents/skills/` → [cursor-skills](https://github.com/PenneconDavid/cursor-skills)
- **Rules:** `.cursor/rules/` → [cursor-rules](https://github.com/PenneconDavid/cursor-rules)
- **Connections:** maintain `ConnectionGuide.txt` if documenting USB/serial paths or build targets

## Conventions

- Userspace keymap lives at `keyboards/crkbd/keymaps/keebv2/`. `scripts/flash-keebv2.ps1` copies it into the local QMK tree. There is no second copy of the keymap in the repo.
- Ask before modifying keymaps the user has not requested.
- VIA overrides `keymap.c` after flashing. Latest VIA snapshot: `layouts/via/crkbd.layout.2026-05-10.json` (pre-v4.1, 4 layers). A new dated export belongs in `layouts/via/` after every flash or live edit.
- `known-good/` holds the only copies of recovery binaries; do not duplicate them under `archive/` or `docs/`.
