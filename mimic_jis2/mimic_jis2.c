#ifdef MIMIC_JIS_KEEP_SCREEN_ENABLE
  #include "timer.h"
#endif

#define JIS    0
#define US     2
#define HHKB   3

enum my_keycodes {
	MY_CONTROL = SAFE_RANGE,
	MY_SHIFT,
	MY_EQL_PLUS,
	MY_MIN_USCRE,
	MY_TILD_BQUOT,
	MY_QUOT_DQUOT,
	MY_KEEP_SCREEEN,
	MY_TOGGLE_OS,
	MY_TOGGLE_LANG,
	MY_ZENKAKU_HANKAKU,
	MY_MOD_1,
	MY_MOD_2,
	MY_MOD_3,
};

static uint32_t last_keyinput_time;

static bool mod_control;
static bool mod_shift;
static bool mod_alt;
static bool mod_gui;

static bool my_control;
static bool my_shift;

static void fetch_mods_status(void)
{
	mod_control = get_mods() & MOD_MASK_CTRL;
	mod_shift   = get_mods() & MOD_MASK_SHIFT;
	mod_alt     = get_mods() & MOD_MASK_ALT;
	mod_gui     = get_mods() & MOD_MASK_GUI;
}

static bool register_control_mods(void)
{
	if (!(get_mods() & MOD_MASK_CTRL)) {
		register_mods(MOD_BIT(KC_LCTL));
		return true;
	}
	return false;
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

static void reset_my_shift_mods(void)
{
	if (my_shift)
		register_shift_mods();
	else
		unregister_shift_mods();
}

static bool register_alt_mods(void)
{
	if (!(get_mods() & MOD_MASK_ALT)) {
		register_mods(MOD_BIT(KC_LALT));
		return true;
	}

	return false;
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

typedef enum {
	KS_TRANS = -1,
	KS_OFF  = 0,
	KS_ON   = 1,
} KEY_STATUS;

struct translation {
	KEY_STATUS from_control;
	KEY_STATUS from_alt;
	KEY_STATUS from_shift;
	uint16_t from_keycode;
	KEY_STATUS to_control;
	KEY_STATUS to_alt;
	KEY_STATUS to_shift;
	uint16_t to_keycode;
	bool *flg_enabled;
	void (*callback)(struct translation *);
};

#ifdef MIMIC_JIS_MY_CUSTOM_FEATURE_ENABLE
static bool flg_control_custom_key_enabled = false;
#endif

#ifdef MIMIC_JIS_OS_WIN_MAC_TOGGLE_ENABLE
static bool flg_os_windows = true;
#endif

static bool flg_jis_mode = true;

#ifdef MIMIC_JIS_MY_CUSTOM_FEATURE_ENABLE
static void cb_toggle_custom_key_mode(struct translation *trans)
{
	flg_control_custom_key_enabled = !flg_control_custom_key_enabled;
}
#endif

#ifdef MIMIC_JIS_MY_CUSTOM_FEATURE_ENABLE
static void cb_show_mode(struct translation *trans)
{
	uint16_t tmp = unregister_control_mods();
	send_string("<");
	send_string(flg_control_custom_key_enabled ? "Cust" : "Dft");
#ifdef MIMIC_JIS_LAYOUT_JIS_US_TOGGLE_ENABLE
	send_string(",");
	send_string(flg_jis_mode ? "JIS" : "US");
#endif
#ifdef MIMIC_JIS_OS_WIN_MAC_TOGGLE_ENABLE
	send_string(",");
	send_string(flg_os_windows ? "WIN" : "MAC");
#endif
	send_string(">");
	if (tmp)
		register_mods(tmp);
}
#endif

#ifdef MIMIC_JIS_MY_CUSTOM_FEATURE_ENABLE
static void cb_win_vdesktop(struct translation *trans)
{
	uint16_t tmp = unregister_alt_mods();

	register_mods(MOD_BIT(KC_LGUI));
	tap_code(trans->from_keycode == KC_L ? KC_RIGHT : KC_LEFT);
	unregister_mods(MOD_BIT(KC_LGUI));

	if (tmp)
		register_mods(tmp);
}
#endif

#ifdef MIMIC_JIS_OS_WIN_MAC_TOGGLE_ENABLE
static void cb_toggle_os_win_mac(struct translation *trans)
{
	flg_os_windows = !flg_os_windows;
}
#endif

#ifdef MIMIC_JIS_LAYOUT_JIS_US_TOGGLE_ENABLE
static void cb_toggle_layout_jis_us(struct translation *trans)
{
	flg_jis_mode = !flg_jis_mode;

	if (flg_jis_mode)
		layer_move(JIS);
	else
		layer_move(US);
}
#endif

#ifdef MIMIC_JIS_KEEP_SCREEN_ENABLE
static enum {
	KEEP_SCREEN_DISABLED,
	KEEP_SCREEN_MESSAGE,
	KEEP_SCREEN_MOD
} keep_screen_mode = KEEP_SCREEN_DISABLED;
static const char keep_screen_message[] = "This message is keeping screen.";
static int keep_screen_pos = 0;

static void cb_toggle_keep_screen_message(struct translation *trans)
{
	if (keep_screen_mode == KEEP_SCREEN_MESSAGE) {
		keep_screen_mode = KEEP_SCREEN_DISABLED;
		return;
	}
	keep_screen_mode = KEEP_SCREEN_MESSAGE;
	keep_screen_pos = 0;
	send_string("]KM\\");
}

static void cb_toggle_keep_screen_mod(struct translation *trans)
{
	if (keep_screen_mode == KEEP_SCREEN_MOD) {
		keep_screen_mode = KEEP_SCREEN_DISABLED;
		return;
	}
	keep_screen_mode = KEEP_SCREEN_MOD;
	keep_screen_pos = 0;
	unregister_control_mods();
	wait_ms(80);
	send_string("]KC\\");
	reset_my_control_mods();
}

void matrix_scan_user(void)
{
	static uint16_t keep_screen_timer = 0;

	if (!keep_screen_mode)
		return;

	if (keep_screen_mode == KEEP_SCREEN_MESSAGE) {
		uint16_t e = timer_elapsed(keep_screen_timer);

		if (e > 5 * 1000) {
			/* Every 5 second */
			keep_screen_timer += e/1000*1000;

			char c = keep_screen_message[keep_screen_pos];
			if (c == 'y' || c == 'Y' || c == 'n' || c == 'N') {
				send_char('"');				 /* * */
			} else if (c == ' ') {
				register_shift_mods();
				tap_code(JP_BSLS);			 /* _ */
				unregister_shift_mods();
			} else {
				send_char(c);
			}

			if (keep_screen_message[keep_screen_pos + 1] == '\0')
				keep_screen_pos = 0;
			else
				keep_screen_pos += 1;
		}
	}

	static uint32_t next_keep_time = 0;
	static uint32_t start_keep_time = 0;
	static uint16_t keep_count = 0;
#define KEEP_MOD_INTERVAL ((uint32_t)270 * 1000)
	if (keep_screen_mode == KEEP_SCREEN_MOD) {
		if (next_keep_time < last_keyinput_time)
			next_keep_time = last_keyinput_time + KEEP_MOD_INTERVAL;

		if (timer_elapsed32(last_keyinput_time) > KEEP_MOD_INTERVAL &&
			next_keep_time < timer_read32()) {
			next_keep_time += KEEP_MOD_INTERVAL;
			if (start_keep_time != last_keyinput_time) {
				start_keep_time = last_keyinput_time;
				keep_count = 1;
			}
			if (keep_count % 5 == 0) {
				register_alt_mods();
				wait_ms(500);
				unregister_alt_mods();
			} else {
				register_shift_mods();
				wait_ms(500);
				unregister_shift_mods();
			}
			keep_count++;
		}
	}
}
#endif

#define KS____ KS_TRANS
static struct translation TRANSLATION_MAP[] = {
	/* US -> JIS translation map */
	/* CTL    ALT     SHIFT   KEYCODE                 CTL     ALT     SHIFT   KEYCODE */
	{ KS____, KS____, KS_OFF, MY_EQL_PLUS  , /* -> */ KS____, KS____, KS_ON , JP_MINS, &flg_jis_mode }, /* = */
	{ KS____, KS____, KS_ON , MY_EQL_PLUS  , /* -> */ KS____, KS____, KS_ON , JP_SCLN, &flg_jis_mode }, /* + */
	{ KS____, KS____, KS_OFF, MY_MIN_USCRE , /* -> */ KS____, KS____, KS_OFF, JP_MINS, &flg_jis_mode }, /* - */
	{ KS____, KS____, KS_ON , MY_MIN_USCRE , /* -> */ KS____, KS____, KS_ON , JP_BSLS, &flg_jis_mode }, /* _ */
#ifdef MIMIC_JIS_TILDE_TO_ZKHK_ENABLE
	{ KS____, KS____, KS_OFF, KC_CAPS      , /* -> */ KS____, KS____, KS_ON , JP_AT  , &flg_jis_mode }, /* ` */
	{ KS____, KS____, KS_ON , KC_CAPS      , /* -> */ KS____, KS____, KS_ON , JP_CIRC, &flg_jis_mode }, /* ~ */
	{ KS____, KS____, KS____, KC_APP       , /* -> */ KS____, KS____, KS____, KC_CAPS, &flg_jis_mode },
	{ KS____, KS____, KS____, MY_TILD_BQUOT, /* -> */ KS____, KS____, KS____, KC_ZKHK, &flg_jis_mode },
#else
	{ KS____, KS____, KS_OFF, MY_TILD_BQUOT, /* -> */ KS____, KS____, KS_ON , JP_AT  , &flg_jis_mode }, /* ` */
	{ KS____, KS____, KS_ON , MY_TILD_BQUOT, /* -> */ KS____, KS____, KS_ON , JP_CIRC, &flg_jis_mode }, /* ~ */
#endif
	{ KS____, KS____, KS_OFF, MY_QUOT_DQUOT, /* -> */ KS____, KS____, KS_ON , KC_7   , &flg_jis_mode }, /* ' */
	{ KS____, KS____, KS_ON , MY_QUOT_DQUOT, /* -> */ KS____, KS____, KS_ON , KC_2   , &flg_jis_mode }, /* " */
	{ KS____, KS____, KS_ON , KC_2         , /* -> */ KS____, KS____, KS_OFF, JP_AT  , &flg_jis_mode }, /* @ */
	{ KS____, KS____, KS_ON , KC_6         , /* -> */ KS____, KS____, KS_OFF, JP_CIRC, &flg_jis_mode }, /* ^ */
	{ KS____, KS____, KS_ON , KC_7         , /* -> */ KS____, KS____, KS_ON , KC_6   , &flg_jis_mode }, /* & */
	{ KS____, KS____, KS_ON , KC_8         , /* -> */ KS____, KS____, KS_ON , JP_COLN, &flg_jis_mode }, /* * */
	{ KS____, KS____, KS_ON , KC_9         , /* -> */ KS____, KS____, KS_ON , KC_8   , &flg_jis_mode }, /* ( */
	{ KS____, KS____, KS_ON , KC_0         , /* -> */ KS____, KS____, KS_ON , KC_9   , &flg_jis_mode }, /* ) */
	{ KS____, KS____, KS_ON , KC_SCLN      , /* -> */ KS____, KS____, KS_OFF, JP_COLN, &flg_jis_mode }, /* : */

#ifdef MIMIC_JIS_ALT_SPC_TO_ZKHK_ENABLE
	{ KS____, KS_ON , KS_OFF, KC_SPC       , /* -> */ KS____, KS____, KS____, KC_ZKHK, &flg_jis_mode }, /* M-SPC -> Zenkaku/Hankaku */
#endif
#ifdef MIMIC_JIS_SHIFT_SPC_TO_ZKHK_ENABLE
	{ KS____, KS____, KS_ON , KC_SPC       , /* -> */ KS____, KS____, KS_OFF, KC_ZKHK, &flg_jis_mode }, /* S-SPC -> Zenkaku/Hankaku */
#endif

#ifdef MIMIC_JIS_MY_CUSTOM_FEATURE_ENABLE
	/* Custom key */
	/* CTL    ALT     SHIFT   KEYCODE          CTL     ALT     SHIFT   KEYCODE   FLAG  CALLBACK */
	{ KS_ON , KS____, KS_OFF, KC_ESC,          KS____, KS____, KS____, KC_NO   , NULL, cb_toggle_custom_key_mode }, /* C-ESC              */
	{ KS_ON , KS____, KS_ON , KC_ESC,          KS____, KS____, KS____, KC_NO   , NULL, cb_show_mode              }, /* C-S-ESC            */
	{ KS_ON , KS____, KS_ON , KC_A  , /* -> */ KS_ON , KS____, KS_OFF, KC_A    , &flg_control_custom_key_enabled }, /* C-S-a => C-a       */
	{ KS_ON , KS____, KS____, KC_A  , /* -> */ KS_OFF, KS____, KS____, KC_HOME , &flg_control_custom_key_enabled }, /* C-a   -> HOME      */
	{ KS_ON , KS____, KS____, KC_E  , /* -> */ KS_OFF, KS____, KS____, KC_END  , &flg_control_custom_key_enabled }, /* C-e   -> END       */
	{ KS_ON , KS____, KS____, KC_D  , /* -> */ KS_OFF, KS____, KS____, KC_DEL  , &flg_control_custom_key_enabled }, /* C-d   -> Delete    */
	{ KS_ON , KS____, KS____, KC_U  , /* -> */ KS_OFF, KS____, KS____, KC_BSPC , &flg_control_custom_key_enabled }, /* C-u   -> Backspace */
	{ KS_ON , KS____, KS____, KC_O  , /* -> */ KS_OFF, KS____, KS____, KC_DEL  , &flg_control_custom_key_enabled }, /* C-o   -> Delete    */
	{ KS_ON , KS____, KS____, KC_J  , /* -> */ KS_OFF, KS____, KS____, KC_DOWN , &flg_control_custom_key_enabled }, /* C-j   -> Down      */
	{ KS_ON , KS____, KS____, KC_K  , /* -> */ KS_OFF, KS____, KS____, KC_UP   , &flg_control_custom_key_enabled }, /* C-k   -> Up        */
	{ KS_ON , KS____, KS____, KC_H  , /* -> */ KS_OFF, KS____, KS____, KC_LEFT , &flg_control_custom_key_enabled }, /* C-h   -> Left      */
	{ KS_ON , KS____, KS____, KC_L  , /* -> */ KS_OFF, KS____, KS____, KC_RIGHT, &flg_control_custom_key_enabled }, /* C-l   -> Right     */
	{ KS_ON , KS____, KS____, KC_M  , /* -> */ KS_OFF, KS____, KS____, KC_ENT  , &flg_control_custom_key_enabled }, /* C-m   -> Enter     */

	/* Shortcut key */
	/* CTL    ALT     SHIFT   KEY            CTL     ALT     SHIFT   KEY    FLAG  CALLBACK */
	{ KS_ON , KS_ON , KS_OFF, KC_L, /* -> */ KS____, KS____, KS____, KC_NO, &flg_control_custom_key_enabled, cb_win_vdesktop }, /* C-A-l */
	{ KS_ON , KS_ON , KS_OFF, KC_H, /* -> */ KS____, KS____, KS____, KC_NO, &flg_control_custom_key_enabled, cb_win_vdesktop }, /* C-A-h */
#endif

#ifdef MIMIC_JIS_KEEP_SCREEN_ENABLE
	/* Keep screen key */
	/* CTL    ALT     SHIFT   KEYCODE                   CTL     ALT     SHIFT   KEY    FLAG  CALLBACK */
	{ KS_OFF, KS_OFF, KS_OFF, MY_KEEP_SCREEEN, /* -> */ KS____, KS____, KS____, KC_NO, NULL, cb_toggle_keep_screen_message },
	{ KS_ON , KS_OFF, KS_OFF, MY_KEEP_SCREEEN, /* -> */ KS____, KS____, KS____, KC_NO, NULL, cb_toggle_keep_screen_mod },
#endif

	/* Toggle */
	/* CTL   ALT     SHIFT  KEYCODE                  CTL     ALT     SHIFT   KEY    FLAG  CALLBACK */
#ifdef MIMIC_JIS_OS_WIN_MAC_TOGGLE_ENABLE
	{ KS_ON, KS____, KS_ON, MY_TOGGLE_OS,   /* -> */ KS____, KS____, KS____, KC_NO, NULL, cb_toggle_os_win_mac },
#endif
#ifdef MIMIC_JIS_LAYOUT_JIS_US_TOGGLE_ENABLE
	{ KS_ON, KS____, KS_ON, MY_TOGGLE_LANG, /* -> */ KS____, KS____, KS____, KC_NO, NULL, cb_toggle_layout_jis_us },
#endif
};
#undef KS____

static struct translation *translating = NULL;

static struct translation *find_translation_key(uint16_t keycode)
{
	struct translation *trans;

	if (!my_shift && !my_control && !mod_alt) {
		if (KC_A <= keycode && keycode <= KC_0)
			return NULL;
	}

	for (int i = 0; i < sizeof(TRANSLATION_MAP) / sizeof(*TRANSLATION_MAP); i++) {
		trans = &TRANSLATION_MAP[i];

		if (trans->from_keycode != keycode)
			continue;

		if (trans->flg_enabled != NULL && !*trans->flg_enabled)
			continue;

		if (trans->from_shift != KS_TRANS &&
			((trans->from_shift == KS_ON && !my_shift) ||
			 (trans->from_shift == KS_OFF && my_shift)))
			continue;

		if (trans->from_control != KS_TRANS &&
			((trans->from_control == KS_ON && !my_control) ||
			 (trans->from_control == KS_OFF && my_control)))
			continue;

		if (trans->from_alt != KS_TRANS &&
			((trans->from_alt == KS_ON && !mod_alt) ||
			 (trans->from_alt == KS_OFF && mod_alt)))
			continue;

		return trans;
	}

	return NULL;
}

static void disable_translation_key(struct translation *trans)
{
	unregister_code(trans->to_keycode);
	reset_my_control_mods();
	reset_my_shift_mods();
}

static void enable_translation_key(struct translation *trans)
{
	bool should_wait = false;

	if (trans->to_shift != KS_TRANS) {
		if (trans->to_shift)
			should_wait |= register_shift_mods();
		else
			should_wait |= unregister_shift_mods();
	}

	if (trans->to_control != KS_TRANS) {
		if (trans->to_control)
			should_wait |= register_control_mods();
		else
			should_wait |= unregister_control_mods();
	}

	if (should_wait)
		wait_ms(80);

	register_code(trans->to_keycode);
}

static void reset_translation_key(void)
{
	if (translating)
		disable_translation_key(translating);
	translating = NULL;
}

static void do_translation_key(struct translation *trans)
{
	reset_translation_key();

	if (trans->callback) {
		trans->callback(trans);
	} else {
		enable_translation_key(trans);
		translating = trans;
	}
}


static bool process_translation_key(uint16_t keycode, keyrecord_t *record)
{
	struct translation *trans = find_translation_key(keycode);

	if (trans == NULL) {
		if (record->event.pressed) {
			/* 変換とは関係のないキーが入力された場合、
			 * 現在の変換を無効化して入力されたキーを有効にする。 */
			reset_translation_key();
			return true;
		}
	} else if (trans == translating) {
		if (!record->event.pressed) {
			reset_translation_key();
		}
	} else {
		/* 過去に変換した可能性のあるキー処理 */

		if (record->event.pressed) {
			/* 変換の新規実施 or 変換の上書き */
			do_translation_key(trans);
			return false;
		} else {
			/* CtrlやShiftのUpはここには来ない。
			 * 変換とは関係ないキーが離された場合のため、デフォルトの動作に任せる。
			 * 別の変換が実施中の可能性があるため、変換の無効化はおこなわない。 */
			return true;
		}
	}
	return true;
}

static bool process_my_control(uint16_t keycode, keyrecord_t *record)
{
	if (keycode != MY_CONTROL) {
		return true;
	}

	my_control = record->event.pressed;
	reset_translation_key();
	reset_my_control_mods();

	return false;
}

static bool process_my_shift(uint16_t keycode, keyrecord_t *record)
{
	if (keycode != MY_SHIFT)
		return true;

	my_shift = record->event.pressed;
	reset_translation_key();
	reset_my_shift_mods();

	return false;
}

#ifdef MIMIC_JIS_NATIVE_FN_ENABLE
static bool process_my_mod(uint16_t keycode, keyrecord_t *record)
{
	if (keycode < MY_MOD_1 || keycode > MY_MOD_3)
		return true;

	uint16_t actual_key = KC_NO;

	switch (keycode) {
	case MY_MOD_1:
		actual_key = (flg_os_windows ? KC_LALT : KC_LGUI);
		break;

	case MY_MOD_2:
		actual_key = (flg_os_windows ? KC_RALT : KC_RGUI);
		break;

	case MY_MOD_3:
		actual_key = (flg_os_windows ? KC_LGUI : KC_RALT);
		break;

	default:
		return true;
	}

	if (record->event.pressed)
		register_mods(MOD_BIT(actual_key));
	else
		unregister_mods(MOD_BIT(actual_key));

	return false;
}
#endif

static bool process_my_special_key(uint16_t keycode, keyrecord_t *record)
{
	if (!(keycode == MY_ZENKAKU_HANKAKU && record->event.pressed))
		return true;

	register_alt_mods();
	wait_ms(80);
	tap_code(KC_GRV);
	unregister_alt_mods();

	return false;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record)
{
	last_keyinput_time = timer_read32();
	fetch_mods_status();

	if (!process_my_control(keycode, record))
		return false;

	if (!process_my_shift(keycode, record))
		return false;

#ifdef MIMIC_JIS_NATIVE_FN_ENABLE
	if (!process_my_mod(keycode, record))
		return false;
#endif

	if (!process_my_special_key(keycode, record))
		return false;
	
	if (!process_translation_key(keycode, record))
		return false;
	
	return true;
}
