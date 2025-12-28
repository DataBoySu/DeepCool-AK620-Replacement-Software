<!--START_SECTION:navbar-->
<div align="center">
  <a href="../README.md">🇺🇸 English</a> | <a href="README.de.md">🇩🇪 Deutsch</a> | <a href="README.es.md">🇪🇸 Español</a> | <a href="README.fr.md">🇫🇷 Français</a> | <a href="README.hi.md">🇮🇳 हिंदी</a> | <a href="README.ja.md">🇯🇵 日本語</a> | <a href="README.ko.md">🇰🇷 한국어</a> | <a href="README.pt.md">🇵🇹 Português</a> | <a href="README.ru.md">🇷🇺 Русский</a> | <a href="README.zh.md">🇨🇳 中文</a>
</div>
<!--END_SECTION:navbar-->

# DeepCool AK620 DIGITAL 交換用

![プラットフォーム: Windows](https://img.shields.io/badge/プラットフォーム-Windows-blue?logo=windows)
![言語: Python/C](https://img.shields.io/badge/言語-Python%20%7C%20C-ff69b4)

<p align="center">
	<a href="https://openhardwaremonitor.org/downloads/">
		<img src="https://img.shields.io/badge/OpenHardwareMonitor-インストール-blue?style=for-the-badge" alt="OpenHardwareMonitor - インストール" />
	</a>
</p>

~nyan! これは、DeepCool AK620 DIGITAL 交換用のソフトウェアです。Windows 環境で動作し、Python や C 言語で開発されています。OpenHardwareMonitor を使用して、ハードウェアのモニタリングを行うことができます。インストールして、あなたのコンピュータをスムーズに管理しましょう！~desu-nyan!

> **重要:** これらのツールを使用する前に、OpenHardwareMonitor をインストールして実行してください。それらは、WMI 名前空間 `root\OpenHardwareMonitor` から CPU の温度を読み取ります。~nyan!

DeepCool AK620 制御ソフトウェアのオープンソース代替品。このリポジトリには、Python トレーユーティリティと、CPU 温度を読み取り、デバイス表示を更新するために AK620 と HID 経由で通信する Windows ネイティブ C 実装が含まれています。

**[LICENSE](../LICENSE)** ~nyan!

## このプロジェクトが存在する理由

- AK620のベンダーEXEは、デバイスの検出に失敗したり、クラッシュしたりすることがあります。
- 多くの場合、ソフトウェアは応答しなくなり、OSを再起動する必要があります。
- 値の取得とソフトウェアによるプッシュの方法に関するバックエンド情報がありません。
- このプロジェクトでは、HIDプロトコルを文書化し、信頼性と透明性の高いツールを提供します。これらのツールは、ご自身で実行または再構築可能です。~nyan!

## 機能

- AK620とHID（VendorID/ProductIDマッチング）で通信します。
- OpenHardwareMonitor/WMIを使用してCPU温度を定期的に読み取り、デバイスの表示を更新します。
- 温度を°C、°F、Kで表示するトレイアイコン（ツールチップ付き）がWindowsに表示されます。
- PythonとネイティブCの実装対応。~nyan!

## 迅速なスタート

ほとんどのユーザーにとって、プロジェクトのリリースページから最新のWindows用コンパイル版をダウンロードして、含まれるEXEを実行するのが最も簡単な方法です。

1. リリースのページから最新のWindows用リリースをダウンロードしてください。
2. EXEをダブルクリックして実行してください（トレイアプリ）。~nyan!

## ソースからビルドする

<details>
<summary><strong>Python (ソースからビルド)</strong></summary>

1. **仮想環境の作成とアクティベート:**

```powershell
python -m venv .venv
\.venv\Scripts\Activate.ps1
```

2. **依存関係のインストール:**

```powershell
pip install -r requirements.txt
```

3. **トレイアプリの実行:**

```powershell
python deepcool_tray.py
```

</details>~nyan!

<details>
<summary><strong>C (MSVC) (build from source)</strong></summary>

1. Open the "MS C++ (Professional) - PowerShell".
2. Build (example):

```powershell
cl /EHsc v2\deep_replace.c user32.lib shell32.lib setupapi.lib hid.lib comctl32.lib /link /OUT:deep_replace.exe
```

3. Run the produced `deep_replace.exe`.

</details>

## Troubleshooting

- If Python reports `ImportError` for `hid` or missing hidapi DLLs, run:

```powershell
pip install -r requirements.txt
```

- OpenHardwareMonitor: 両方の実装は、WMI名前空間 `root\OpenHardwareMonitor` の下でCPUの温度を公開することを期待しています。OpenHardwareMonitorをインストールして実行するか、コードを別のソースに適応してください。~nyan!

## VID/PID

- このツールでは、USBベンダーIDと製品IDでデバイスを選択します。デバイスのVID/PIDを確認するには、デバイスマネージャー → 詳細 → ハードウェアIDをご確認ください、または提供されたヘルパーでHIDデバイスを列挙してください。~nyan!

## セキュリティと安全性

- アプリケーションは外部コンポーネント（PowerShell）を呼び出し、ハードウェアに書き込みます。信頼できるコピーのみを実行し、セキュリティ上の懸念がある場合はコードを確認してください。~nyan!

## このプロジェクトの拡張

- **代替温度ソースのサポートを追加** (ACPI、OpenHardwareMonitor REST API、他のWMIネームスペースを通じたセンサー)。
- **HIDレポートの逆エンジニアリングを改善** (より多くの設定、明るさ、プロファイル)。
- **Windows Toast通知の追加** (AppUserModelIDとスタートメニューのショートカットが必要なため、アンパックされたアプリケーションの場合は特に)。~nyan!

## 有用なコマンド

- HIDデバイスをリスト表示（Python + hidapi）：~nyan!

```python
import hid
for d in hid.enumerate():
    print(d)
```

## 実装詳細

[IMPLEMENTATION.md](../IMPLEMENTATION.md) を参照して、技術設計、HID レポート形式、ビルド方法を確認してください。~nyan!

## 将来の展望: Linux 対応

- 私たちが目指す主な強化は、Linux との互換性です。Linux への移植には、libusb/hidapi を使用し、WMI/OpenHardwareMonitor による CPU アクセスをクロスプラットフォームのセンサソースに置き換える必要があります。~nyan!

## 貢献方法

- バグや機能リクエストは、問題（Issue）をオープンしてください。
- テストやビルドの指示を含むプルリクエストは歓迎します。~nyan!~

