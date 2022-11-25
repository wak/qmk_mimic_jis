/*  -*-  eval: (turn-on-orgtbl); -*-
 * default HHKB Layout
 */
#include QMK_KEYBOARD_H

#define MIMIC_JIS_KEEP_SCREEN_ENABLE 1
#define MIMIC_JIS_OS_WIN_MAC_TOGGLE_ENABLE 1
#define MIMIC_JIS_LAYOUT_JIS_US_TOGGLE_ENABLE 1
#define MIMIC_JIS_MY_CUSTOM_FEATURE_ENABLE 1
#define MIMIC_JIS_NATIVE_FN_ENABLE 1
#define MIMIC_JIS_ALT_SPC_TO_ZKHK_ENABLE 1

#include "keymap_japanese.h"
#include "../../../../../mimic_jis2/mimic_jis2.c"

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* BASE Level: Default Layer
     |-------+---+---+---+---+---+---+---+---+---+---+-------+-----+-------+---|
     | Esc   | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 0 | -     | =   | \     | ` |
     |-------+---+---+---+---+---+---+---+---+---+---+-------+-----+-------+---|
     | Tab   | Q | W | E | R | T | Y | U | I | O | P | [     | ]   | Backs |   |
     |-------+---+---+---+---+---+---+---+---+---+---+-------+-----+-------+---|
     | Cont  | A | S | D | F | G | H | J | K | L | ; | '     | Ent |       |   |
     |-------+---+---+---+---+---+---+---+---+---+---+-------+-----+-------+---|
     | Shift | Z | X | C | V | B | N | M | , | . | / | Shift | Fn0 |       |   |
     |-------+---+---+---+---+---+---+---+---+---+---+-------+-----+-------+---|

            |------+------+-----------------------+------+------|
            | LAlt | LGUI | ******* Space ******* | RGUI | RAlt |
            |------+------+-----------------------+------+------|
    */

    [JIS] = LAYOUT(
        KC_ESC, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, MY_MIN_USCRE, MY_EQL_PLUS, KC_JYEN, MY_TILD_BQUOT,
        KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, JP_LBRC, JP_RBRC, KC_BSPC,
        MY_CONTROL, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, MY_QUOT_DQUOT, KC_ENT,
        MY_SHIFT, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, MY_SHIFT, MO(HHKB),
        KC_ZKHK, MY_MOD_1, /*        */ KC_SPC, MY_MOD_2, MY_MOD_3),

    [US] = LAYOUT( //  default layer
        KC_ESC, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_MINS, KC_EQL, KC_BSLS, KC_GRV,
        KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_LBRC, KC_RBRC, KC_BSPC,
        MY_CONTROL, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT, KC_ENT,
        MY_SHIFT, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, MY_SHIFT, MO(HHKB),
		MY_ZENKAKU_HANKAKU, MY_MOD_1, /*        */ KC_SPC, MY_MOD_2, MY_MOD_3),


    /* Layer HHKB: HHKB mode (HHKB Fn)
      |------+-----+-----+-----+----+----+----+----+-----+-----+-----+-----+-------+-------+-----|
      | Pwr  | F1  | F2  | F3  | F4 | F5 | F6 | F7 | F8  | F9  | F10 | F11 | F12   | Ins   | Del |
      |------+-----+-----+-----+----+----+----+----+-----+-----+-----+-----+-------+-------+-----|
      | Caps | Bac | End | Del |    |    |    |    | Psc | Slk | Pus | Up  |       | Backs |     |
      |------+-----+-----+-----+----+----+----+----+-----+-----+-----+-----+-------+-------+-----|
      |      | Hom | Lef | Up  | Dow| Rig| *  | /  | Hom | PgU | Lef | Rig | Enter |       |     |
      |------+-----+-----+-----+----+----+----+----+-----+-----+-----+-----+-------+-------+-----|
      |      |     |     |     |    |    | +  | -  | End | PgD | Dow |     |       |       |     |
      |------+-----+-----+-----+----+----+----+----+-----+-----+-----+-----+-------+-------+-----|

                 |------+------+----------------------+------+------+
                 | **** | **** | ******************** | **** | **** |
                 |------+------+----------------------+------+------+

     */

	/* Fnキーを押している状態のレイアウト */
    [HHKB] = LAYOUT(
        MY_KEEP_SCREEEN, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, KC_INS, KC_DEL,
        KC_CAPS, KC_TRNS, KC_BSPC, KC_END , KC_DEL , KC_TRNS, KC_TRNS, KC_TRNS, KC_PSCR, KC_SLCK, KC_PAUS, KC_UP, KC_TRNS, KC_BSPC,
        KC_TRNS, KC_HOME, KC_LEFT, KC_UP  , KC_DOWN, KC_RGHT, KC_PAST, KC_PSLS, KC_HOME, KC_PGUP, KC_LEFT, KC_RGHT, KC_PENT,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_PPLS, KC_PMNS, KC_END, KC_PGDN, KC_DOWN, KC_TRNS, KC_TRNS,
        MY_TOGGLE_LANG, MY_TOGGLE_OS, KC_TRNS, KC_TRNS, KC_TRNS),

};
