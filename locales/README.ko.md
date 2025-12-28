<!--START_SECTION:navbar-->
<div align="center">
  <a href="../README.md">🇺🇸 English</a> | <a href="README.de.md">🇩🇪 Deutsch</a> | <a href="README.es.md">🇪🇸 Español</a> | <a href="README.fr.md">🇫🇷 Français</a> | <a href="README.hi.md">🇮🇳 हिंदी</a> | <a href="README.ja.md">🇯🇵 日本語</a> | <a href="README.ko.md">🇰🇷 한국어</a> | <a href="README.pt.md">🇵🇹 Português</a> | <a href="README.ru.md">🇷🇺 Русский</a> | <a href="README.zh.md">🇨🇳 中文</a>
</div>
<!--END_SECTION:navbar-->

# DeepCool AK620 디지털 교체

![플랫폼: Windows](https://img.shields.io/badge/플랫폼-Windows-blue?logo=windows)
![언어: Python/C](https://img.shields.io/badge/언어-Python%7C%20C-ff69b4)


<p align="center">
	<a href="https://openhardwaremonitor.org/downloads/">
		<img src="https://img.shields.io/badge/OpenHardwareMonitor-설치-blue?style=for-the-badge" alt="OpenHardwareMonitor - 설치" />
	</a>
</p>

> **중요:** 이러한 도구 사용 전에 OpenHardwareMonitor를 설치하고 실행하십시오;
> 이들은 `root\OpenHardwareMonitor` WMI 네임스페이스에서 CPU 온도를 읽습니다.

DeepCool AK620 컨트롤러 소프트웨어의 오픈소스 대체 소프트웨어입니다. 이 저장소에는 CPU 온도를 읽고 장치 디스플레이를 업데이트하는 Python 트레이 유틸리티와 Windows C 네이티브 구현이 포함되어 있습니다. 이 소프트웨어는 AK620과 HID를 통해 통신합니다.

[라이선스](../LICENSE)

## 이 프로젝트가 존재하는 이유

- AK620 장치를 감지하지 못하거나 크래시하는 경우가 있는 공급업체 EXE.
- 많은 경우 소프트웨어가 응답하지 않고 운영 체제를 재시작해야 한다.
- 값을 가져와 푸는 소프트웨어의 백엔드 정보가 없다.
- 이 프로젝트는 HID 프로토콜을 문서화하고 재구축하거나 실행할 수 있는 투명한 도구를 제공한다.

## 기능

- AK620과 HID를 통해 통신 (VendorID/ProductID 매칭).
- OpenHardwareMonitor/WMI를 통해 주기적으로 CPU 온도를 읽고 장치 표시를 업데이트.
- 온도 (°C, °F, K)를 표시하는 윈도우 트레이 아이콘과 툴팁 제공.
- 파이썬 및 네이티브 C 구현.

## 빠른 시작

대부분의 사용자에게는 프로젝트 릴리스 페이지에서 최신 컴파일된 Windows 릴리스를 다운로드하고 포함된 EXE를 실행하는 것이 가장 쉬운 옵션입니다.

1. Releases에서 최신 Windows 릴리스 다운로드.
2. EXE를 두 번 클릭하여 실행 (트레이 앱).

## 소스 코드로부터 빌드하기

<details>
<summary><strong>파이썬 (소스 코드로부터 빌드)</strong></summary>

1. 가상 환경 생성 및 활성화:

```powershell
python -m venv .venv
\.venv\Scripts\Activate.ps1
```

2. 의존성 설치:

```powershell
pip install -r requirements.txt
```

3. 트레이 앱 실행:

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

## 문제 해결

- 파이썬이 `hid`에 대한 `ImportError`를 보고하거나 `hidapi` DLL이 누락된 경우:

```powershell
pip install -r requirements.txt
```

- OpenHardwareMonitor: WMI 네임스페이스 `root\OpenHardwareMonitor` 아래에서 CPU 온도를 게시하는 것을 기대하는 두 가지 구현을 설치하고 실행하거나 다른 소스로 코드를 적응시킵니다.

## VID/PID

- 도구는 USB 벤더 ID와 제품 ID를 통해 장치를 선택합니다. 장치 관리자 → 세부 정보 → 하드웨어 ID를 통해 장치의 VID/PID를 확인하거나 제공된 헬퍼를 사용하여 HID 장치를 열거합니다.

## 보안 및 안전

- 애플리케이션은 외부 구성 요소(PowerShell)를 호출하고 하드웨어에 기록합니다. 보안 우려가 있는 경우 신뢰할 수 있는 복사본만 실행하고 코드를 검토하십시오.

## 이 프로젝트 확장

- 대안 온도 소스 지원 추가 (ACPI, OpenHardwareMonitor REST API, 다른 WMI 네임스페이스를 통한 센서).
- HID 보고 역공학 개선 (더 많은 설정, 밝기, 프로필).
- Windows 토스트 알림 추가 (AppUserModelID 및 언패킹 앱의 시작 메뉴 단축키가 필요함).

## 유용한 명령어

- HID 장치 목록 (Python + hidapi)

```python
import hid
for d in hid.enumerate():
    print(d)
```

## 구현 세부 사항

[IMPLEMENTATION.md](../IMPLEMENTATION.md) 파일을 참조하여 기술 설계, HID 보고서 형식 및 빌드 가이드를 확인하십시오.

## 미래: 리눅스 지원

- 우리가 원하는 주요 개선 사항은 리눅스 호환성입니다. 리눅스로 포팅하려면 libusb/hidapi를 사용해야 하고 WMI/OpenHardwareMonitor 대신 크로스 플랫폼 센서 소스를 CPU 접근에 대체해야 합니다.

## 기여 방법

- 버그나 기능 요청은 이슈를 열어주세요.
- 테스트/빌드 지침이 포함된 풀 리퀘스트은 환영합니다.

