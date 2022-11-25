/*  -*-  eval: (turn-on-orgtbl); -*-
 * default HHKB Layout
 */
#include QMK_KEYBOARD_H
#include "keymap_jp.h"


#define JIS    0
#define HHKB   1

enum my_keycodes {
	MY_CONTROL = SAFE_RANGE,
	MY_SHIFT,
	MY_EQL_PLUS,
	MY_MIN_USCRE,
	MY_TILD_BQUOT,
	MY_QUOT_DQUOT,
};

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
        KC_ZKHK, KC_LALT, /*        */ KC_SPC, KC_RALT, KC_LGUI),


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
        KC_PWR, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, KC_INS, KC_DEL,
        KC_CAPS, KC_TRNS, KC_BSPC, KC_END , KC_DEL , KC_TRNS, KC_TRNS, KC_TRNS, KC_PSCR, KC_SLCK, KC_PAUS, KC_UP, KC_TRNS, KC_BSPC,
        KC_TRNS, KC_HOME, KC_LEFT, KC_UP  , KC_DOWN, KC_RGHT, KC_PAST, KC_PSLS, KC_HOME, KC_PGUP, KC_LEFT, KC_RGHT, KC_PENT,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_PPLS, KC_PMNS, KC_END, KC_PGDN, KC_DOWN, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS),

};

static bool mod_control;
static bool mod_shift;
static bool alt;
static bool gui;

static bool my_control;
static bool my_shift;

static bool custom_key_mode = false;
static bool custom_key_processing = false;

static struct {
	bool with_my_shift;
	uint16_t from;
	uint16_t to;
	bool shift;
	bool enabled;
} JIS_KEYCODE_MAP[] = {
	{false, MY_EQL_PLUS  , JP_MINS, true , false}, /* = */
	{true , MY_EQL_PLUS  , JP_SCLN, true , false}, /* + */

	{false, MY_MIN_USCRE , JP_MINS, false, false}, /* - */
	{true , MY_MIN_USCRE , JP_BSLS, true , false}, /* _ */

	{false, MY_TILD_BQUOT, JP_AT  , true , false}, /* ` */
	{true , MY_TILD_BQUOT, JP_CIRC, true , false}, /* ~ */

	{false, MY_QUOT_DQUOT, KC_7   , true , false}, /* ' */
	{true , MY_QUOT_DQUOT, KC_2   , true , false}, /* " */

	{true , KC_2         , JP_AT  , false, false}, /* @ */
	{true , KC_6         , JP_CIRC, false, false}, /* ^ */
	{true , KC_7         , KC_6   , true , false}, /* & */
	{true , KC_8         , JP_COLN, true , false}, /* * */
	{true , KC_9         , KC_8   , true , false}, /* ( */
	{true , KC_0         , KC_9   , true , false}, /* ) */

	{true , KC_SCLN      , JP_COLN, false, false}, /* : */
};

static void fetch_mods_status(void)
{
	mod_control = get_mods() & MOD_MASK_CTRL;
	mod_shift   = get_mods() & MOD_MASK_SHIFT;
	alt         = get_mods() & MOD_MASK_ALT;
	gui         = get_mods() & MOD_MASK_GUI;
}

static uint16_t unregister_alt_mods(void)
{
	uint16_t ret = 0;

	if (get_mods() & MOD_BIT(KC_LALT)) {
		unregister_mods(MOD_BIT(KC_LALT));
		ret |= MOD_BIT(KC_LALT);
	}
	
	if (get_mods() & MOD_BIT(KC_RALT)) {
		unregister_mods(MOD_BIT(KC_RALT));
		ret |= MOD_BIT(KC_RALT);
	}

	return ret;
}

static void register_control_mods(void)
{
	if (!(get_mods() & MOD_MASK_CTRL))
		register_mods(MOD_BIT(KC_LCTL));
}

static uint16_t unregister_control_mods(void)
{
	uint16_t ret = 0;

	if (get_mods() & MOD_BIT(KC_LCTL)) {
		unregister_mods(MOD_BIT(KC_LCTL));
		ret |= MOD_BIT(KC_LCTL);
	}
	
	return ret;
}

static void reset_my_control_mods(void)
{
	if (my_control)
		register_control_mods();
	else
		unregister_control_mods();
}

static bool register_shift_mods(void)
{
	if (!(get_mods() & MOD_MASK_SHIFT)) {
		register_mods(MOD_BIT(KC_LSFT));
		return true;
	}
	return false;
}

static uint16_t unregister_shift_mods(void)
{
	uint16_t ret = 0;

	if (get_mods() & MOD_MASK_SHIFT) {
		unregister_mods(MOD_BIT(KC_LSFT));
		ret |= MOD_BIT(KC_LSFT);
	}

	return ret;
}

static bool process_my_control(uint16_t keycode, keyrecord_t *record)
{
	if (keycode != MY_CONTROL)
		return true;

	my_control = record->event.pressed;

	if (record->event.pressed) {
		reset_my_control_mods();
	} else {
		/* 例えば、Ctrl+k を押している状態でCtrlを離した場合、
		 * 通常のキーボードの場合はkが入力される。
		 * その挙動をさせるのは手間がかかるため、問答無用でキー入力を解除する。*/
		if (custom_key_processing)
			clear_keyboard();
		else
			unregister_control_mods();
		custom_key_processing = false;
	}

	return false;
}

static uint16_t get_control_custom_key(uint16_t keycode)
{
	switch (keycode) {
	case KC_A: return KC_HOME;
	case KC_E: return KC_END;
	case KC_D: return KC_DEL;
	case KC_U: return KC_BSPC;
	case KC_O: return KC_DEL;
	case KC_J: return KC_DOWN;
	case KC_K: return KC_UP;
	case KC_H: return KC_LEFT;
	case KC_L: return KC_RIGHT;
	case KC_M: return KC_ENT;
	}
	
	return KC_NO;
}

static bool handle_toggle_custom_key_mode(uint16_t keycode, keyrecord_t *record)
{
	if (!record->event.pressed)
		return false;

	/* Ctrl + ESC */
	if (keycode == KC_ESC  && my_control && !my_shift && !alt && !gui) {
		custom_key_mode = !custom_key_mode;
		return true;
	}
	
	return false;
}

static bool handle_show_custom_key_mode(uint16_t keycode, keyrecord_t *record)
{
	if (!record->event.pressed)
		return false;

	/* Ctrl + Shift + ESC */
	if (keycode == KC_ESC && my_control && my_shift && !alt && !gui) {
		uint16_t tmp = unregister_control_mods();
		send_string(custom_key_mode ? "C" : "D");
		if (tmp)
			register_mods(tmp);
		return true;
	}
	
	return false;
}

static bool process_control_custom_key(uint16_t keycode, keyrecord_t *record)
{
	custom_key_processing = false;

	if (handle_show_custom_key_mode(keycode, record))
		return false;

	if (handle_toggle_custom_key_mode(keycode, record))
		return false;

	if (!custom_key_mode) {
		reset_my_control_mods();
		return true;
	}

	if (!my_control)
		return true;

	uint16_t key = get_control_custom_key(keycode);
	
	if (record->event.pressed) {
		if (key != KC_NO && !my_shift && !alt && !gui) {
			custom_key_processing = true;
			unregister_control_mods();
			register_code(key);
			return false;
		} 

		/* Ctrl+a */
		if (my_control && my_shift && !alt && !gui && keycode == KC_A) {
			uint16_t tmp = unregister_shift_mods();

			tap_code(KC_A);

			if (tmp)
				register_mods(tmp);
			return false;
		}
		/*
		 * カスタム定義キー以外のキーが入力された場合、Ctrlの押下状態を物理キーと合わせる。
		 * Ctrl+j を入力中に q を入力した場合は、DOWN→Ctrl+q の流れになる。
		 */
		reset_my_control_mods();
		return true;
	} else {
		if (key != KC_NO && !my_shift && !alt && !gui) {
			unregister_code(key);
			return false;
		}
		return true;
	}
}

static bool register_jis_key(uint16_t keycode)
{
	for (int i = 0; i < sizeof(JIS_KEYCODE_MAP) / sizeof(*JIS_KEYCODE_MAP); i++) {
		if (JIS_KEYCODE_MAP[i].from == keycode && JIS_KEYCODE_MAP[i].with_my_shift == my_shift) {
			if (JIS_KEYCODE_MAP[i].shift) {
				/*
				 * PCが重い状態だと、Shiftのすぐあとにキー入力をすると、Shiftキー入力が判定されない。
				 * waitが入力の妨げにならない程度に待つ。
				 */
				if (register_shift_mods())
					wait_ms(80);
			} else {
				unregister_shift_mods();
			}
			register_code(JIS_KEYCODE_MAP[i].to);
			JIS_KEYCODE_MAP[i].enabled = true;
			return true;
		}
	}
	return false;
}

static bool unregister_jis_key(uint16_t keycode)
{
	for (int i = 0; i < sizeof(JIS_KEYCODE_MAP) / sizeof(*JIS_KEYCODE_MAP); i++) {
		if (JIS_KEYCODE_MAP[i].from == keycode && JIS_KEYCODE_MAP[i].with_my_shift == my_shift) {
			unregister_code(JIS_KEYCODE_MAP[i].to);
			JIS_KEYCODE_MAP[i].enabled = false;
			return true;
		}
	}
	return false;
}

static void unregister_jis_key_all(void)
{
	for (int i = 0; i < sizeof(JIS_KEYCODE_MAP) / sizeof(*JIS_KEYCODE_MAP); i++) {
		if (JIS_KEYCODE_MAP[i].enabled) {
			unregister_code(JIS_KEYCODE_MAP[i].to);
			JIS_KEYCODE_MAP[i].enabled = false;
		}
	}
}

static bool process_my_shift(uint16_t keycode, keyrecord_t *record)
{
	if (keycode != MY_SHIFT)
		return true;

	my_shift = record->event.pressed;

	if (my_shift) {
		/* 変換テーブルにあるキーを入力中にShiftキーを押した場合、
		 * 変換元のキーをunregisterして、変換先のキーをregisterする必要がある。
		 * その動作を再現しても使うことはないため実装しない。 */

		clear_keys();	/* 変換元が入力されつづけても仕方がないため、強制UP。 */
		register_shift_mods();
	} else {
		unregister_shift_mods();
		
		/* 変換元のキーを押しっぱなしの場合は、通常であればもとのキー入力が続く。
		 * その動作を再現しても使い時がないため実装しない。 */
		unregister_jis_key_all();
	}
	return false;
}

static void reset_my_shift_mods(void)
{
	if (my_shift)
		register_shift_mods();
	else
		unregister_shift_mods();
}

static bool process_jis_key(uint16_t keycode, keyrecord_t *record)
{
	if (record->event.pressed) {
		if (register_jis_key(keycode)) {
			return false;
		} else {
			reset_my_shift_mods();
			return true;
		}
	} else {
		if (unregister_jis_key(keycode)) {
			reset_my_shift_mods();
			return false;
		} else {
			reset_my_shift_mods();
			return true;
		}
	}
}

static bool process_shortcut_key(uint16_t keycode, keyrecord_t *record)
{
	if (!record->event.pressed)
		return true;

	/* Windows仮想デスクトップ切替 */
	if (my_control && alt && !my_shift && !gui && (keycode == KC_L || keycode == KC_H)) {
		uint16_t tmp = unregister_alt_mods();
		
		register_mods(MOD_BIT(KC_LGUI));
		tap_code(keycode == KC_L ? KC_RIGHT : KC_LEFT);
		unregister_mods(MOD_BIT(KC_LGUI));
		
		if (tmp)
			register_mods(tmp);
		return false;
	}

	return true;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record)
{
	fetch_mods_status();

	if (!process_my_shift(keycode, record))
		return false;

	if (!process_my_control(keycode, record))
		return false;

	if (!process_jis_key(keycode, record))
		return false;

	if (!process_shortcut_key(keycode, record))
		return false;

	if (!process_control_custom_key(keycode, record))
		return false;
	
	return true;
}
