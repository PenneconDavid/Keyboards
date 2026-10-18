# OLED asset provenance

Luna sprite data: Copyright 2021 HellSingCoder, GPL-2.0-or-later.
Source: https://github.com/HellSingCoder/qmk_firmware/blob/master/keyboards/sofle/keymaps/helltm/keymap.c
The exact fetched source and SHA-256 are retained under
`docs/oled-previews/sources/luna.c` and `source-manifest.json` in this repository.
Only the ten 96-byte sprite initializers are incorporated, losslessly compressed, in `oled_assets.h`.

Original Luna source license notice:

> This program is free software: you can redistribute it and/or modify
> it under the terms of the GNU General Public License as published by
> the Free Software Foundation, either version 2 of the License, or
> (at your option) any later version.
>
> This program is distributed in the hope that it will be useful,
> but WITHOUT ANY WARRANTY; without even the implied warranty of
> MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
> GNU General Public License for more details.
>
> You should have received a copy of the GNU General Public License
> along with this program. If not, see <https://www.gnu.org/licenses/>.

Layer-card artwork and compact rendering code: created for this userspace and
provided under GPL-2.0-or-later. Card lettering is rasterized from the existing
local QMK `keyboards/crkbd/lib/glcdfont.c` font (classic Adafruit_GFX 5x7 font
with 6x8 cells). The ARC motif is an original generic arc, not a game logo.

Regenerate the generated header with `docs/oled-previews/build_assets.py`
(Python + Pillow; reads the local QMK font and retained Luna source). This keeps
firmware assets and review previews in sync.

The approved header retains its original `firmware-out/oled-previews/build_assets.py`
provenance comment so its build-time source hash stays intact. That generator now lives at
`docs/oled-previews/build_assets.py`; future regeneration uses a location-independent comment.
