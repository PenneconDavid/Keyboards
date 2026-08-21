# Firmware repo assessment

## 2026-08-21 — Hardware identity (user-confirmed)

Naming: seller “eyelash” covers both boards. User’s **primary** is a **wireless 42-key Corne-like** (knob left, 5-way right, dual nice!view). The **larger** split is a **Sofle** (~58 keys), also knob + 5-way.

| Board | Role | Screens | Extras | Dongle today | Remap / keymap |
|---|---|---|---|---|---|
| Wireless 42-key Corne | Daily driver | nice!view both halves (not backlit) | Left encoder = volume; right 5-way = arrows + Enter; switch RGB works | USB-C stick, **square** display, button + switch on back (not Prospector) | Screenshot 42-key layout; same layout if Prospector becomes central |
| Wired Corne | Backup | OLED (QMK keebv2) | No wireless dongle | n/a (TRRS) | Same 42-key screenshot / May 2026 VIA JSON |
| Sofle (~58) | Separate keyboard | (not re-confirmed this turn) | Knob + directional | Own dongle in vendor packs | **Do not** copy the 42-key Corne map |
| Prospector | Unused; intended Corne dongle replacement | Round color LCD (photo pending) | XIAO BLE | Would replace the square-display stick | Same Corne keymap, halves stay peripherals |

MCU on the wireless Corne still unknown (nice!nano vs soldered). Photos promised: Corne underside, current dongle, Prospector. Until then, treat vendor **dongle-niceview** pack as rollback (OLED on the **stick**, nice!view on the **halves**).

## 2026-08-21 — Dongle-central confirmed; Prospector profile added

Daily-driver Eyelash **is** dongle-as-central with both halves peripheral (vendor nice!nano-class receiver, not Prospector). ZmkConfig already modeled that; the open question was which vendor pack is on the live boards.

Implemented (no commit):

- Keyboards: QMK userspace path `keyboards/crkbd/keymaps/keebv2/`, `qmk.json`, VIA `layouts/via/crkbd.layout.2026-05-10.json`, `known-good/`.
- ZmkConfig: vendor rollback `known-good/eyelash-corne/{dongle-niceview,dongle-oled}/`.
- ZmkConfig: `profiles/prospector-eyelash/` plus live keymap/overlays/confs wired to that attempt (same 42-key screenshot mapping; extra inner cluster kept).
- XIAO `reset_settings_xiao` added to `build.yaml`. Halves no longer clear bonds on every boot.

Preferred Eyelash keymap screenshot is `profiles/prospector-eyelash/keymap-preferred.png`.

## 2026-08-21 — Dual-repo review and Prospector verdict

Assessment of `Keyboards` (this repo) and `C:\Users\dseib\Documents\Projects\ZmkConfig`, plus the VIA export at `C:\Users\dseib\OneDrive\Desktop\Desktop Files (PC)\crkbd.layout 5.10.26.json`.

### Identity

- **Keep two repos.** QMK/VIA and ZMK are different CLIs, CI, board IDs, and remap tools. Merging them is what made `Keyboards` hard to scan.
- **Keyboards** is a wired Corne QMK staging area (`firmware/qmk/keymaps/keebv2`, VIA enabled) with leftover binaries for Sofle ZMK and Cheapino Vial. It is not a ZMK source repo.
- **ZmkConfig** is an extended zmk-config (not a ZMK fork) for wireless Sofle and Eyelash Corne, plus an unfinished Prospector central.
- **Eyelash Corne is not foostan/crkbd.** Vendor: https://github.com/a741725193/zmk-new_corne — will not run stock `corne` / QMK `crkbd` firmware.

### VIA JSON (May 2026)

- File is a VIA Crkbd export (`vendorProductId` 1179844609 = 0x46580001), not an Eyelash/ZMK keymap.
- Live VIA EEPROM has drifted from `keebv2` (`TO(2)` right thumb, empty macros vs in-repo `1992`).
- VIA overrides `keymap.c` after flash; the Desktop JSON is the live wired-Corne layout.

### Prospector (separate task)

**Verdict: no solution at ≥98% confidence** for replacing the current Eyelash dongle with Prospector as HID central.

Reasons: prior ZmkConfig attempt failed (USB up, no split BLE connect; halves `Error notifying -128`); repo tracks `zmk@main` (v0.4 as of 2026-07-08) while official `carrefinho/prospector-zmk-module` `main` targets ZMK v0.3 and `seeeduino_xiao_ble` (v0.4 board ID is `xiao_ble//zmk`); eyelash boards are custom, not `corne_dongle`; `eyelash_corne_dongle_xiao.overlay` is empty (ZMK dongle guide requires matching layouts/transforms); vendor west.yml now uses `cormoran` ZMK `v0.3-branch+dya` + DYA Studio; nRF52840 cannot be dumped via UF2.

**Rollback exists only as vendor packs**, not as a dump of whatever is currently flashed. Packs in `C:\Users\dseib\Downloads\ZMK分体键盘说明10月30日 (2)\...\固件-firmware\`:

- Dongle + nice!view: `new-corne-dongle-firmware接收器版本固件\`
- Dongle + OLED: `corne-OLED-dongle接收器版corne+OLED屏幕固件\`
- No-dongle studio: `new-corne_firmware\`

Sofle vendor UF2s are already in `archive/sofle-dongle-firmware/`.

Scanner-mode Prospector (t-ogura) does **not** replace the dongle.

### Upstream refs (as of assessment date)

- QMK External Userspace: https://docs.qmk.fm/newbs_external_userspace
- ZMK user config / pin version: https://zmk.dev/docs/user-setup , https://zmk.dev/blog/2025/06/20/pinned-zmk
- ZMK dongle: https://zmk.dev/docs/hardware-integration/dongle
- ZMK v0.4 / `xiao_ble//zmk`: https://zmk.dev/blog/2025/12/09/zephyr-4-1
- Prospector hardware: https://github.com/carrefinho/prospector
- Prospector module: https://github.com/carrefinho/prospector-zmk-module (v0.3 on `main`; Zephyr 4.1 on `feat/new-status-screens`)
- Vendor eyelash: https://github.com/a741725193/zmk-new_corne
