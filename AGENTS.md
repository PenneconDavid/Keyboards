# Agent Instructions — Keyboards

## Project

- **Name:** Keyboards
- **Purpose:** Personal QMK firmware staging area for Corne split keyboard layouts
- **Stack:** QMK firmware, crkbd/Corne, PowerShell flash scripts

## Build / flash

- **Compile:** `qmk compile -kb crkbd -km keebv2` (after copying keymap into `qmk_firmware`)
- **No web dev server** — Tier 2 browser QA skills do not apply.

## Shared config

- **Skills:** `.agents/skills/` → [cursor-skills](https://github.com/PenneconDavid/cursor-skills)
- **Rules:** `.cursor/rules/` → [cursor-rules](https://github.com/PenneconDavid/cursor-rules)
- **Connections:** maintain `ConnectionGuide.txt` if documenting USB/serial paths or build targets

## Conventions

- Copy keymap into local QMK tree before compile — do not assume in-repo paths match QMK install.
- Ask before modifying keymaps the user has not requested.
