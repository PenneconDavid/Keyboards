# OLED host verification

Run from PowerShell: `./docs/oled-previews/verification/run-host-test.ps1`.

The harness compiles the actual keymap `oled_graphics.h` and generated `oled_assets.h`
with QMK APIs mocked on the host, using bundled QMK MSYS GCC 15.1.0.
Compile flags: `-std=c11 -Wall -Wextra -Werror`.

Verified 2026-10-18 (including the right-display orientation correction):

- All seven left cards reconstruct exactly as independent PNG-derived 512-byte fixtures, with both software flip settings.
- Reads the real keymap config and verifies left 270 / right 270 degrees. Right changed from 90
  after the user's upside-down/north-end hardware report; left remains unchanged.
- All ten Luna frames match independent fixtures extracted from the original source, with both software flip settings.
- Caps Lock takes precedence over Ctrl, and both take precedence over typing speed.
- Jump and landing rewrite the complete band, erasing prior sprite residue.
- Both displays turn off for idle state and turn on again for input.
- Symbols layer hold does not jump; resolved layer-tap Space does jump.
- Plain Space stays raised while held; short taps remain latched for transport.
- Failed RPC sends retry; successful unchanged state waits for the heartbeat.
- Every mocked framebuffer write stays inside 512 bytes (58,400 writes exercised).

Result: PASS.

This is deterministic logic and framebuffer verification. It does not emulate AVR timing,
serial interrupts, OLED hardware, EEPROM, or the board's physical orientation. The atomic
block mock only executes its body once; the real AVR synchronization was reviewed against
`platforms/avr/drivers/serial.c` and the keymap's volatile/atomic snapshot code.
The generated host executable is disposable and is not keyboard firmware.

The compressed decoder was tested from nonzero initial framebuffer content and preserves bytes outside its 96-byte sprite history ring. `build_expected.py` regenerates independent test fixtures from the original Luna source and final card PNGs; run it only after deliberate artwork changes.
