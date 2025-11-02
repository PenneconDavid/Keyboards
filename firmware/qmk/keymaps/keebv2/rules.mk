VIA_ENABLE = yes
OLED_ENABLE = yes
SPACE_CADET_ENABLE = yes

# Per-key RGB (do NOT enable RGBLIGHT simultaneously)
RGBLIGHT_ENABLE = no
RGB_MATRIX_ENABLE = yes
# Hardware PWM driver is available in newer QMK builds. Override at
# compile time (e.g. `qmk compile ... WS2812_DRIVER=pwm`) when supported.
WS2812_DRIVER ?= bitbang

LTO_ENABLE = yes

