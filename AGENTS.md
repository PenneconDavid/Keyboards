# Agent Instructions — Keyboards

## Project

- **Name:** Keyboards
- **Purpose:** Personal QMK userspace for wired Corne (and archived VIA/Vial binaries)
- **Stack:** QMK firmware, crkbd/Corne, PowerShell flash scripts
- **Wireless ZMK:** `C:\Users\dseib\Documents\Projects\ZmkConfig` (do not mix sources here)

## Build / flash

- **Compile:** `qmk compile -kb crkbd -km keebv2` (keymap at `keyboards/crkbd/keymaps/keebv2/`; script copies into `qmk_firmware`)
- **No web dev server** — Tier 2 browser QA skills do not apply.

## Commands

| Task | Command |
|------|---------|
| Compile | `qmk compile -kb crkbd -km keebv2` |
| Flash | `scripts/flash-keebv2.ps1` (ask first; it writes to a connected board) |

No test runner, lint, or web server. Tier 2 browser QA skills do not apply.

## Definition of done

- `qmk compile -kb crkbd -km keebv2` succeeds with no new warnings.
- Layer or key changes are described in the commit body so they can be verified on the board.
- Never flash without the user asking.

## Shared config

- **Skills:** `.agents/skills/` → [cursor-skills](https://github.com/PenneconDavid/cursor-skills)
- **Rules:** `.cursor/rules/` → [cursor-rules](https://github.com/PenneconDavid/cursor-rules)
- **Connections:** maintain `ConnectionGuide.txt` if documenting USB/serial paths or build targets

## Conventions

- Userspace keymap lives at `keyboards/crkbd/keymaps/keebv2/`. `scripts/flash-keebv2.ps1` copies it into the local QMK tree.
- Ask before modifying keymaps the user has not requested.
- Live VIA snapshot: `layouts/via/crkbd.layout.2026-05-10.json` (wired Corne only).
