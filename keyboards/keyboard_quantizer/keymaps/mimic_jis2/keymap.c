/* Copyright 2020 sekigon-gonnoc
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H
#include "keymap_jp.h"

#define MIMIC_JIS_KEEP_SCREEN_ENABLE 1
#define MIMIC_JIS_TILDE_TO_ZKHK_ENABLE 1
#define MIMIC_JIS_MY_CUSTOM_FEATURE_ENABLE 1
// #define MIMIC_JIS_SHIFT_SPC_TO_ZKHK_ENABLE 1
#define MIMIC_JIS_ALT_SPC_TO_ZKHK_ENABLE 1
#include "../../../../mimic_jis2/mimic_jis2.c"

// Defines names for use in layer keycodes and the keymap
enum layer_names {
    _BASE,
    _LOWER,
    _RAISE,
};

#define LOSP LT(_LOWER, KC_SPC)
#define RAEN LT(_RAISE, KC_ENT)

// clang-format off
/*
 *  keyboard_quantizer.h のLAYOUTも修正しているので注意。
 *   KC_BSLASH => yen
*/
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Base */
    [JIS] =  LAYOUT(
		KC_ESC, KC_F1, KC_F2, KC_F3, KC_F4,     KC_F5, KC_F6, KC_F7, KC_F8,     KC_F9, KC_F10, KC_F11, KC_F12,    KC_PSCR, KC_SCROLLLOCK, KC_PAUS,
        KC_CAPS, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, MY_MIN_USCRE, MY_EQL_PLUS, JP_YEN, KC_BSPACE, KC_INSERT, KC_HOME, KC_PGUP,       KC_NUMLOCK, KC_KP_SLASH, KC_KP_ASTERISK, KC_KP_MINUS,
        KC_TAB,   KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, JP_LBRC, JP_RBRC,           KC_ENT,   KC_DEL, KC_END,  KC_PGDN,          KC_KP_7, KC_KP_8, KC_KP_9, KC_KP_PLUS,
        MY_TILD_BQUOT,   KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, MY_QUOT_DQUOT, JP_RBRC,                                                  KC_KP_4, KC_KP_5, KC_KP_6,
        MY_SHIFT,    KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, JP_SLSH, JP_BSLS, MY_SHIFT,                 KC_UP,                     KC_KP_1, KC_KP_2, KC_KP_3, KC_KP_ENTER,
        MY_CONTROL, MO(JIS+1), KC_LALT, JP_MHEN, /*LOSP*/KC_SPC,  /*RAEN*/KC_SPC, JP_KANA, KC_RALT, KC_RGUI, KC_APP, MY_CONTROL,            KC_LEFT,  KC_DOWN, KC_RIGHT,       KC_KP_0,          KC_KP_DOT
		),
	[JIS+1] =  LAYOUT(
		MY_KEEP_SCREEEN, KC_F1, KC_F2, KC_F3, KC_F4,     KC_F5, KC_F6, KC_F7, KC_F8,     KC_F9, KC_F10, KC_F11, KC_F12,    KC_PSCR, KC_SCROLLLOCK, KC_PAUS,
        KC_CAPS, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, MY_MIN_USCRE, MY_EQL_PLUS, JP_YEN, KC_BSPACE, KC_INSERT, KC_HOME, KC_PGUP,       KC_NUMLOCK, KC_KP_SLASH, KC_KP_ASTERISK, KC_KP_MINUS,
        KC_TAB,   KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, JP_LBRC, JP_RBRC,           KC_ENT,   KC_DEL, KC_END,  KC_PGDN,          KC_KP_7, KC_KP_8, KC_KP_9, KC_KP_PLUS,
        MY_TILD_BQUOT,   KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, MY_QUOT_DQUOT, JP_RBRC,                                                  KC_KP_4, KC_KP_5, KC_KP_6,
        MY_SHIFT,    KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, JP_SLSH, JP_BSLS, MY_SHIFT,                 KC_UP,                     KC_KP_1, KC_KP_2, KC_KP_3, KC_KP_ENTER,
        MY_CONTROL, KC_TRNS, KC_LALT, JP_MHEN, LOSP,  RAEN, JP_KANA, KC_RALT, KC_RGUI, KC_APP, MY_CONTROL,            KC_LEFT,  KC_DOWN, KC_RIGHT,       KC_KP_0,          KC_KP_DOT
		),
};
// clang-format on

/* bool process_record_user(uint16_t keycode, keyrecord_t *record) { */
/*     return true; */
/* } */

#ifdef OLED_DRIVER_ENABLE
#    include "rev1.h"
#    include "oled_driver.h"
#endif

#ifdef OLED_DRIVER_ENABLE
void oled_task_user(void) { render_logo(); }
#endif

