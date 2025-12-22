try:
    import hid
    hid_import_error = None
except Exception as e:
    hid = None
    hid_import_error = e

import time
import threading
import wmi
import pystray
from PIL import Image, ImageDraw
import math
import pythoncom

VENDOR_ID = 0x3633
PRODUCT_ID = 0x0002

def get_cpu_temp():
    try:
        w = wmi.WMI(namespace="root\\OpenHardwareMonitor")
        sensors = w.Sensor()
        temps = []
        for sensor in sensors:
            if sensor.SensorType == u'Temperature' and sensor.Value is not None:
                if "CPU Package" in sensor.Name or "CPU Core" in sensor.Name or "CPU" in sensor.Name:
                    temps.append(float(sensor.Value))
        if temps:
            return math.ceil(max(temps))
    except Exception:
        pass
    return 0

def create_image():
    image = Image.new('RGB', (64, 64), color=(0, 0, 0))
    d = ImageDraw.Draw(image)
    d.ellipse((16, 16, 48, 48), fill=(0, 255, 0))
    return image

def run_tray():
    if hid is None:
        print("ERROR: the 'hid' native library could not be loaded.")
        print(repr(hid_import_error))
        print("")
        print("On Windows, install a Python package that bundles hidapi (for example: `pip install hidapi`).")
        print("Alternatively, install the hidapi DLL (hidapi.dll / libhidapi-0.dll) so the `hid` package can load it.")
        print("If you use a virtualenv, run the pip command inside that environment.")
        return

    dev = hid.device()
    try:
        dev.open(VENDOR_ID, PRODUCT_ID)
    except Exception:
        return

    temp_c = 0
    stop_event = threading.Event()

    def hid_loop():
        nonlocal temp_c
        pythoncom.CoInitialize()
        while not stop_event.is_set():
            temp_c = get_cpu_temp()
            temp_c = min(max(temp_c, 0), 99)
            tens = temp_c // 10
            ones = temp_c % 10
            out_report = [16, 19, 1, 0, tens, ones] + [0] * 58
            try:
                dev.write(out_report)
            except Exception:
                pass
            time.sleep(2)

    def on_exit(icon, item):
        stop_event.set()
        icon.stop()

    menu = pystray.Menu(pystray.MenuItem('Exit', on_exit))
    icon = pystray.Icon("DeepCool", create_image(), "CPU Temp: 0°C", menu=menu)

    hid_thread = threading.Thread(target=hid_loop, daemon=True)
    hid_thread.start()

    def tray_update():
        while not stop_event.is_set():
            icon.title = f"CPU Temp: {temp_c}°C"
            time.sleep(2)

    tray_thread = threading.Thread(target=tray_update, daemon=True)
    tray_thread.start()

    icon.run()
    dev.close()

if __name__ == "__main__":
    run_tray()