<!--START_SECTION:navbar-->
[zh]
<!--END_SECTION:navbar-->

# Remplacement DeepCool AK620 DIGITAL

![Plateforme : Windows](https://img.shields.io/badge/plateforme-Windows-blue?logo=windows)
![Langage : Python/C](https://img.shields.io/badge/langage-Python%20%7C%20C-ff69b4)

<p align="center">
	<a href="https://openhardwaremonitor.org/downloads/">
		<img src="https://img.shields.io/badge/OpenHardwareMonitor-Installer-blue?style=for-the-badge" alt="OpenHardwareMonitor - Installer" />
	</a>
</p>

**Important :** Installez et exécutez OpenHardwareMonitor avant d'utiliser ces outils ; ils lisent les températures CPU à partir de l'espace de noms WMI `root\OpenHardwareMonitor`.

Un remplacement open-source pour le logiciel de contrôle DeepCool AK620. Ce dépôt contient un utilitaire de barre d'état système en Python et une implémentation native Windows en C qui communiquent avec l'AK620 via HID, lisent la température du CPU et mettent à jour l'affichage du dispositif.

**[LICENCE](../LICENSE)**

## Pourquoi cela existe

- L'EXE fournie par le fabricant pour l'AK620 peut échouer dans la détection du périphérique ou planter.
- Souvent, il cesse de répondre, nécessitant un redémarrage du système d'exploitation.
- Il n'y a aucune information sur l'arrière-plan quant à la façon dont les valeurs sont récupérées puis poussées par le logiciel.
- Ce projet documente le protocole HID et fournit des outils fiables et transparents que vous pouvez exécuter ou reconstruire vous-même.

## Fonctionnalités

- Communique avec l'AK620 via HID (correspondance VendorID/ProductID).
- Lit périodiquement la température du CPU (via OpenHardwareMonitor/WMI) et met à jour l'affichage du dispositif.
- Icône de tray Windows avec une infobulle affichant les températures en °C, °F et K.
- Implémentations en Python et C natif.

## Démarrage rapide

Pour la plupart des utilisateurs, l'option la plus simple consiste à télécharger la dernière version compilée pour Windows depuis la page des versions du projet et à exécuter le fichier EXE inclus.

1. Téléchargez la dernière version pour Windows depuis la page des versions.
2. Double-cliquez sur le fichier EXE pour lancer l'application (icône dans la barre des tâches).

## Construction à partir de la source

<details>
<summary><strong>Python (construction à partir de la source)</strong></summary>

1. Créer et activer un environnement virtuel :

```powershell
python -m venv .venv
. \.venv\Scripts\Activate.ps1
```

2. Installer les dépendances :

```powershell
pip install -r requirements.txt
```

3. Exécuter l'application de la barre des tâches :

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

## Dépannage

- Si Python signale une `ImportError` pour `hid` ou manque des DLL hidapi, exécutez :

```powershell
pip install -r requirements.txt
```

- OpenHardwareMonitor : les deux implémentations attendent que les températures du CPU soient publiées sous l'espace de noms WMI `root\OpenHardwareMonitor`. Installez et exécutez OpenHardwareMonitor ou adaptez le code à une autre source.

## VID / PID

- L'outil sélectionne le périphérique par l'identifiant du fabricant USB (VID) et l'identifiant du produit (PID). Vérifiez l'ID VID/PID de votre périphérique via Gestionnaire de périphériques → Détails → Identifiants matériels, ou énumérez les périphériques HID avec l'assistant fourni.

## Sécurité et sécurité

- Les applications appellent des composants externes (PowerShell) et écrivent sur le matériel. N'exécutez que des copies fiables et examinez le code si vous avez des préoccupations en matière de sécurité.

## Étendre ce projet

- Ajouter le support de sources de température alternatives (ACPI, OpenHardwareMonitor REST API, capteurs via d'autres espaces de noms WMI).
- Améliorer l'ingénierie inverse des rapports HID (plus de paramètres, luminosité, profils).
- Intégrer les notifications Toast Windows (nécessite AppUserModelID et un raccourci vers le menu Démarrage pour les applications non empaquetées).

## Commandes utiles

- Lister les dispositifs HID (Python + hidapi) :

```python
import hid
for d in hid.enumerate():
    print(d)
```

## Détails d'implémentation

Consultez [IMPLEMENTATION.md](../IMPLEMENTATION.md) pour la conception technique, le format de rapport HID et les instructions de construction.

## Avenir : Support Linux

- L'amélioration majeure que nous souhaitons apporter est la compatibilité Linux. Le portage vers Linux nécessite l'utilisation de libusb/hidapi et le remplacement de l'accès WMI/OpenHardwareMonitor pour le CPU par une source de capteurs multiplateforme.

## Contribution

- Ouvrez des problèmes pour signaler des bogues ou des demandes de fonctionnalités.
- Les demandes de tirage avec des instructions de test/construction sont les bienvenues.

