<!--START_SECTION:navbar-->
<div align="center">
  <a href="../README.md">🇺🇸 English</a> | <a href="README.de.md">🇩🇪 Deutsch</a> | <a href="README.es.md">🇪🇸 Español</a> | <a href="README.fr.md">🇫🇷 Français</a> | <a href="README.hi.md">🇮🇳 हिंदी</a> | <a href="README.ja.md">🇯🇵 日本語</a> | <a href="README.ko.md">🇰🇷 한국어</a> | <a href="README.pt.md">🇵🇹 Português</a> | <a href="README.ru.md">🇷🇺 Русский</a> | <a href="README.zh.md">🇨🇳 中文</a>
</div>
<!--END_SECTION:navbar-->

# DeepCool AK620 DIGITAL Ersatz

![Plattform: Windows](https://img.shields.io/badge/plattform-Windows-blue?logo=windows)
![Sprache: Python/C](https://img.shields.io/badge/sprache-Python%20%7C%20C-ff69b4)

<p align="center">
	<a href="https://openhardwaremonitor.org/downloads/">
		<img src="https://img.shields.io/badge/OpenHardwareMonitor-Installieren-blue?style=for-the-badge" alt="OpenHardwareMonitor - Installieren" />
	</a>
</p>

**Wichtig:** Installiere und führe OpenHardwareMonitor aus, bevor du diese Tools verwendest;
sie lesen CPU-Temperaturen aus dem WMI-Namespace `root\OpenHardwareMonitor`.

Ein Open-Source-Ersatz für die DeepCool AK620-Steuerungssoftware. Dieses Repository enthält eine Python-Tray-Anwendung und eine native Windows-C-Implementierung, die über HID mit dem AK620 kommunizieren, die CPU-Temperatur abrufen und die Geräteanzeige aktualisieren.

**[LIZENZ](../LICENSE)**

## Warum dies existiert

- Die Vendor-EXE für das AK620 kann fehlschlagen, wenn es um die Erkennung des Geräts oder das Abstürzen geht.
- Häufig hört es auf zu reagieren, sodass ein Neustart des Betriebssystems erforderlich ist.
- Es gibt keine Hintergrundinformationen darüber, wie Werte abgerufen und dann von der Software gesendet werden.
- Dieses Projekt dokumentiert das HID-Protokoll und stellt zuverlässige, transparente Tools bereit, die du entweder ausführen oder selbst neu erstellen kannst.

## Funktionen

- **Kommunikation mit AK620:** Über HID (Übereinstimmung von VendorID/ProductID).
- **Periodische Lesevorgänge der CPU-Temperatur:** Via OpenHardwareMonitor/WMI und Aktualisierung der Gerätdarstellung.
- **Windows-Tray-Icon:** Mit Tooltip, der die Temperaturen in °C, °F und K anzeigt.
- **Implementierungen:** In Python und nativem C.

## Schnelle Einführung

Für die meisten Benutzer ist die einfachste Option, die neueste kompilierte Windows-Version aus der Projekt-Releases-Seite herunterzuladen und die enthaltene EXE-Datei auszuführen.

1. Lade die neueste Windows-Version aus den Releases herunter.
2. Doppelklicke die EXE, um das Programm zu starten (Systemtray-App).

## Aufbau aus Quellcode

<details>
<summary><strong>Python (Aufbau aus Quellcode)</strong></summary>

1. Erstelle und aktiviere eine virtuelle Umgebung:

```bash
python -m venv .venv
source .venv/bin/activate  # Auf Windows: .\venv\Scripts\activate
```

2. Installiere Abhängigkeiten:

```bash
pip install -r requirements.txt
```

3. Starte die Tray-App:

```bash
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

## Fehlerbehebung

- Wenn Python bei `hid` einen `ImportError` ausgibt oder fehlende `hidapi`-DLLs meldet, führe Folgendes aus:

```powershell
pip install -r requirements.txt
```

- **OpenHardwareMonitor**: Sowohl die Implementierungen erwarten CPU-Temperaturen, die unter dem WMI-Namespace `root\OpenHardwareMonitor` veröffentlicht werden. Installiere und führe OpenHardwareMonitor aus oder passe den Code an eine andere Quelle an.

## VID/PID

- Das Tool wählt das Gerät anhand der USB-Hersteller-ID (Vendor ID) und der Produkt-ID (Product ID) aus. Bestätige die VID/PID deines Geräts über Geräte-Manager → Details → Hardware-IDs oder enumeriere HID-Geräte mit dem bereitgestellten Assistenten.

## Sicherheit und Schutz

- Die Anwendungen rufen externe Komponenten (PowerShell) auf und schreiben auf Hardware. Führe nur vertrauenswürdige Kopien aus und überprüfe den Code, falls du Sicherheitsbedenken hast.

## Die Erweiterung dieses Projekts

- **Hinzufügen der Unterstützung alternativer Temperaturquellen:** (ACPI, OpenHardwareMonitor REST API, Sensoren über andere WMI-Namespaces).
- **Verbesserung der HID-Bericht-Reverse-Engineering** (weitere Einstellungen, Helligkeit, Profile).
- **Hinzufügen von Windows Toast-Benachrichtigungen** (erfordert AppUserModelID und Startmenü-Shortcut für unpaketierte Anwendungen).

## Nützliche Befehle

- Liste der HID-Geräte auflisten (Python + hidapi):

```python
import hid
for d in hid.enumerate():
    print(d)
```

## Implementierungsdetails

Siehe [IMPLEMENTATION.md](../IMPLEMENTATION.md) für technische Design-Konzepte, HID-Berichtsformat und Build-Anweisungen.

## Zukunft: Linux-Unterstützung

- Die wichtigste Verbesserung, die wir anstreben, ist die Linux-Kompatibilität. Um auf Linux zu portieren, müssen wir libusb/hidapi verwenden und den Zugriff auf die CPU über WMI/OpenHardwareMonitor durch eine plattformübergreifende Sensordatenquelle ersetzen.

## Beiträge

- Öffne Issues für Fehler oder Feature-Anfragen.
- Pull Requests mit Test- und Build-Anweisungen sind willkommen.

