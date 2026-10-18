VIA_ENABLE = yes
OLED_ENABLE = yes
WPM_ENABLE = yes         # Luna's walk/run speed; sent to the right by KEEB_PET_STATE
COMBO_ENABLE = yes        # game exit: left Esc + right Esc
TAP_DANCE_ENABLE = yes    # Shift / Caps Lock on the home-row pinky
SPACE_CADET_ENABLE = no   # no longer used by the layout

# Flash budget trims (nothing in the layout uses these)
MAGIC_ENABLE = no         # QK_MAGIC_* keycodes (swap ctrl/gui etc.)
GRAVE_ESC_ENABLE = no
NKRO_ENABLE = no          # cannot be toggled without MAGIC anyway; 6KRO is plenty for 42 keys

# Per-key RGB (do NOT enable RGBLIGHT simultaneously)
RGBLIGHT_ENABLE = no
RGB_MATRIX_ENABLE = yes
# Hardware PWM driver is available in newer QMK builds. Override at
# compile time (e.g. `qmk compile ... WS2812_DRIVER=pwm`) when supported.
WS2812_DRIVER ?= bitbang

LTO_ENABLE = yes
