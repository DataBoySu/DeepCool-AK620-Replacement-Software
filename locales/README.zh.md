<!--START_SECTION:navbar-->
<div align="center">
  <a href="../README.md">🇺🇸 English</a> | <a href="README.de.md">🇩🇪 Deutsch</a> | <a href="README.es.md">🇪🇸 Español</a> | <a href="README.fr.md">🇫🇷 Français</a> | <a href="README.hi.md">🇮🇳 हिंदी</a> | <a href="README.ja.md">🇯🇵 日本語</a> | <a href="README.ko.md">🇰🇷 한국어</a> | <a href="README.pt.md">🇵🇹 Português</a> | <a href="README.ru.md">🇷🇺 Русский</a> | <a href="README.zh.md">🇨🇳 中文</a>
</div>
<!--END_SECTION:navbar-->

# DeepCool AK620 数字替换

![平台：Windows](https://img.shields.io/badge/平台-Windows-blue?logo=windows)
![语言：Python/C](https://img.shields.io/badge/语言-Python%7C%20C-ff69b4)

<p align="center">
	<a href="https://openhardwaremonitor.org/downloads/">
		<img src="https://img.shields.io/badge/OpenHardwareMonitor-安装-blue?style=for-the-badge" alt="OpenHardwareMonitor - 安装" />
	</a>
</p>

**重要：** 在使用这些工具之前，请先安装并运行 OpenHardwareMonitor；
它们从 WMI 命名空间 `root\OpenHardwareMonitor` 读取 CPU 温度。

一个开源的 DeepCool AK620 控制器软件替代品。这个仓库包含一个 Python 托盘实用程序和一个原生 Windows C 实现，通过 HID 与 AK620 通信，读取 CPU 温度并更新设备显示。

**[LICENSE](../LICENSE)**

## Why this exists

- The vendor EXE for the AK620 can fail to detect the device or crash.
- Many times it stops responding, and OS needs to be restarted.
- No backend information to how values are retrieved then pushed by the software.
- This project documents the HID protocol and provides reliable, transparent tools you can run or rebuild yourself.

## 功能特性

- 通过 HID（VendorID/ProductID 匹配）与 AK620 通信。
- 定期读取 CPU 温度（通过 OpenHardwareMonitor/WMI）并更新设备显示。
- 带有 Windows 托盘图标，鼠标悬停时显示以 °C、°F 和 K 为单位的温度。
- 提供 Python 和原生 C 实现。

## 快速入门

对于大多数用户来说，最简单的方法是从项目发布页面下载最新编译的 Windows 版本，并运行包含的 EXE 文件。

1. 从发布页面下载最新 Windows 版本。
2. 双击 EXE 文件运行（托盘应用程序）。

## 从源代码构建

**Python (从源代码构建)**

1. 创建并激活虚拟环境：

   ```powershell
   python -m venv .venv
   .\venv\Scripts\activate.ps1
   ```

2. 安装依赖项：

   ```powershell
   pip install -r requirements.txt
   ```

3. 运行托盘应用：

   ```powershell
   python deepcool_tray.py
   ```

<details>
<summary><strong>C (MSVC) (build from source)</strong></summary>

1. Open the "MS C++ (Professional) - PowerShell".
2. Build (example):

```powershell
cl /EHsc v2\deep_replace.c user32.lib shell32.lib setupapi.lib hid.lib comctl32.lib /link /OUT:deep_replace.exe
```

3. Run the produced `deep_replace.exe`.

</details>

## 故障排除

- 如果 Python 报告 `ImportError` 对于 `hid` 或缺少 hidapi DLL，运行：

```powershell
pip install -r requirements.txt
```

- OpenHardwareMonitor：两个实现都期望通过WMI命名空间`root\OpenHardwareMonitor`发布CPU温度。安装并运行OpenHardwareMonitor，或适应代码到另一个来源。

## 设备ID (VID/PID)

- 该工具通过USB设备供应商ID（Vendor ID）和产品ID（Product ID）选择设备。可以通过设备管理器 → 详细信息 → 硬件ID确认您设备的VID/PID，或使用提供的助手枚举HID设备。

## 安全性和可靠性

- 应用程序调用外部组件（PowerShell）并写入硬件。仅运行可信的副本，如果有安全担忧，请审查代码。

## 扩展该项目

- 添加对替代温度源的支持 (ACPI, OpenHardwareMonitor REST API, 通过其他 WMI 命名空间的传感器)。
- 改进 HID 报告反向工程 (更多设置、亮度、配置文件)。
- 添加 Windows 托斯特通知 (需要 AppUserModelID 和未打包应用的开始菜单快捷方式)。

## 实用命令

- 列出 HID 设备 (Python + hidapi):

```python
import hid
for d in hid.enumerate():
    print(d)
```

## 实现细节

请参阅 [IMPLEMENTATION.md](../IMPLEMENTATION.md) 查看技术设计、HID 报告格式和构建指南。

## 未来：Linux 支持

- 我们希望的主要改进是 Linux 兼容性。移植到 Linux 需要使用 libusb/hidapi，并用跨平台传感器源替换 WMI/OpenHardwareMonitor 对 CPU 的访问。

## Contributing

- Open issues for bugs or feature requests.
- Pull requests with tests/build instructions are welcome.

