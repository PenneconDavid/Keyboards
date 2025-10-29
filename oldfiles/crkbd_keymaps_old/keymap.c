#include QMK_KEYBOARD_H

// --------------------------- USER TWEAKS ---------------------------
// Show the daisy logo (not a grid)
#define OLED_SHOW_DAISY 1
// VIA owns RGB (Matrix). Set to 1 only if you want a forced test effect briefly.
#define RGB_DIAG_FORCE 0

// Portrait panels usually like 90°. If sideways, set BOTH to 270 and reflash both halves.
#define ROT_LEFT    90
#define ROT_RIGHT   90

// ---- Daisy shape controls (safe defaults for 32px wide portrait) ----
#define DAISY_R_CENTER   5   // center disk radius
#define DAISY_RX_PETAL   7   // horizontal petal radius (ellipse)
#define DAISY_RY_PETAL   9   // vertical petal radius (ellipse)
// Petal centers relative to screen center; higher overlap -> more “single flower”, less “five circles”
#define DAISY_OFF_X      8   // 6–8 looks good on 32px width
#define DAISY_OFF_Y     14   // vertical spacing (you have plenty of height)
#define DAISY_MARGIN     2   // keep a tiny margin from edges
// -------------------------------------------------------------------

enum { _BASE = 0, _NAV, _SYM, _FN };

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_BASE] = LAYOUT_split_3x6_3(
    KC_TAB,  KC_Q,   KC_W,   KC_E,   KC_R,   KC_T,                      KC_Y,   KC_U,   KC_I,    KC_O,   KC_P,   KC_BSPC,
    KC_LCTL, KC_A,   KC_S,   KC_D,   KC_F,   KC_G,                      KC_H,   KC_J,   KC_K,    KC_L,   KC_SCLN,KC_QUOT,
    KC_LSFT, KC_Z,   KC_X,   KC_C,   KC_V,   KC_B,                      KC_N,   KC_M,   KC_COMM, KC_DOT, KC_SLSH,KC_ESC,
                                  KC_LGUI, MO(_NAV), KC_SPC,      KC_ENT, MO(_SYM), KC_RALT
  ),
  [_NAV] = LAYOUT_split_3x6_3(
    KC_TAB, KC_1, KC_2, KC_3, KC_4, KC_5,                                 KC_6, KC_7, KC_8, KC_9, KC_0, KC_BSPC,
    KC_LCTL, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,               KC_LEFT, KC_DOWN, KC_UP, KC_RIGHT, XXXXXXX, XXXXXXX,
    KC_LSFT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,               XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                                  KC_LGUI, _______, KC_SPC,      KC_ENT, MO(_FN), KC_RALT
  ),
  [_SYM] = LAYOUT_split_3x6_3(
    KC_TAB, KC_EXLM, KC_AT,  KC_HASH, KC_DLR, KC_PERC,                   KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_BSPC,
    KC_LCTL, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                KC_MINS, KC_EQL,  KC_LBRC, KC_RBRC, KC_BSLS, KC_GRV,
    KC_LSFT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                KC_UNDS, KC_PLUS, KC_LCBR, KC_RCBR, KC_PIPE, KC_TILD,
                                  KC_LGUI, MO(_FN), KC_SPC,       KC_ENT, _______, KC_RALT
  ),
  [_FN] = LAYOUT_split_3x6_3(
    QK_BOOT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    RGB_TOG, RGB_HUI, RGB_SAI, RGB_VAI, XXXXXXX, XXXXXXX,                XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    RGB_MOD, RGB_HUD, RGB_SAD, RGB_VAD, XXXXXXX, XXXXXXX,                XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                                  KC_LGUI, _______, KC_SPC,       KC_ENT, _______, KC_RALT
  )
};

#ifdef RGB_MATRIX_ENABLE
void keyboard_post_init_user(void) {
#if RGB_DIAG_FORCE
  rgb_matrix_enable_noeeprom();
  rgb_matrix_mode_noeeprom(RGB_MATRIX_RAINBOW_MOVING_CHEVRON);
  rgb_matrix_sethsv_noeeprom(128, 255, 160);
#else
  rgb_matrix_enable_noeeprom(); // VIA owns modes
#endif
}
#endif

// --------------------------- OLED SECTION ---------------------------
#ifdef OLED_ENABLE
// helpers
#define SQI(x)    ((int32_t)(x) * (int32_t)(x))
#define MINI(a,b) ((a) < (b) ? (a) : (b))
#define MAXI(a,b) ((a) > (b) ? (a) : (b))

static inline oled_rotation_t rot_from_deg(uint16_t d) {
  switch (d) { case 0: return OLED_ROTATION_0; case 90: return OLED_ROTATION_90; case 180: return OLED_ROTATION_180; default: return OLED_ROTATION_270; }
}
oled_rotation_t oled_init_user(oled_rotation_t rot) {
  return is_keyboard_master() ? rot_from_deg(ROT_LEFT) : rot_from_deg(ROT_RIGHT);
}

// rotated dimensions so drawings fill visible area
static inline void get_rotated_dims(uint8_t *rw, uint8_t *rh) {
  const uint8_t base_w = OLED_DISPLAY_WIDTH;   // e.g., 128 on SSD1306 128x32
  const uint8_t base_h = OLED_DISPLAY_HEIGHT;  // e.g., 32  on SSD1306 128x32
  const bool rot90or270 = ((ROT_LEFT % 180) != 0);
  *rw = rot90or270 ? base_h : base_w; // rotated width  (≈32 on portrait)
  *rh = rot90or270 ? base_w : base_h; // rotated height (≈128 on portrait)
}

// filled circle test
static inline bool in_circle(int16_t x, int16_t y, int16_t cx, int16_t cy, int16_t r) {
  int32_t dx = x - cx, dy = y - cy;
  return (SQI(dx) + SQI(dy) <= SQI(r));
}

// filled ellipse test (axis-aligned)
static inline bool in_ellipse(int16_t x, int16_t y, int16_t cx, int16_t cy, int16_t rx, int16_t ry) {
  int32_t dx = x - cx, dy = y - cy;
  // dx^2/ rx^2 + dy^2/ry^2 <= 1  ->  dx^2*ry^2 + dy^2*rx^2 <= rx^2*ry^2
  return (SQI(dx) * SQI(ry) + SQI(dy) * SQI(rx) <= SQI(rx) * SQI(ry));
}

static void draw_daisy_overlap(void) {
  uint8_t w, h; get_rotated_dims(&w, &h);
  const int16_t cx = w / 2;
  const int16_t cy = h / 2;

  const int16_t rC  = DAISY_R_CENTER;
  const int16_t rxP = DAISY_RX_PETAL;
  const int16_t ryP = DAISY_RY_PETAL;
  const int16_t offx = DAISY_OFF_X;
  const int16_t offy = DAISY_OFF_Y;

  // ensure safe bounds (minimal margins)
  const int16_t x_min = MAXI(0,          cx - (offx + rxP) - DAISY_MARGIN);
  const int16_t x_max = MINI((int16_t)w-1, cx + (offx + rxP) + DAISY_MARGIN);
  const int16_t y_min = MAXI(0,          cy - (offy + ryP) - DAISY_MARGIN);
  const int16_t y_max = MINI((int16_t)h-1, cy + (offy + ryP) + DAISY_MARGIN);

  oled_clear();
  for (int16_t y = y_min; y <= y_max; y++) {
    for (int16_t x = x_min; x <= x_max; x++) {
      bool on = false;
      // center disk (solid)
      if (in_circle(x, y, cx, cy, rC)) on = true;

      // four petals (solid ellipses) with deliberate overlap
      if (!on && in_ellipse(x, y, cx + offx, cy,        rxP, ryP)) on = true; // East
      if (!on && in_ellipse(x, y, cx - offx, cy,        rxP, ryP)) on = true; // West
      if (!on && in_ellipse(x, y, cx,        cy + offy, rxP, ryP)) on = true; // South
      if (!on && in_ellipse(x, y, cx,        cy - offy, rxP, ryP)) on = true; // North

      if (on) oled_write_pixel((uint8_t)x, (uint8_t)y, true);
    }
  }
}

#if OLED_SHOW_DAISY
bool oled_task_user(void) {
  static bool left_done = false, right_done = false;
  if (is_keyboard_master()) {
    if (!left_done)  { draw_daisy_overlap(); left_done = true; }
  } else {
    if (!right_done) { draw_daisy_overlap(); right_done = true; }
  }
  return false;
}
#else
// (Fallback grid—set OLED_SHOW_DAISY to 0 to re-check alignment)
static void draw_rotaware_grid(void) {
  uint8_t w, h; get_rotated_dims(&w, &h);
  oled_clear();
  for (uint8_t y = 0; y < h; y++) {
    bool hline = (y == 0) || (y == h - 1) || (y % 8 == 0);
    for (uint8_t x = 0; x < w; x++) {
      bool vline = (x == 0) || (x == w - 1) || (x % 8 == 0);
      if (hline || vline) oled_write_pixel(x, y, true);
    }
  }
}
bool oled_task_user(void) {
  static bool left_done = false, right_done = false;
  if (is_keyboard_master()) {
    if (!left_done)  { draw_rotaware_grid(); left_done = true; }
  } else {
    if (!right_done) { draw_rotaware_grid(); right_done = true; }
  }
  return false;
}
void suspend_power_down_user(void) {
  #ifdef RGB_MATRIX_ENABLE
      rgb_matrix_set_suspend_state(true);
  #endif
  #ifdef OLED_ENABLE
      oled_off();
  #endif
  }
  
  void suspend_wakeup_init_user(void) {
  #ifdef RGB_MATRIX_ENABLE
      rgb_matrix_set_suspend_state(false);
  #endif
  #ifdef OLED_ENABLE
      oled_on();
  #endif
  }
  
#endif // OLED_SHOW_DAISY
#endif // OLED_ENABLE
