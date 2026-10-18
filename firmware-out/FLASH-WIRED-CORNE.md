# Flashing the wired Corne (keebv2) on Windows

For the Keebmaker Corne 42-key, sold as "v4.1" but built on two **ATmega32U4 Pro Micros** with the
**Caterina** bootloader. QMK target `crkbd` (= `crkbd/rev1`), never `rev4_1`. One `.hex` file goes
on **both** halves.

Updated 2026-10-18 for the ZMK v4.1 layout with seven layer cards (left) and Luna (right).
The first OLED build was tested by the user: Luna was upside-down at the north end. This new
build turns only the right OLED 180 degrees. It is compiled and host-checked; the corrected right
orientation was confirmed by the user on 2026-10-18 ("this looks good now").

## What you need

| Item | Where |
|---|---|
| The firmware | **`firmware-out\crkbd_rev1_keebv2_v4-1_oled-luna-cards-orientation-fix.hex`** — use this same file for both halves. Build evidence: `docs\OLED-FIRMWARE-BUILD.md`. The older `firmware-out/crkbd_rev1_keebv2_v4-1.hex` is retained and has the previous experimental graphics. |
| Rollback firmware | `known-good\wired-corne\crkbd_rev1_dave_via.hex` (the old 4-layer VIA build) |
| QMK Toolbox | https://github.com/qmk/qmk_toolbox/releases → `qmk_toolbox_install.exe` (installer) or `qmk_toolbox.exe` (portable) |
| A data USB cable | charge-only cables are the number-one "it does not show up" cause |

## Rules for this board

**Before flashing:** save any current live VIA edits to a dated JSON in `layouts/via/`.
The packaged firmware keeps the seven-layer source layout, but a new firmware build can reset
VIA's EEPROM map. Re-import only a compatible seven-layer export after checking it.

1. **Never plug or unplug the TRRS cable while USB is connected.** The TRRS plug can short VCC to
   GND as it slides in and that kills Pro Micros. USB out first, always.
2. **Flash each half on its own over USB.** Firmware only reaches the half that is plugged into the
   computer. Unplug TRRS before you start (rule 1), flash left, flash right, then reconnect TRRS,
   then USB.
3. **USB goes into the left half in normal use.** The firmware is `MASTER_LEFT`: the half on USB is
   the master and it assumes it is the left one. Plug into the right half and the keys come out
   mirrored (Q types P, and so on). That is the quickest way to tell which half you are on.
4. **The same hex on both halves.** The halves agree on a serial protocol; mixing builds gives a
   dead or lagging right half.
5. **Flashing a newly dated VIA build can reset the VIA keymap.** VIA stamps its EEPROM with the firmware's
   build date; a new build does not match, so on first boot the board reloads all seven layers from
   `keymap.c`. Live edits made in VIA since the last export are gone. The last export before this
   port is `layouts\via\crkbd.layout.2026-05-10.json`.

## Entering the bootloader (Caterina)

Each half has a small reset button on the PCB (on Keebmaker boards it sits next to the Pro Micro,
reachable from the edge or through the plate cutout). **Double-tap it quickly** (two presses within
about half a second). The half drops off USB and comes back as a **new COM port** (Device Manager →
Ports: "Arduino Leonardo bootloader" or "USB Serial Device (COMn)") for about **8 seconds**, then
boots back into the keyboard firmware if nothing happens. If you miss the window, double-tap again.
A single tap just restarts the keyboard.

If there is no reset button: short the `RST` and `GND` pads on the Pro Micro twice with tweezers.

## Method A: QMK Toolbox (recommended for the first time)

1. Install and open QMK Toolbox. First time only: **Tools → Install Drivers** and accept the
   prompts (this installs the Caterina / avrdude USB serial driver).
2. **Open** → pick the `.hex`. Set **MCU (AVR only)** to `atmega32u4`. Tick **Auto-Flash**.
3. Unplug USB, unplug TRRS. Plug USB into the **left** half. The log shows a HID device connect.
4. Double-tap reset. The log shows `Caterina device connected (COMn)` and, with Auto-Flash on,
   immediately runs avrdude: `Flash complete`, then `Caterina device disconnected`, then the HID
   device reconnects. Takes about ten seconds.
5. Unplug USB. Plug USB into the **right** half. Double-tap reset. Same log. Same hex.
6. Unplug USB. Reconnect TRRS. Plug USB into the **left** half.

If Auto-Flash is off, press **Flash** inside the 8-second window instead.

## Method B: `scripts\flash-keebv2.ps1`

From the repo root in PowerShell:

```powershell
.\scripts\flash-keebv2.ps1
```

It copies the keymap into `~\qmk_firmware`, compiles, then stops twice ("Plug in the left half
alone, then press Enter and put it into the bootloader"). Press Enter, double-tap reset, and
`qmk flash -bl avrdude` waits for the new COM port and flashes it. It does not need a COM number;
the old script hard-coded COM5 (left) and COM6 (right), which only held on one machine. If
auto-detection does not pick the port up, read the number from Device Manager while the bootloader
is active and pass it:

```powershell
.\scripts\flash-keebv2.ps1 -LeftPort COM5 -RightPort COM6
```

`-BuildOnly` compiles without flashing. The script also sets the three environment variables the
QMK MSYS CLI needs when run from a normal PowerShell.

## After flashing: what to check

1. USB in the left half, TRRS connected. Both OLEDs light up: left shows the BASE card,
   right shows Luna near the bottom. **Both should read the same way up.** If the right (or both) is
   upside-down, see "OLEDs" in `keyboards\crkbd\keymaps\keebv2\README.md` for the two knobs.
2. Type on both halves: letters on the right half prove the TRRS link; `q` from the top-left letter
   key proves the master is the left half.
3. Home-row pinky: tap = Shift (hold and type a capital), double-tap = Caps Lock, double-tap again
   to clear.
4. Hold Space: numbers on the right hand. Hold Backspace: F-keys. Hold Backspace and tap the
   right-hand bottom row third key from the left (`ARC`): the left OLED switches to the ARC/WASD card.
   Leave with the right inner thumb, then re-enter and leave with left Esc + top-right key together.
   Also enter WASD, ESDF and ARC-E and confirm each has its own card; Symbols and Fn should show
   SYM and FN while held.
   On the right, type slowly then quickly to check sit/walk/run, hold Ctrl for sneak, enable Caps
   Lock for bark, and press Space for jump. A Symbols hold should not jump. Release Space and
   confirm clean landing. After 60 seconds of no input both displays should sleep; type on either
   half to wake them. Check PC sleep/wake as well.
5. Open https://usevia.app in Chrome or Edge, **Authorize device**, pick the Corne, check all seven
   layers look like the README table, then **Save + Load → Save** to
   `layouts\via\crkbd.layout.2026-10-17.json` (or today's date) and commit it.
6. If everything works, copy the hex next to the old one as the new baseline:
   `known-good\wired-corne\crkbd_rev1_keebv2_v4-1_oled-luna-cards-orientation-fix.hex`, and note it in `known-good\README.md`.
   (Left for after the hardware check.)

## Rolling back

Same procedure, Method A or B, with `known-good\wired-corne\crkbd_rev1_dave_via.hex` on both halves.
That build is the 4-layer map from `layouts\via\crkbd.layout.2026-05-10.json`; the VIA keymap resets
to its `keymap.c` on first boot, so re-import the May snapshot in VIA if you want the live tweaks back.

## When it goes wrong

| Symptom | Cause / fix |
|---|---|
| No COM port after double-tap | Too slow between taps, or a charge-only cable. Try again; try another cable or port. |
| Toolbox: `avrdude: butterfly_recv(): programmer is not responding` | The 8-second window closed. Double-tap and flash again. |
| Right half dead after flashing only the left | Rule 2: flash the right half too. |
| Keys mirrored | USB is in the right half. Move it to the left. |
| Board types but OLEDs dark | OLED timeout (60 s idle). Press a key. If still dark, reseat the OLED headers. |
| VIA does not see the keyboard | Use Chrome/Edge (WebHID), click Authorize device, try another USB port. The VID/PID is `0x4653:0x0001`. |
| Build says `firmware is too large` | Check the size line and build report before adding graphics. The ATmega32U4 application limit is 28,672 bytes. |
