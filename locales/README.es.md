<!--START_SECTION:navbar-->
[zh]
<!--END_SECTION:navbar-->

# Reemplazo DeepCool AK620 DIGITAL

![Plataforma: Windows](https://img.shields.io/badge/plataforma-Windows-blue?logo=windows)
![Lenguaje: Python/C](https://img.shields.io/badge/lenguaje-Python%20%7C%20C-ff69b4)

<p align="center">
	<a href="https://openhardwaremonitor.org/downloads/">
		<img src="https://img.shields.io/badge/OpenHardwareMonitor-Instalar-blue?style=for-the-badge" alt="OpenHardwareMonitor - Instalar" />
	</a>
</p>

**Importante:** Instala y ejecuta OpenHardwareMonitor antes de utilizar estas herramientas; leen las temperaturas del CPU desde el espacio de nombres WMI `root\OpenHardwareMonitor`.

Reemplazo de código abierto para el software DeepCool AK620. Este repositorio contiene una utilidad de bandeja de Python y una implementación nativa de Windows en C que se comunican con el AK620 a través de HID, leen la temperatura del CPU y actualizan la pantalla del dispositivo.

[LICENCIA](../LICENSE)

## ¿Por qué existe esto?

- El EXE del proveedor para AK620 puede fallar en detectar el dispositivo o colapsar.
- Con frecuencia deja de responder y el sistema operativo debe reiniciarse.
- No hay información de la parte posterior sobre cómo se obtienen y empujan los valores por el software.
- Este proyecto documenta el protocolo HID y proporciona herramientas fiables y transparentes que puedes ejecutar o reconstruir por tu cuenta.

## Características

- Comunica con el AK620 utilizando HID (coincidencia de VendorID/ProductID).
- Lee periódicamente la temperatura del CPU (a través de OpenHardwareMonitor/WMI) y actualiza la visualización del dispositivo.
- Ícono en la bandeja de Windows con una herramienta de información que muestra las temperaturas en °C, °F y K.
- Implementaciones en Python y C nativo.

## Inicio rápido

Para la mayoría de los usuarios, la opción más sencilla es descargar la última versión compilada de Windows desde la página de Lanzamientos del proyecto y ejecutar el archivo EXE incluido.

1. Descarga la última versión para Windows desde Lanzamientos.
2. Haz doble clic en el EXE para ejecutarlo (aplicación de área de notificación).

## Compilación desde el código fuente

<details>
<summary><strong>Python (compilación desde el código fuente)</strong></summary>

1. Crear y activar un entorno virtual:

```powershell
python -m venv .venv
\.venv\Scripts\Activate.ps1
```

2. Instalar dependencias:

```powershell
pip install -r requirements.txt
```

3. Ejecutar la aplicación de la bandeja:

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

## Solución de problemas

- Si Python informa un `ImportError` para `hid` o faltan las DLL de hidapi, ejecute:

```powershell
pip install -r requirements.txt
```

- OpenHardwareMonitor: Ambas implementaciones esperan temperaturas de CPU publicadas bajo el espacio de nombres WMI `root\OpenHardwareMonitor`. Instala y ejecuta OpenHardwareMonitor o adapta el código a otra fuente.

## VID / PID

- La herramienta selecciona el dispositivo por el ID de proveedor USB (VID) y el ID de producto (PID). Confirme el VID/PID de su dispositivo a través del Administrador de dispositivos → Detalles → Identificadores de hardware, o enumere dispositivos HID con el asistente proporcionado.

## Seguridad y protección

- Las aplicaciones llaman a componentes externos (PowerShell) y escriben en el hardware. Solo ejecute copias confiables y revise el código si tiene preocupaciones de seguridad.

## Ampliando este proyecto

- Agregar soporte para fuentes de temperatura alternativas (ACPI, API REST de OpenHardwareMonitor, sensores a través de otros espacios de nombre WMI).
- Mejorar la ingeniería inversa de informes HID (más configuraciones, brillo, perfiles).
- Incorporar notificaciones Toast en Windows (requiere AppUserModelID y acceso al menú Inicio para aplicaciones sin empaquetar).

## Comandos útiles

- Listar dispositivos HID (Python + hidapi):

```python
import hid
for d in hid.enumerate():
    print(d)
```

## Detalles de implementación

Consulte [IMPLEMENTACIÓN.md](../IMPLEMENTACIÓN.md) para el diseño técnico, el formato del informe HID y las instrucciones de compilación.

## Futuro: Soporte para Linux

- La principal mejora que deseamos es la compatibilidad con Linux. Portar a Linux implica utilizar libusb/hidapi y reemplazar el acceso a WMI/OpenHardwareMonitor para la CPU con una fuente de sensores multiplataforma.

## Contribución

- Abre problemas para reportar errores o solicitar nuevas funcionalidades.
- Se aceptan solicitudes de extracción con instrucciones de pruebas/compilación.

