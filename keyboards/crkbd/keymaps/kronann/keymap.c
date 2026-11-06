#include QMK_KEYBOARD_H
#include "layers.h"
#include "keymap_japanese.h"

enum custom_keycodes {
    HYPER_KEY = SAFE_RANGE,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT_split_3x6_3_ex2(
        LCTL_T(KC_TAB), KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_LEFT,     KC_UP,   KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC,
        KC_LSFT, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_RGHT,     KC_DOWN, KC_H,    KC_J,    KC_K,    KC_L,    JP_SCLN, JP_QUOT,
        KC_LCTL, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                          KC_N,    KC_M,    JP_COMM, JP_DOT,  JP_SLSH, KC_ESC,
                                            HYPER_KEY, TL_LOWR, KC_SPC,      KC_ENT,  TL_UPPR, KC_LALT
    ),

    // Keys left transparent go out with Super held, so every Hyprland/skhd shortcut keeps working.
    [_HYPER] = LAYOUT_split_3x6_3_ex2(
        _______, _______, _______, _______, _______, _______, KC_PGUP,     _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, KC_PGDN,     _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______,                       KC_MUTE, KC_VOLD, KC_VOLU, KC_BRID, KC_BRIU, _______,
                                            _______, _______, _______,     _______, _______, _______
    ),

    [_LOWER] = LAYOUT_split_3x6_3_ex2(
        JP_EXLM, JP_HASH, JP_AT,   JP_LCBR, JP_RCBR, JP_AMPR, LGUI(KC_PGUP),     JP_CIRC, JP_ASTR, KC_1,    KC_2,    KC_3,    JP_SLSH, KC_DEL,
        KC_LSFT, JP_DLR,  JP_PERC, JP_LPRN, JP_RPRN, JP_EQL,  LGUI(KC_PGDN),     JP_PIPE, JP_PLUS, KC_4,    KC_5,    KC_6,    JP_MINS, LCTL(KC_SPC),
        KC_LCTL, JP_GRV,  JP_LABK, JP_LBRC, JP_RBRC, JP_RABK,                             JP_UNDS, KC_7,    KC_8,    KC_9,    JP_DOT,  JP_COMM,
                                            _______, _______, _______,     KC_0,    _______, _______
    ),

    [_RAISE] = LAYOUT_split_3x6_3_ex2(
        _______, XXXXXXX, MS_WHLD, MS_UP,   MS_WHLU, XXXXXXX, _______,     _______, KC_HOME, KC_PGDN, KC_PGUP, KC_END,  KC_PSCR, _______,
        KC_LSFT, MS_WHLU, MS_LEFT, MS_DOWN, MS_RGHT, MS_ACL0, _______,     _______, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, XXXXXXX, XXXXXXX,
        KC_LCTL, MS_WHLD, MS_BTN2, MS_BTN3, MS_BTN1, XXXXXXX,                       C(KC_LEFT), C(S(KC_DOWN)), C(S(KC_UP)), C(KC_RGHT), XXXXXXX, _______,
                                            MS_BTN2, TL_LOWR, MS_BTN1,     _______, _______, _______
    ),

    [_ADJUST] = LAYOUT_split_3x6_3_ex2(
        KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   XXXXXXX,     XXXXXXX, KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,
        RM_TOGG, RM_HUEU, RM_SATU, RM_VALU, RM_NEXT, XXXXXXX, XXXXXXX,     XXXXXXX, KC_MUTE, KC_VOLD, KC_VOLU, KC_BRID, KC_BRIU, XXXXXXX,
        QK_BOOT, RM_HUED, RM_SATD, RM_VALD, RM_PREV, XXXXXXX,                       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, QK_BOOT,
                                            XXXXXXX, _______, XXXXXXX,     XXXXXXX, _______, XXXXXXX
    ),
};

const key_override_t delete_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_BSPC, KC_DEL);

const key_override_t quote_key_override = ko_make_basic(MOD_MASK_SHIFT, JP_QUOT, JP_DQUO);
const key_override_t question_key_override = ko_make_basic(MOD_MASK_SHIFT, JP_EXLM, JP_QUES);
// On a JIS host Shift+- would type =, which LOWER already has.
const key_override_t shifted_minus_key_override = ko_make_basic(MOD_MASK_SHIFT, JP_MINS, KC_NO);

const key_override_t *key_overrides[] = {
    &delete_key_override,
    &quote_key_override,
    &question_key_override,
    &shifted_minus_key_override,
};

// Karabiner reorders a report that both releases Shift and presses a key, so Shift is released in its own report first.
static void tap_without_shift(uint16_t keycode) {
    const uint8_t mods = get_mods();
    del_mods(MOD_MASK_SHIFT);
    send_keyboard_report();
    tap_code16(keycode);
    set_mods(mods);
    send_keyboard_report();
}

static bool     hyper_used  = false;
static uint16_t hyper_timer = 0;

static bool is_media(uint16_t keycode) {
    switch (keycode) {
        case KC_MUTE:
        case KC_VOLD:
        case KC_VOLU:
        case KC_BRID:
        case KC_BRIU:
            return true;
    }
    return false;
}

// Hyper holds _HYPER and adds Super only once a non-media key is pressed: Super with
// Volume or Brightness would open system settings on macOS. Tapped alone, it taps Super.
static bool process_hyper(uint16_t keycode, keyrecord_t *record) {
    if (keycode == HYPER_KEY) {
        if (record->event.pressed) {
            hyper_used  = false;
            hyper_timer = timer_read();
            layer_on(_HYPER);
        } else {
            layer_off(_HYPER);
            if (get_mods() & MOD_BIT(KC_LGUI)) {
                unregister_mods(MOD_BIT(KC_LGUI));
            } else if (!hyper_used && timer_elapsed(hyper_timer) < TAPPING_TERM) {
                tap_code(KC_LGUI);
            }
        }
        return false;
    }
    if (record->event.pressed && layer_state_is(_HYPER)) {
        hyper_used = true;
        if (is_media(keycode)) {
            unregister_mods(MOD_BIT(KC_LGUI));
        } else {
            register_mods(MOD_BIT(KC_LGUI));
        }
    }
    return true;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!process_hyper(keycode, record)) {
        return false;
    }
    if (!record->event.pressed || !(get_mods() & MOD_MASK_SHIFT)) {
        return true;
    }
    switch (keycode) {
        case JP_SCLN:
            tap_without_shift(JP_COLN);
            return false;
        case JP_PIPE:
            tap_without_shift(JP_BSLS);
            return false;
    }
    return true;
}

// The default would shift - into _, but a JIS host types = for Shift+-.
bool caps_word_press_user(uint16_t keycode) {
    switch (keycode) {
        case KC_A ... KC_Z:
            add_weak_mods(MOD_BIT(KC_LSFT));
            return true;
        case KC_1 ... KC_0:
        case KC_BSPC:
        case KC_DEL:
        case JP_MINS:
        case JP_UNDS:
            return true;
        default:
            return false;
    }
}
