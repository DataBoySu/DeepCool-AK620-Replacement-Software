# DeepCool AK620 Replacement

![Platform: Windows](https://img.shields.io/badge/platform-Windows-blue?logo=windows)
![Language: Python/C](https://img.shields.io/badge/language-Python%20%7C%20C-ff69b4)


<p align="center">
	<a href="https://openhardwaremonitor.org/downloads/">
		<img src="https://img.shields.io/badge/OpenHardwareMonitor-Install-blue?style=for-the-badge" alt="OpenHardwareMonitor - Install" />
	</a>
</p>

> **Important:** Install and run OpenHardwareMonitor before using these tools;
> they read CPU temperatures from the WMI namespace `root\OpenHardwareMonitor`.

Open-source replacement for the DeepCool AK620 controller software. This repo contains a Python tray utility and a native Windows C implementation that communicate with the AK620 over HID, read CPU temperature and update the device display.

[IMPLMENTATION](IMPLEMENTATION.md)

- See `IMPLEMENTATION.md` for a deeper technical description of the HID protocol, runtime flow, and build guidance.

[LICENSE](LICENSE)

## Features

- Communicates with the AK620 using HID (VendorID/ProductID matching).
- Periodically reads CPU temperature (via OpenHardwareMonitor/WMI) and updates the device display.
- Windows tray icon with tooltip showing temperatures in °C, °F and K.
- Python and native C implementations.

## Quick start

For most users the easiest option is to download the latest compiled Windows release from the project's Releases page and run the included EXE.

1. Download the latest release for Windows from Releases.
2. Double-click the EXE to run (tray app).

## Building from source

<details>
<summary><strong>Python (build from source)</strong></summary>

1. Create and activate a virtual environment:

```powershell
python -m venv .venv
. \.venv\Scripts\Activate.ps1
```

2. Install dependencies:

```powershell
pip install -r requirements.txt
```

3. Run the tray app:

```powershell
python deepcool_tray.py
```

</details>

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

- OpenHardwareMonitor: both implementations expect CPU temperatures published under WMI namespace `root\OpenHardwareMonitor`. Install and run OpenHardwareMonitor or adapt the code to another source.

## VID / PID

- The tool selects the device by USB Vendor ID and Product ID. Confirm your device's VID/PID via Device Manager → Details → Hardware Ids, or enumerate HID devices with the provided helper.

## Security and safety

- The apps call external components (PowerShell) and write to hardware. Run only trusted copies and review the code if you have security concerns.

## Extending this project

- Add support for alternative temperature sources (ACPI, OpenHardwareMonitor REST API, sensors via other WMI namespaces).
- Improve HID report reverse engineering (more settings, brightness, profiles).
- Add Windows Toast notifications (requires AppUserModelID & Start Menu shortcut for unpackaged apps).

## Useful commands

- List HID devices (Python + hidapi):

```python
import hid
for d in hid.enumerate():
    print(d)
```

## Implementation details

See [IMPLEMENTATION.md](IMPLEMENTATION.md) for technical design, HID report format, and build guidance.

## Future: Linux support

- The major enhancement we want is Linux compatibility. Porting to Linux requires using libusb/hidapi and replacing WMI/OpenHardwareMonitor CPU access with a cross-platform sensor source.

## License

- Suggested: MIT. Replace with your preferred license by adding a `LICENSE` file.

## Contributing

- Open issues for bugs or feature requests.
- Pull requests with tests/build instructions are welcome.
# DeepCool AK620 Digital's Open-source Replacement

This repository provides an open-source replacement for the official DeepCool AK620 controller software. It includes:

- A Python tray utility (`deepcool_tray.py`) that polls CPU temperature and sends HID reports to the AK620.
- A native C implementation (`v2/deep_replace.c`) that does the same with a Windows tray icon.

Why this exists

- The vendor EXE for the AK620 can fail to detect the device or crash.
- Many times it stops responding, and OS needs to be restarted.
- No backend information to how values are retrieved then pushed by the software.
- This project documents the HID protocol and provides reliable, transparent tools you can run or rebuild yourself.

Features

- Communicates with the AK620 over HID (VendorID/ProductID match).
- Periodically reads CPU temperature (via OpenHardwareMonitor/WMI) and updates the device display.
- Shows a tray icon and tooltip with temperature (Celsius, Fahrenheit, Kelvin).
- Windows-native C implementation for a lightweight binary.

Latest release

- For most users the easiest option is to download the latest compiled Windows release from the project's Releases page and run the included EXE. This provides a ready-to-run binary without building from source.

Build Python (build from source)

<details>

<summary>Building and running the Python tray utility
</summary>

1. Create and activate a virtual environment:

```powershell
python -m venv .venv
. \.venv\Scripts\Activate.ps1
```

2. Install dependencies:

```powershell
pip install hidapi wmi pystray pillow pywin32
```

3. Run the tray app:

```powershell
python deepcool_tray.py
```

</details>

Build C (MSVC) (build from source)

<details>
<summary>Building the native Windows C binary</summary>

1. Open the "MS C++ (Professional) - PowerShell" terminal profile (see workspace settings).
2. Build with cl (example):

```powershell
cl /EHsc v2\deep_replace.c user32.lib shell32.lib setupapi.lib hid.lib comctl32.lib /link /OUT:deep_replace.exe
```

3. Run the produced `deep_replace.exe` (GUI tray app).

</details>

## Extra Information

<details><summary> Troubleshooting</summary>

- hid ImportError (Python): If you see errors about missing hidapi DLLs, install `hidapi` in the same virtualenv or ensure `hidapi.dll` / `libhidapi-0.dll` is on PATH. On Windows use:

```powershell
pip install hidapi
```

- OpenHardwareMonitor requirement: Both implementations query the WMI namespace created by OpenHardwareMonitor. Install and run OpenHardwareMonitor (or ensure something publishes CPU temps to `root\OpenHardwareMonitor`) or the app will show an error in the tray.
- Degree symbol / tooltip encoding: The C app uses Unicode tooltips; if you see encoding artifacts rebuild the binary after ensuring the source is saved in UTF-8.

VID / PID

- The device is selected using its USB Vendor ID and Product ID (VID/PID). Confirm on your machine via Device Manager → Details → Hardware Ids or by enumerating HID devices.

Security and safety

- The apps call external components (PowerShell) and write to hardware. Run the tools only from trusted copies and inspect the code if you have security concerns.

Extending this project

- Porting the Python tool to use cross-platform temperature sources and libusb/hidapi on Linux will enable broader use.
- Add support for alternative temperature sources (ACPI, OpenHardwareMonitor REST API, sensors via WMI other namespaces).
- Improve HID report reverse engineering (support more settings or brightness/profiles).
- Add Windows Toast notifications (requires AppUserModelID and Start Menu shortcut registration for unpackaged apps).

Useful commands

- List HID devices (Python script using hidapi is recommended). Example (Python):

```python
import hid
for d in hid.enumerate():
	print(d)
```

</details>

## Questions or PRs

- Open an issue if you want help building, adding features, or clarifying the HID report format.
