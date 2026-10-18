# Wired Corne: layer cards + Luna

## Current firmware: right orientation correction, 2026-10-18

**Use `../firmware-out/crkbd_rev1_keebv2_v4-1_oled-luna-cards-orientation-fix.hex` on both halves.**
The previous build was tested by the user and looked good except Luna was upside-down at the
north end. `ROT_RIGHT` changes from 270 to 90, so the driver's effective right rotation changes
from 90 to 270. This turns both the picture and its north/south placement 180 degrees.
Left cards, keymap, RGB configuration, renderer and bitmap assets are unchanged.

BuildOnly succeeded: **28,374 / 28,672 bytes, 298 bytes free**. No compiler warnings; only QMK's
near-capacity notice. Independent review confirmed the rotation arithmetic. The host harness now
reads the actual keymap config and passes all checks, including corrected right rotation.
Evidence: `builds/wired-corne/2026-10-18/oled-build-orientation-fix.log`, `builds/wired-corne/2026-10-18/oled-host-verification-orientation-fix.log`, and
`builds/wired-corne/2026-10-18/oled-firmware-orientation-fix-manifest.json`. HEX checksums and application-region bounds pass;
compiled source hashes match the packaged build. Previous HEX/ELF and their manifest are preserved.
No flashing was performed by the agent. The user subsequently confirmed the corrected build
looks good on 2026-10-18; this is the approved firmware retained in `firmware-out/`.

## Original layer-card / Luna build, 2026-10-17 (retained history)


Built 2026-10-17; superseded by the approved orientation correction above.
Historical artifact: `../archive/firmware/wired-corne/2026-10-18/crkbd_rev1_keebv2_v4-1_oled-luna-cards.hex`.
Target: `crkbd` / `crkbd/rev1`, ATmega32U4 Pro Micro, Caterina. Normal use: USB in the left.
See `../firmware-out/FLASH-WIRED-CORNE.md` for the full procedure, recovery and hardware checklist.

## Delivered behavior

- Left: seven distinct cards, selected by highest active layer: BASE, SYM, WASD, FN, ARC, ESDF,
  ARC-E. ARC variants show the underlying hand position. No independent slideshow obscures state.
- Right: Luna sits at <=10 WPM, walks at 11-40, runs above 40. Caps selects bark; Ctrl selects
  sneak (Caps takes precedence). Space jumps while held and is latched for at least 400 ms so
  quick taps reach the right display. Holding the Symbols layer does not jump.
- Two-byte master-to-slave state snapshot with retries, change updates at most every 100 ms,
  and a 500 ms heartbeat. Atomic snapshot protects the AVR interrupt callback.
- Both OLEDs sleep after 60 seconds of no input; typing wakes them. PC suspend hooks retained.
- VIA, all seven key bindings/layers, combos, Shift/Caps tap dance and all six RGB Matrix effects
  preserved. No ZMK sources or recovery binaries changed.
- Existing rotation correction retained: effective left 270 degrees / right 90 degrees. Physical
  orientation still needs to be checked on the board.

## Build and checks

Command: `scripts/flash-keebv2.ps1 -BuildOnly` (runs `qmk compile -kb crkbd -km keebv2`).
Compiler: avr-gcc 15.1.0. Final build exit: 0.

**Application flash: 28,400 / 28,672 bytes; 272 bytes free (99%).**
Previous build: 28,554 bytes, 118 free. This build leaves 154 more bytes free.
No compiler warnings. QMK's existing near-capacity warning remains.
Static SRAM: 530 data + 1,386 BSS = 1,916 / 2,560 bytes, leaving 644 bytes for stack/runtime.
The decoder uses the existing OLED framebuffer as its history ring, with no second framebuffer.
Cards occupy 940 compressed bytes; Luna occupies 549; compression is lossless.

The first uncompressed candidate was 834 bytes too large; it was replaced with the compressed
implementation. Its log is retained as `builds/wired-corne/2026-10-18/oled-build.log`; it is **not** the packaged firmware.
The successful build evidence is **`builds/wired-corne/2026-10-18/oled-build-final.log`**.

Independent source review caught and resolved the serial-interrupt snapshot issue. Host C checks,
compiled with `-Wall -Wextra -Werror`, verify every byte of all seven cards and ten Luna frames,
both software flip settings, jump/landing cleanup, bounds (58,400 writes), tap/hold behavior,
sleep/wake and RPC retries/heartbeat. See `builds/wired-corne/2026-10-18/oled-host-verification.log` and
`oled-previews/verification/README.md` for the reproducible runner. All key bindings were compared
with HEAD and are unchanged. `git diff --check` passes.

The packaged Intel HEX was checked for record checksums, EOF, 28,400 data bytes, and all addresses
below the 28,672-byte application limit. Packaged HEX/ELF are copied from the successful build;
source hashes match the compiled keymap copy. SHA-256 and source hashes are recorded in
`builds/wired-corne/2026-10-18/oled-firmware-manifest.json`; its original `.sha256` companion is
retained in that same build-record directory.

## Review and flashing

Exact asset preview: `oled-previews/final/firmware-preview.png`.
The original `../firmware-out/crkbd_rev1_keebv2_v4-1.hex` and `known-good/` recovery files were preserved.

Use QMK Toolbox Method A in `../firmware-out/FLASH-WIRED-CORNE.md` to flash the packaged HEX. Method B recompiles
the current sources before flashing. Do not disconnect/reconnect TRRS while USB is connected.
Export any current VIA edits **before** flashing, since a new build can reset its EEPROM map;
check and export a fresh dated VIA snapshot after flashing. Do not treat this image as a new
known-good baseline until both halves pass the hardware checklist.

No board was flashed. The build and host tests establish compilation and logic, not physical
OLED appearance, real serial timing, power behavior, USB suspend/resume or hardware key behavior.
Those checks remain in the flash guide for the user's first test.
