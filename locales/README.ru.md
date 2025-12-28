<!--START_SECTION:navbar-->
[zh]
<!--END_SECTION:navbar-->

# Замена DeepCool AK620 DIGITAL

![Платформа: Windows](https://img.shields.io/badge/platform-Windows-blue?logo=windows)
![Язык: Python/C](https://img.shields.io/badge/language-Python%20%7C%20C-ff69b4)

<p align="center">
	<a href="https://openhardwaremonitor.org/downloads/">
		<img src="https://img.shields.io/badge/OpenHardwareMonitor-Установить-blue?style=for-the-badge" alt="OpenHardwareMonitor - Установить" />
	</a>
</p>

> **Важно:** Установите и запустите OpenHardwareMonitor перед использованием этих инструментов;
> они читают температуры процессора из пространства WMI `root\OpenHardwareMonitor`.

Открытый исходный код, заменяющий программное обеспечение контроллера DeepCool AK620. Этот репозиторий содержит утилиту системы трая на Python и имплементацию на C для Windows, которые общаются с AK620 через HID, читают температуру процессора и обновляют отображение устройства.

[Лицензия](../LICENSE)

## Why this exists

- The vendor EXE for the AK620 can fail to detect the device or crash.
- Many times it stops responding, and OS needs to be restarted.
- No backend information to how values are retrieved then pushed by the software.
- This project documents the HID protocol and provides reliable, transparent tools you can run or rebuild yourself.

## Функции

- Обеспечивает связь с AK620 через HID (совпадение VendorID/ProductID).
- Периодически считывает температуру процессора (с помощью OpenHardwareMonitor/WMI) и обновляет отображение на устройстве.
- Плавка в системе Windows с подсказкой, показывающей температуры в °C, °F и K.
- Реализация на Python и C (создание нативных бинарных файлов).

## Быстрый старт

Для большинства пользователей самым простым вариантом является скачивание последней скомпилированной версии для Windows с страницы релизов проекта и запуск включённого в неё EXE.

1. Скачайте последнюю версию для Windows с страницы релизов.
2. Двойным щелчком запустите EXE (приложением в панели задач).

## Сборка из исходного кода

<details>
<summary><strong>Python (сборка из исходного кода)</strong></summary>

1. Создание и активация виртуального окружения:

```powershell
python -m venv .venv
\.venv\Scripts\Activate.ps1
```

2. Установка зависимостей:

```powershell
pip install -r requirements.txt
```

3. Запуск приложения в трее:

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

## Решение проблем

- Если Python сообщает `ImportError` для `hid` или отсутствующих библиотек hidapi DLL, запустите:

```powershell
pip install -r requirements.txt
```

- OpenHardwareMonitor: обе реализации ожидают, что температуры процессора будут опубликованы в пространстве WMI `root\OpenHardwareMonitor`. Установите и запустите OpenHardwareMonitor или адаптируйте код под другой источник.

## VID/PID

- Инструмент выбирает устройство по идентификаторам USB-поставщика (VID) и продукта (PID). Подтвердите VID/PID вашего устройства через "Управитель устройств" → "Детали" → "Идентификаторы аппаратного обеспечения" или перечислите HID-устройства с помощью предоставленного помощника.

## Безопасность и безопасность

- Приложения вызывают внешние компоненты (PowerShell) и пишут в аппаратное обеспечение. Запускайте только проверенные копии и проверяйте код, если у вас есть опасения по поводу безопасности.

## Расширение проекта

- Добавление поддержки альтернативных источников температуры (ACPI, REST API OpenHardwareMonitor, датчики через другие пространства WMI).
- Улучшение обратной инженерии отчетов HID (более настройки, яркость, профили).
- Добавление уведомлений Windows Toast (требует AppUserModelID и ярлыка в Старте для непакованных приложений).

## Полезные команды

- Список устройств HID (Python + hidapi):

```python
import hid
for d in hid.enumerate():
    print(d)
```

## Детали реализации

См. [IMPLEMENTATION.md](../IMPLEMENTATION.md) для технической архитектуры, формата отчета HID и инструкций по сборке.

## Будущее: Поддержка Linux

- Главное улучшение, которое мы хотим реализовать, — это совместимость с Linux. Для перехода на Linux необходимо использовать libusb/hidapi и заменить доступ к CPU через WMI/OpenHardwareMonitor на кросс-платформенный источник данных сенсоров.

## Участие

- Откройте задачи для отчетов о багах или запросов на новые функции.
- Принимаются запросы на слияние с включёнными тестами и инструкциями по сборке.

