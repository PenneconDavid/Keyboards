# keebv2 Corne keymap

Drop the contents of this folder into `qmk_firmware/keyboards/crkbd/keymaps/keebv2` and build with:

```
qmk compile -kb crkbd -km keebv2
```

Flashing (example for left half on COM5):

```
qmk flash -kb crkbd -km keebv2 -bl avrdude -c avr109 -p ATmega32U4 -P COM5
```

Hold the inner left thumb key for layer 1 (symbols/numpad), the inner right thumb key for navigation, and the middle right thumb key for the media/F-row layer. The nav layer includes the `M_1992` macro (tap to send "1992").

OLED code renders a static daisy image once per half; set `OLED_SHOW_DAISY` to `0` in `keymap.c` to switch back to the alignment grid.

