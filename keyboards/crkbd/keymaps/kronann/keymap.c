#include QMK_KEYBOARD_H
#include "layers.h"
#include "keymap_japanese.h"

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT_split_3x6_3_ex2(
        LCTL_T(KC_TAB), KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_LEFT,     KC_UP,   KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC,
        KC_LSFT, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_RGHT,     KC_DOWN, KC_H,    KC_J,    KC_K,    KC_L,    JP_SCLN, JP_QUOT,
        KC_LCTL, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                          KC_N,    KC_M,    JP_COMM, JP_DOT,  JP_SLSH, KC_ESC,
                                            KC_LGUI, TL_LOWR, KC_SPC,      KC_ENT,  TL_UPPR, KC_LALT
    ),

    [_LOWER] = LAYOUT_split_3x6_3_ex2(
        JP_EXLM, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    _______,     LGUI(KC_PGUP), KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_DEL,
        KC_LSFT, JP_HASH, JP_LCBR, JP_LBRC, JP_LPRN, JP_EQL,  _______,     LGUI(KC_PGDN), JP_MINS, JP_RPRN, JP_RBRC, JP_RCBR, JP_DLR,  JP_UNDS,
        KC_LCTL, JP_GRV,  JP_AT,   JP_AMPR, JP_LABK, JP_PIPE,                              JP_CIRC, JP_RABK, JP_ASTR, JP_PERC, JP_PLUS, JP_TILD,
                                            _______, _______, LCTL(KC_SPC),    _______,    _______, _______
    ),

    [_RAISE] = LAYOUT_split_3x6_3_ex2(
        _______, XXXXXXX, XXXXXXX, MS_UP,   XXXXXXX, XXXXXXX, _______,     _______, KC_HOME, KC_PGDN, KC_PGUP, KC_END,  KC_PSCR, _______,
        KC_LSFT, MS_WHLU, MS_LEFT, MS_DOWN, MS_RGHT, MS_ACL0, _______,     _______, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, XXXXXXX, XXXXXXX,
        KC_LCTL, MS_WHLD, MS_BTN2, MS_BTN3, MS_BTN1, XXXXXXX,                       KC_MUTE, KC_VOLD, KC_VOLU, KC_BRID, KC_BRIU, _______,
                                            MS_BTN2, TL_LOWR, MS_BTN1,     _______, _______, _______
    ),

    [_ADJUST] = LAYOUT_split_3x6_3_ex2(
        KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   XXXXXXX,     XXXXXXX, KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,
        RM_TOGG, RM_HUEU, RM_SATU, RM_VALU, RM_NEXT, XXXXXXX, XXXXXXX,     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        QK_BOOT, RM_HUED, RM_SATD, RM_VALD, RM_PREV, XXXXXXX,                       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, QK_BOOT,
                                            XXXXXXX, _______, XXXXXXX,     XXXXXXX, _______, XXXXXXX
    ),
};

const key_override_t delete_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_BSPC, KC_DEL);

const key_override_t quote_key_override = ko_make_basic(MOD_MASK_SHIFT, JP_QUOT, JP_DQUO);
const key_override_t question_key_override = ko_make_basic(MOD_MASK_SHIFT, JP_EXLM, JP_QUES);

const key_override_t *key_overrides[] = {
    &delete_key_override,
    &quote_key_override,
    &question_key_override,
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

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!record->event.pressed || !(get_mods() & MOD_MASK_SHIFT)) {
        return true;
    }
    switch (keycode) {
        case JP_SCLN:
            tap_without_shift(JP_COLN);
            return false;
        case JP_SLSH:
            tap_without_shift(JP_BSLS);
            return false;
    }
    return true;
}
