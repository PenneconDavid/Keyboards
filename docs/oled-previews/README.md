# Wired Corne OLED review previews

Review artifacts only. No keymap, firmware configuration, or connected board was changed.
The six panels correspond to A-F in `../../firmware-out/OLED-GRAPHICS-OPTIONS.md`.

## Selected implementation

The selected B-right / E-left implementation is now compiled. See
`../OLED-FIRMWARE-BUILD.md` and `final/firmware-preview.png` for the actual firmware assets.
The original six-option comparison remains a concept reference. In the selected build,
the active layer selects one of seven cards; the left display does not cycle independently.

- `six-options.png`: six-option comparison, enlarged with nearest-neighbor pixels.
- `A-preview.png` through `F-preview.png`: individual panels.
- `A-bongo-source.gif`: the two actual tap frames, upright.
- `B-luna.gif`: sit, walk, run, bark, sneak, two frames per pose, repeated for visibility.
- `E-slideshow.gif`: three example cards. Animation timings are illustrative.
- `render_previews.py`: reproducible renderer; requires Pillow and reads the local QMK Corne font.

## Sources and corrections

A uses https://github.com/nwii/oledbongocat/blob/main/keymap.c, retained in
`sources/bongo.c`. Its artwork initializers are 512 bytes per frame (128x32), but its
declarations reserve 636 bytes per frame. Eight frames therefore reserve 5,088 bytes,
including zero padding, before player code and WPM. This is not the compressed-diff
implementation described in the options note. Even removing padding leaves 4,096 bytes
of artwork, beyond the note's 4,050-byte total budget before code. A trimmed frame set
or compression would be needed. The fetched repository root does not list a LICENSE;
the options note's blanket licensing assurance has not been verified. Original art
attribution in the options note: StrayRogue and DitzyFlama.

B uses https://github.com/HellSingCoder/qmk_firmware/blob/master/keyboards/sofle/keymaps/helltm/keymap.c,
retained in `sources/luna.c`. Each sprite uses 96 bytes (32x24 storage, artwork described
as 32x22), so the ten sprites total 960 bytes, rather than 1,280. This implementation
barks for Caps Lock, sneaks for Ctrl and jumps for Space. The options note incorrectly
describes Space as the bark trigger. Preview placement is illustrative; this is Luna,
not a verified Felix variant.

C and E are newly drawn 1-bit examples, not previously approved final designs. D uses
the actual local 6x8 font with illustrative status values. F decodes the logo glyphs in
`C:/Users/dseib/qmk_firmware/keyboards/crkbd/lib/glcdfont.c` and `logo_reader.c`.
That local font has 1,344 bytes; the note's 1.5 KiB figure is approximate.

A and F show landscape art sideways within the physical tall panel, with an upright
inset for inspection. Changing the driver's rotation changes the logical canvas;
it does not physically turn the panel. B-F portrait previews have 32x128 pixels.

Historical preview-stage finding (superseded by the approved orientation correction in
`../OLED-FIRMWARE-BUILD.md`): source confirmed a portrait rotation offset added to the keyboard's mounting
correction: effective left 270 degrees and right 90 degrees. Older Second Brain notes
saying both are 270 degrees predate that fix. Orientation on the actual board has not
been verified in this preview task. The note's build sizes were not remeasured; no
firmware implementation was requested. Final fit, transport synchronization, timing,
and appearance still need compile/hardware validation after selection.

`source-manifest.json` records fetched source hashes and decoded sizes. Source files
are retained as reference material with their original notices, not linked into firmware.
