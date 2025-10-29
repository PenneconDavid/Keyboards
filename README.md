# Keyboards

Personal staging area for split keyboard firmware, layouts, and flashing notes.

## Repository Layout
- `docs/`: reference material (layer diagrams, wiring photos, build notes).
- `layouts/via/`: raw layout exports from VIA or other tools for archival.
- `firmware/qmk/keymaps/keebv2/`: active Corne keymap ready to drop into `qmk_firmware`.
- `crkbd/`: untouched upstream board definition (pulled from the public QMK repo).
- `archive/`: historical builds and configs that are no longer flashed but may contain reusable snippets.

## Using the `keebv2` Keymap
1. Install the QMK CLI (`python -m pip install qmk` or follow <https://docs.qmk.fm/#/newbs_getting_started>). Run `qmk setup` once to clone the firmware tree.
2. Copy the contents of `firmware/qmk/keymaps/keebv2/` into `qmk_firmware/keyboards/crkbd/keymaps/keebv2/` (create the folder if it does not exist).
3. From the QMK repo root: `qmk compile -kb crkbd -km keebv2`
4. Flash each half with `qmk flash -kb crkbd -km keebv2`. For Pro Micros, specify the bootloader when needed, e.g. `-bl avrdude -c avr109 -p ATmega32U4 -P COM5`.
5. After flashing, use `qmk console` to confirm both halves connect, RGB syncs, and no unexpected debug spam appears.

## Flashing Best Practices
- Put the keyboard into reset using the reset switch or `QK_BOOT` keycode; avoid power-cycling USB mid-flash.
- Flash the left (master) half first, then the right; confirm OLED orientation and layer sync before disconnecting.
- Keep `qmk_firmware` up to date (`git pull upstream master`) so you receive split keyboard fixes and RGB/OLED improvements.
- Store every VIA export you care about in `layouts/` before experimenting, so you can diff changes against version control.
- When experimenting with new effects or hardware, branch the repo (e.g. `git checkout -b feature/oled-experiments`) to keep shipping firmware pristine.
