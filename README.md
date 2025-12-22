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

Troubleshooting

- hid ImportError (Python): If you see errors about missing hidapi DLLs, install `hidapi` in the same virtualenv or ensure `hidapi.dll` / `libhidapi-0.dll` is on PATH. On Windows use:

```powershell
pip install hidapi
```

- OpenHardwareMonitor requirement: Both implementations query the WMI namespace created by OpenHardwareMonitor. Install and run OpenHardwareMonitor (or ensure something publishes CPU temps to `root\OpenHardwareMonitor`) or the app will show an error in the tray.
- Degree symbol / tooltip encoding: The C app uses Unicode tooltips; if you see encoding artifacts rebuild the binary after ensuring the source is saved in UTF-8.

VID / PID

- The device is selected using its USB Vendor ID and Product ID (VID/PID). Confirm on your machine via Device Manager → Details → Hardware Ids or by enumerating HID devices.

9) Security and safety
- The apps call external components (PowerShell) and write to hardware. Run the tools only from trusted copies and inspect the code if you have security concerns.

10) Extending this project
- Add support for alternative temperature sources (ACPI, OpenHardwareMonitor REST API, sensors via WMI other namespaces).
- Improve HID report reverse engineering (support more settings or brightness/profiles).
- Add Windows Toast notifications (requires AppUserModelID and Start Menu shortcut registration for unpackaged apps).

11) Useful commands
- List HID devices (Python script using hidapi is recommended). Example (Python):
```python
import hid
for d in hid.enumerate():
	print(d)
```

Questions or PRs
- Open an issue if you want help building, adding features, or clarifying the HID report format.

Implementation details
- See `IMPLEMENTATION.md` for a deeper technical description of the HID protocol, runtime flow, and build guidance.

Future: Linux support
- The biggest improvement and contribution we welcome is adding Linux compatibility. The project is currently Windows-centric (WMI / SetupAPI / HidD). Porting the Python tool to use cross-platform temperature sources and libusb/hidapi on Linux will enable broader use.

License
- Suggested: MIT. If you want a different license, replace the `LICENSE` file accordingly.

Contributing

- Open issues describing bugs or desired features.
- Pull requests with tests or build instructions are welcome.
