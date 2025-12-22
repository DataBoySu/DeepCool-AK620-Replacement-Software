# Implementation Details

## DeepCool AK620 Digital Software Replacement

This document explains the internals of the two implementations in this repo (Python and C), the runtime flow, HID report details, and build notes.

1) High-level architecture
- Device selection: the apps enumerate HID devices and match Vendor ID (`VENDOR_ID`) and Product ID (`PRODUCT_ID`). The C code uses `SetupDiGetClassDevs` + `HidD_GetAttributes`; the Python version uses `hid` (hidapi).
- Temperature source: the apps read CPU temperature from the OpenHardwareMonitor WMI namespace (`root\OpenHardwareMonitor`). The Python app calls into WMI directly; the C app spawns a hidden PowerShell command that queries WMI.
- Output: periodic HID reports are written to the device to update its built-in display. A tray icon shows the current temperature.

2) Runtime flow (C implementation)
- `WinMain`:
  - Calls `open_hid_device()` to locate and open the AK620 HID handle.
  - Registers a message-only window and adds a tray icon (`Shell_NotifyIconW`).
  - Starts two background threads: `hid_loop` and `tray_update_loop`.
  - Runs the Windows message loop to handle tray interactions.
- `hid_loop`:
  - Every `TEMP_CHECK_INTERVAL_MS` it calls `get_cpu_temp()`.
  - If temperature retrieval fails (OpenHardwareMonitor not available), it sets an error state and updates the tray tooltip with an error message (no longer fakes temperatures).
  - If temperature is valid, it computes `tens` and `ones` digits and writes the HID report via `WriteFile` on the open device handle.
- `tray_update_loop`:
  - When `temp_c` changes, it updates the tray tooltip using `update_tray()` which now formats Celsius, Fahrenheit and Kelvin in a single column.

3) HID report format
- The implementation uses a 64-byte report buffer pre-filled like: `[16, 19, 1, 0, tens, ones, 0, 0, ...]`.
- Indices 4 and 5 are written with the tens and ones decimal digits of the temperature (0–99). The rest of the report is left as fixed/zero values to match the device's expected report length.
- This was derived by reverse-engineering the official controller behavior and validating against the device.

4) Device enumeration and identification
- The C app enumerates interfaces with `SetupDiEnumDeviceInterfaces`, retrieves the device path, opens the device with `CreateFile`, then calls `HidD_GetAttributes` to read VendorID/ProductID.
- If you want to uniquely identify a single unit across systems or across USB ports, prefer VID+PID+SerialNumber (obtained via `HidD_GetSerialNumberString`) rather than the OS device path (which can change per port).

5) Temperature source and failure modes
- The code expects OpenHardwareMonitor (or another provider) to publish sensors to `root\OpenHardwareMonitor` WMI namespace. If that namespace is missing or returns no temperature, the app now sets an error state and shows an explicit tray tooltip error.
- You can run OpenHardwareMonitor or another WMI publisher (or adapt the code to use other sources such as `OpenHardwareMonitorReader`, `psutil` or vendor SDKs).

6) Tray tooltip and internationalization
- The C implementation uses wide-character APIs (`NOTIFYICONDATAW`, `Shell_NotifyIconW`) and formats text with Unicode degree signs (`\u00B0`) so that the tooltip shows `°C` and `°F` correctly. The tooltip displays three lines: C, F, K.

7) Building
- MSVC (recommended on Windows): open a Developer PowerShell and run:
```powershell
cl /EHsc v2\deep_replace.c user32.lib shell32.lib setupapi.lib hid.lib comctl32.lib /link /OUT:deep_replace.exe
```
- If link errors appear about missing `.lib` files (for `hid.lib`, `setupapi.lib`, etc.), locate the Windows SDK libraries (e.g. under `C:\Program Files (x86)\Windows Kits\10\Lib\<version>\um\x64`) and pass `/LIBPATH:...` to `cl`.

8) Python notes
- The Python app uses `hidapi` (module `hid`), `wmi` for reading sensors, `pystray` and `Pillow` for the tray icon, and `pywin32` (for COM initialization / pythoncom). If `import hid` fails with an ImportError about missing native libraries, install `hidapi` in the same virtualenv or ensure the hidapi DLL is available.