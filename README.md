# Mimic JIS 2
英語配列のキーボードを日本語設定のOSで使用するための実装です。

## 対応機器
- USB-USB Converter
  https://geekhack.org/index.php?topic=109514.0
  > make converter/usb_usb:mimic_jis2

- HHKB Alt Controller with Mini-B connector
  https://geekhack.org/index.php?topic=71517.0
  > make hhkb/ansi:mimic_jis2

- Keyboard Quantizer Rev3
  https://github.com/sekigon-gonnoc/keyboard-quantizer-doc
  > make keyboard_quantizer/rev3:mimic_jis2

そもまま書き込む場合は、:flashをつける。
> make keyboard_quantizer/rev3:mimic_jis2:flash

## カスタマイズ
以下のカスタマイズが可能です。
`#include "mimic_jis2.c"` の前に、以下のマクロを `#define` してください。

- MIMIC_JIS_KEEP_SCREEN_ENABLE
  - スクリーンロックしないように、定期的に文字入力する機能を有効にします。
  - 標準では、Ctrl+ESC で起動するようになっています。
  - 好みのキーに割り当てる場合は、MY_KEEP_SCREEEN キーをキーマップに設定してください。
- MIMIC_JIS_TILDE_TO_ZKHK_ENABLE
  - チルダキー を 全角/半角 キーに変更します。
  - 左上にチルダキーがあることが多いため、このキーを全角/半角にしたい場合に使用できます。
- MIMIC_JIS_MY_CUSTOM_FEATURE_ENABLE
  - 私の個人的なショートカットです。定義不要です。

## 特殊なキーの説明
内部で使用するための特別なキーコードを定義しています。
キーマップをお好みに変更する場合に活用ください。

### 製品共通
|キー定義|説明|
|-|-|
|MY_CONTROL        |Ctrlキーをハンドルするために使用しています。KC_LCTLなどを置き換えてください。|
|MY_SHIFT          |Shiftキーをハンドルするために使用しています。KC_LSFTなどを置き換えてください。|
|MY_EQL_PLUS       |英字キーの[=/+]キーをJIS配列向きの出力に変換するために使用します。|
|MY_MIN_USCRE      |英字キーの[-/_]キーをJIS配列向きの出力に変換するために使用します。|
|MY_TILD_BQUOT     |英字キーの[`/~]キーをJIS配列向きの出力に変換するために使用します。|
|MY_QUOT_DQUOT     |英字キーの['/"]キーをJIS配列向きの出力に変換するために使用します。|
|MY_KEEP_SCREEEN   |スクリーン維持機能の切り替え用のキーです。|
|MY_ZENKAKU_HANKAKU|[全角/半角]キーを出力します。|
|MY_MOD_1          |個人的なショートカットです。|
|MY_MOD_2          |個人的なショートカットです。|
|MY_MOD_3          |個人的なショートカットです。|

### HHKB Alt Controller専用
HHKB Alt Controllerは、取り外しが簡単にはできないため、
Win/Mac切替やUS/JIS切替ができるようにしています。

|キー定義|説明|
|-|-|
|MY_TOGGLE_OS      |WindowsとMac向けの入力を切り替えます。HHKB以外は意味ありません。|
|MY_TOGGLE_LANG    |US=>JIS変換するかしないかを切り替えます。HHKB以外は意味ありません。|


## ビルド手順
### qmk_firmware本家のソースコードを使用するデバイス
対象デバイス
  - HHKB Alt Controller with Mini-B connector
  - USB-USB Converter

https://github.com/qmk/qmk_firmware

対応時点:
```
commit 6ed605a614595bf01721070e7ed6407e8e19585e (HEAD -> master, origin/master, origin/HEAD)
Author: Joel Challis <git@zvecr.com>
Date:   Fri Nov 25 04:29:20 2022 +0000

    Fix annepro2/c18:iso_default (#19147)
```

qmk_firmware と mimic_jis を同じディレクトリに配置してください。

```
.
├ qmk_firmware
└ qmk_mimic_jis
```

ソースコードを配置します。
```
cd qmk_firmware
ln -s ../qmk_mimic_jis/mimic_jis2 .
ln -s ../../../../../qmk_mimic_jis/keyboards/hhkb/ansi/keymaps/mimic_jis2 keyboards/hhkb/ansi/keymaps/
ln -s ../../../../../qmk_mimic_jis/keyboards/converter/usb_usb/keymaps/mimic_jis2 keyboards/converter/usb_usb/keymaps/
make converter/usb_usb:mimic_jis2
```

ビルドします。
```
make hhkb/ansi:mimic_jis2
make converter/usb_usb:mimic_jis2
```
### Keyboard Quantizer
対象デバイス
  - Keyboard Quantizer Rev3

対応時点:
```
commit 240038525b2cbdbc3a1cbbec47399a1dfd498ed1 (HEAD -> dev/sekigon, origin/dev/sekigon)
Author: sekigon-gonnoc <43873124+sekigon-gonnoc@users.noreply.github.com>
Date:   Sun Aug 8 16:23:11 2021 +0900

    quantizer: support SlimBlade button 3, 4
```

手順:
```
git clone https://github.com/sekigon-gonnoc/qmk_firmware.git -b dev/sekigon qmk_quantizer
cd qmk_quantizer
ln -s ../qmk_mimic_jis/mimic_jis2 .
ln -s ../../../../qmk_mimic_jis/keyboards/keyboard_quantizer/keymaps/mimic_jis2 keyboards/keyboard_quantizer/keymaps/
```

keyboards/keyboard_quantizer/keyboard_quantizer.hを修正します。
```
diff --git a/keyboards/keyboard_quantizer/keyboard_quantizer.h b/keyboards/keyboard_quantizer/keyboard_quantizer.h
index 278da053d5..6daf780ba6 100644
--- a/keyboards/keyboard_quantizer/keyboard_quantizer.h
+++ b/keyboards/keyboard_quantizer/keyboard_quantizer.h
@@ -79,7 +79,7 @@ lctrl, lgui, lalt, mhen, spc, henk, kana, ralt, rgui, app, rctrl,        left, d
     { u, v, w, x, y, z, k1, k2}, \
     { k3, k4, k5, k6, k7, k8, k9, k0}, \
     {ent, esc, bspc, tab, spc, min, circ, at}, \
-    {lbrc, KC_BSLASH, rbrc, scln, cln, hanzen, comm, dot}, \
+    {lbrc, yen, rbrc, scln, cln, hanzen, comm, dot}, \
     {slsh, caps, f1, f2, f3, f4, f5, f6}, \
     {f7, f8, f9, f10, f11, f12, pscr, scrl}, \
     {pause, ins, home, pageup, del, end, pagedown, right}, \
```

```
make keyboard_quantizer/rev3:mimic_jis2
```

# Keyboard Quantizer Rev3のメモ
タイムアウトを伸ばしたい場合は、以下のようにする。

```
diff --git a/keyboards/keyboard_quantizer/rev3/matrix.c b/keyboards/keyboard_quantizer/rev3/matrix.c
index f9a5e4eac2..a7e11d7a05 100644
--- a/keyboards/keyboard_quantizer/rev3/matrix.c
+++ b/keyboards/keyboard_quantizer/rev3/matrix.c
@@ -23,7 +23,7 @@ along with this program.  If not, see <http://www.gnu.org/licenses/>.
 #include "keyboard_quantizer.h"

 bool no_com_from_start = true;
-#define KEYBOARD_NOCOM_TIMEOUT 3000
+#define KEYBOARD_NOCOM_TIMEOUT 10000
 #define CH559_NOCOM_TIMEOUT 30000

 extern const bool    ch559_start;
```
