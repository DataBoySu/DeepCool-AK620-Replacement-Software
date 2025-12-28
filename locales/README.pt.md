<!--START_SECTION:navbar-->
[zh]
<!--END_SECTION:navbar-->

# Substituição Digital DeepCool AK620

![Plataforma: Windows](https://img.shields.io/badge/plataforma-Windows-blue?logo=windows)
![Linguagem: Python/C](https://img.shields.io/badge/linguagem-Python%20%7C%20C-ff69b4)

<p align="center">
	<a href="https://openhardwaremonitor.org/downloads/">
		<img src="https://img.shields.io/badge/OpenHardwareMonitor-Instalar-blue?style=for-the-badge" alt="OpenHardwareMonitor - Instalar" />
	</a>
</p>

**Importante:** Instale e execute o OpenHardwareMonitor antes de utilizar essas ferramentas; elas leem as temperaturas da CPU do namespace WMI `root\OpenHardwareMonitor`.

Um substituto de código aberto para o software do controlador DeepCool AK620. Este repositório contém uma utilidade de bandeja Python e uma implementação C nativa para Windows que se comunicam com o AK620 através de HID, leem a temperatura do CPU e atualizam a exibição do dispositivo.

**[LICENSE](../LICENSE)**

## Why this exists

- The vendor EXE for the AK620 can fail to detect the device or crash.
- Many times it stops responding, and OS needs to be restarted.
- No backend information to how values are retrieved then pushed by the software.
- This project documents the HID protocol and provides reliable, transparent tools you can run or rebuild yourself.

## Recursos

- **Comunicação com AK620:** Utiliza HID (correspondência de VendorID/ProductID).
- **Leitura de Temperatura:** Lê periodicamente a temperatura da CPU (via OpenHardwareMonitor/WMI) e atualiza a exibição do dispositivo.
- **Ícone na Barra de Tarefas:** Ícone na barra de tarefas com dica de ferramenta exibindo as temperaturas em °C, °F e K.
- **Implementações:** Disponíveis em Python e C nativo.

## Início Rápido

Para a maioria dos usuários, a opção mais simples é baixar a última versão compilada para Windows na página de Lançamentos do projeto e executar o arquivo EXE incluído.

1. Baixe a última versão para Windows na seção de Lançamentos.
2. Dê um duplo clique no arquivo EXE para executá-lo (aplicativo em bandeja).

## Construindo a partir da fonte

<details>
<summary><strong>Python (construção a partir da fonte)</strong></summary>

1. Crie e ative um ambiente virtual:

```powershell
python -m venv .venv
. \.venv\Scripts\Activate.ps1
```

2. Instale as dependências:

```powershell
pip install -r requirements.txt
```

3. Execute o aplicativo da bandeja:

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

## Solução de Problemas

- Se o Python relatar um `ImportError` para `hid` ou a falta de DLLs do hidapi, execute:

```powershell
pip install -r requirements.txt
```

- OpenHardwareMonitor: Ambas as implementações esperam temperaturas da CPU publicadas no namespace WMI `root\OpenHardwareMonitor`. Instale e execute o OpenHardwareMonitor ou adapte o código para outra fonte.

## VID/PID

- A ferramenta seleciona o dispositivo pelo *USB Vendor ID* (ID do Fabricante USB) e *Product ID* (ID do Produto). Confirme o *VID/PID* do seu dispositivo no Gerenciador de Dispositivos → Detalhes → Identificadores de Hardware, ou enumere dispositivos HID com o auxiliar fornecido.

## Segurança e conformidade

- As aplicações interagem com componentes externos (PowerShell) e acessam o hardware. Execute apenas cópias confiáveis e revise o código caso tenha preocupações com a segurança.

## Expandindo este projeto

- Adicionar suporte para fontes alternativas de temperatura (ACPI, API REST OpenHardwareMonitor, sensores via outros namespaces WMI).
- Melhorar a engenharia reversa de relatórios HID (mais configurações, brilho, perfis).
- Integrar notificações Windows Toast (requer AppUserModelID e atalho no Menu Iniciar para aplicativos não empacotados).

## Comandos Úteis

- Listar dispositivos HID (Python + hidapi):

```python
import hid
for d in hid.enumerate():
    print(d)
```

## Detalhes de Implementação

Consulte o [IMPLEMENTATION.md](../IMPLEMENTATION.md) para o design técnico, formato do relatório HID e orientações de construção.

## Futuro: Suporte a Linux

- A principal melhoria que desejamos é a compatibilidade com Linux. A portabilidade para Linux exige o uso de libusb/hidapi e a substituição do acesso ao WMI/OpenHardwareMonitor CPU por uma fonte de sensores multiplataforma.

## Contribuição

- Abra problemas para relatar bugs ou solicitar novas funcionalidades.
- Envie pull requests com instruções de teste e compilação.

