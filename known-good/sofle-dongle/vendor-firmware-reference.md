## Vendor Firmware Reference

The manufacturer provided a bundle of ready-to-flash UF2 images under
`c:/Users/dseib/Downloads/ZMK分体键盘说明10月30日 (2)/ZMK分体键盘说明10月27日/zmk -studio and firmware改键软件和固件/固件-firmware/`.
The table below maps the packages that are relevant for the Sofle dongle build.

| Folder | Contents | Notes |
| --- | --- | --- |
| `sofle-dongle-firmware接收器版固件` | `settings_reset-nice_nano_v2-zmk.uf2`, `eyelash_sofle_central_dongle_oled.uf2`, `eyelash_sofle_peripheral_left nice_view-nice_nano_v2-zmk.uf2`, `eyelash_sofle_peripheral_right nice_view-nice_nano_v2-zmk.uf2` | Matches the wireless Sofle with nice!view displays on both halves and an OLED on the dongle. Use this set as the factory baseline when debugging hardware. |
| `sofle-firmware` | `settings_reset-nice_nano_v2-zmk.uf2`, `eyelash_sofle_studio_left.uf2`, `nice_view-eyelash_sofle_right-zmk.uf2` | Standalone split build without the dongle. Useful if you ever want to run the halves as independent peripherals. |
| `sofle-oled-firmware` | UF2 files for Sofle halves with OLED shields instead of nice!view. Ignore unless you swap displays. |
| `oled-sofle-dongle` | Dongle-centric build for OLED halves. Not applicable to the current hardware. |

### Flashing sequence
1. Double-tap reset on the target nice!nano to enter UF2 bootloader mode.
2. Copy `settings_reset*.uf2` first, wait for the drive to remount, then copy the role-specific UF2 from the same folder.
3. Repeat for each board (dongle → left → right) and toggle their power switches afterwards so the new bonds start clean.

### When to use these images
- **Hardware validation:** If pairing keeps failing, flash the vendor UF2 set to confirm the MCU modules, batteries, and displays still work as shipped.
- **Comparing behavior:** Capture dongle logs with the factory firmware and note any differences in BLE events. This helps isolate whether issues stem from hardware or the custom configuration.
- **Emergency recovery:** Keep the UF2s archived in your cloud backup so you can always return to a known-good state without waiting on new builds.


