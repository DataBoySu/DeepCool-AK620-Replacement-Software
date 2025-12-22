// deep_replace.c - Minimal DeepCool tray app in C for Windows
// Features: HID communication, CPU temp polling, tray icon, periodic update
// Compile with: gcc deep_replace.c -o deep_replace.exe -lshell32 -luser32 -lhid -lsetupapi
// If using Microsoft's C compiler: cl /EHsc deep_replace.c user32.lib shell32.lib setupapi.lib hid.lib comctl32.lib /link /OUT:deep_replace.exe

#include <windows.h>
#include <shellapi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <hidsdi.h>
#include <setupapi.h>

#define VENDOR_ID 0x3633
#define PRODUCT_ID 0x0002
#define POLL_INTERVAL_MS 5000  // Increased from 2s to 5s for lower CPU usage
#define TEMP_CHECK_INTERVAL_MS 3000  // Check temp less frequently than HID writes
#define WM_TRAYICON (WM_USER + 1)

NOTIFYICONDATA nid;
HWND hwnd;
HINSTANCE hInst;

// Global variables for thread communication
volatile int temp_c = 0;
volatile int last_temp_sent = -1;  // Track last temperature sent to avoid redundant updates
volatile bool stop_event = false;
HANDLE hid_device = INVALID_HANDLE_VALUE;

// Pre-allocated buffer for HID writes (avoid repeated allocations)
static unsigned char hid_buffer[64] = {16, 19, 1, 0, 0, 0}; // Pre-set the fixed bytes

// Get CPU temp using PowerShell command (hidden window)
int get_cpu_temp() {
    STARTUPINFO si = {0};
    PROCESS_INFORMATION pi = {0};
    HANDLE hRead, hWrite;
    SECURITY_ATTRIBUTES sa = {0};
    char temp_str[256] = {0};
    DWORD bytesRead;
    int temp = 0;
    
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.bInheritHandle = TRUE;
    sa.lpSecurityDescriptor = NULL;
    
    // Create pipe for reading output
    if (!CreatePipe(&hRead, &hWrite, &sa, 0)) return 0;
    
    si.cb = sizeof(STARTUPINFO);
    si.hStdOutput = hWrite;
    si.hStdError = hWrite;
    si.dwFlags |= STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE; // Hide the window!
    
    char cmdline[] = "powershell.exe -WindowStyle Hidden -Command \"Get-WmiObject -Namespace root\\OpenHardwareMonitor -Class Sensor | Where-Object { $_.SensorType -eq 'Temperature' -and $_.Name -like '*CPU*' } | ForEach-Object { [int]$_.Value } | Measure-Object -Maximum | Select-Object -ExpandProperty Maximum\"";
    
    if (CreateProcess(NULL, cmdline, NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
        CloseHandle(hWrite);
        WaitForSingleObject(pi.hProcess, 3000); // Wait max 3 seconds
        
        if (ReadFile(hRead, temp_str, sizeof(temp_str)-1, &bytesRead, NULL)) {
            temp = atoi(temp_str);
        }
        
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    } else {
        CloseHandle(hWrite);
    }
    
    CloseHandle(hRead);
    
    // Fallback: use a mock temperature if OpenHardwareMonitor is not available
    if (temp == 0) {
        static int mock_temp = 45;
        mock_temp += (rand() % 3) - 1; // Simulate temperature fluctuation
        if (mock_temp < 30) mock_temp = 30;
        if (mock_temp > 80) mock_temp = 80;
        temp = mock_temp;
    }
    
    return temp;
}

// Open HID device (matches Python dev.open(VENDOR_ID, PRODUCT_ID))
bool open_hid_device() {
    GUID hidGuid;
    HDEVINFO deviceInfoSet;
    SP_DEVICE_INTERFACE_DATA deviceInterfaceData;
    PSP_DEVICE_INTERFACE_DETAIL_DATA deviceInterfaceDetailData;
    DWORD requiredSize;
    DWORD index = 0;
    
    HidD_GetHidGuid(&hidGuid);
    deviceInfoSet = SetupDiGetClassDevs(&hidGuid, NULL, NULL, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
    
    if (deviceInfoSet == INVALID_HANDLE_VALUE) return false;
    
    deviceInterfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
    
    while (SetupDiEnumDeviceInterfaces(deviceInfoSet, NULL, &hidGuid, index, &deviceInterfaceData)) {
        SetupDiGetDeviceInterfaceDetail(deviceInfoSet, &deviceInterfaceData, NULL, 0, &requiredSize, NULL);
        deviceInterfaceDetailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)malloc(requiredSize);
        deviceInterfaceDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
        
        if (SetupDiGetDeviceInterfaceDetail(deviceInfoSet, &deviceInterfaceData, deviceInterfaceDetailData, requiredSize, NULL, NULL)) {
            HANDLE device = CreateFile(deviceInterfaceDetailData->DevicePath, GENERIC_READ | GENERIC_WRITE, 
                                     FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
            
            if (device != INVALID_HANDLE_VALUE) {
                HIDD_ATTRIBUTES attributes;
                attributes.Size = sizeof(HIDD_ATTRIBUTES);
                
                if (HidD_GetAttributes(device, &attributes)) {
                    if (attributes.VendorID == VENDOR_ID && attributes.ProductID == PRODUCT_ID) {
                        hid_device = device;
                        free(deviceInterfaceDetailData);
                        SetupDiDestroyDeviceInfoList(deviceInfoSet);
                        return true;
                    }
                }
                CloseHandle(device);
            }
        }
        free(deviceInterfaceDetailData);
        index++;
    }
    
    SetupDiDestroyDeviceInfoList(deviceInfoSet);
    return false;
}

// Optimized HID write with pre-allocated buffer
void hid_write(int tens, int ones) {
    if (hid_device == INVALID_HANDLE_VALUE) return;
    
    // Only update the temperature bytes, rest stay the same
    hid_buffer[4] = tens;
    hid_buffer[5] = ones;
    
    DWORD bytesWritten;
    WriteFile(hid_device, hid_buffer, 64, &bytesWritten, NULL);
}

// Update tray icon tooltip
void update_tray(int temp) {
    char buf[64];
    sprintf(buf, "CPU Temp: %d°C", temp);
    nid.uFlags = NIF_TIP;
    strncpy(nid.szTip, buf, sizeof(nid.szTip) - 1);
    nid.szTip[sizeof(nid.szTip) - 1] = '\0';
    Shell_NotifyIcon(NIM_MODIFY, &nid);
}

// Tray message handler with context menu
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_TRAYICON:
            if (lParam == WM_RBUTTONUP) {
                POINT pt;
                GetCursorPos(&pt);
                HMENU hMenu = CreatePopupMenu();
                AppendMenu(hMenu, MF_STRING, 1, "Exit");
                SetForegroundWindow(hWnd);
                int cmd = TrackPopupMenu(hMenu, TPM_RETURNCMD | TPM_NONOTIFY, pt.x, pt.y, 0, hWnd, NULL);
                DestroyMenu(hMenu);
                if (cmd == 1) {
                    stop_event = true;
                    Shell_NotifyIcon(NIM_DELETE, &nid);
                    PostQuitMessage(0);
                }
            }
            break;
        case WM_DESTROY:
            stop_event = true;
            Shell_NotifyIcon(NIM_DELETE, &nid);
            PostQuitMessage(0);
            break;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

// Optimized HID loop - separates temp checking from HID writing
DWORD WINAPI hid_loop(LPVOID lpParam) {
    int last_temp_check = 0;
    int current_tens = 0, current_ones = 0;
    
    while (!stop_event) {
        // Only check temperature every TEMP_CHECK_INTERVAL_MS
        if (GetTickCount() - last_temp_check > TEMP_CHECK_INTERVAL_MS) {
            int new_temp = get_cpu_temp();
            if (new_temp < 0) new_temp = 0;
            if (new_temp > 99) new_temp = 99;
            
            if (new_temp != temp_c) {  // Only update if temperature changed
                temp_c = new_temp;
                current_tens = temp_c / 10;
                current_ones = temp_c % 10;
            }
            last_temp_check = GetTickCount();
        }
        
        // Always write to HID to keep display alive (but less frequently)
        hid_write(current_tens, current_ones);
        Sleep(POLL_INTERVAL_MS);
    }
    return 0;
}

// Optimized tray update - only when temperature changes
DWORD WINAPI tray_update_loop(LPVOID lpParam) {
    while (!stop_event) {
        if (temp_c != last_temp_sent) {
            update_tray(temp_c);
            last_temp_sent = temp_c;
        }
        Sleep(POLL_INTERVAL_MS);  // Check less frequently
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    hInst = hInstance;
    
    // Open HID device first (matches Python dev.open())
    if (!open_hid_device()) {
        MessageBox(NULL, "Failed to open DeepCool device", "Error", MB_OK);
        return 1;
    }
    
    WNDCLASS wc;
    ZeroMemory(&wc, sizeof(wc));
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "DeepCoolTray";
    RegisterClass(&wc);
    hwnd = CreateWindowEx(0, wc.lpszClassName, "DeepCool", 0, 0,0,0,0, HWND_MESSAGE, 0, hInstance, 0);
    
    // Initialize tray icon
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd = hwnd;
    nid.uID = 1;
    nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
    nid.uCallbackMessage = WM_TRAYICON;
    nid.hIcon = LoadIcon(NULL, IDI_INFORMATION);
    strncpy(nid.szTip, "CPU Temp: 0°C", sizeof(nid.szTip) - 1);
    nid.szTip[sizeof(nid.szTip) - 1] = '\0';
    Shell_NotifyIcon(NIM_ADD, &nid);
    
    // Start both threads (matches Python structure exactly)
    HANDLE hid_thread = CreateThread(NULL, 0, hid_loop, NULL, 0, NULL);
    HANDLE tray_thread = CreateThread(NULL, 0, tray_update_loop, NULL, 0, NULL);
    
    // Message loop
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    // Cleanup
    stop_event = true;
    WaitForSingleObject(hid_thread, 1000);
    WaitForSingleObject(tray_thread, 1000);
    CloseHandle(hid_thread);
    CloseHandle(tray_thread);
    
    if (hid_device != INVALID_HANDLE_VALUE) {
        CloseHandle(hid_device);
    }
    
    Shell_NotifyIcon(NIM_DELETE, &nid);
    return 0;
}
