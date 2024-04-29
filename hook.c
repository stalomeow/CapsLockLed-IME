#include <Windows.h>
#include <ntddkbd.h>
#include <string.h>

#pragma data_seg("shared_hook_data")
    HHOOK hHook = NULL;
#pragma data_seg()
#pragma comment(linker, "/SECTION:shared_hook_data,RWS")


BOOL IsChineseKeyboardLayout()
{
    CHAR name[KL_NAMELENGTH];

    if (GetKeyboardLayoutNameA((LPSTR)&name))
    {
        // https://learn.microsoft.com/en-us/globalization/keyboards/kbdus_2
        if (strcmp(name, "00000804") == 0)
        {
            return TRUE;
        }
    }

    return FALSE;
}

int SetCapsLockIndicator(BOOL on)
{
    KEYBOARD_INDICATOR_PARAMETERS kip = { 0 };

    if (!DefineDosDevice(DDD_RAW_TARGET_PATH, TEXT("myKBD"), TEXT("\\Device\\KeyboardClass0")))
    {
        return -1;
    }

    HANDLE device = CreateFile(TEXT("\\\\.\\myKBD"), GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (device == INVALID_HANDLE_VALUE)
    {
        return -2;
    }

    DWORD bytesReturned;
    if (!DeviceIoControl(device, IOCTL_KEYBOARD_QUERY_INDICATORS, NULL, 0, &kip, sizeof(kip), &bytesReturned, NULL))
    {
        return -3;
    }

    if (on)
    {
        kip.LedFlags = (USHORT)(kip.LedFlags | KEYBOARD_CAPS_LOCK_ON);
    }
    else
    {
        kip.LedFlags = (USHORT)(kip.LedFlags & ~KEYBOARD_CAPS_LOCK_ON);
    }

    if (!DeviceIoControl(device, IOCTL_KEYBOARD_SET_INDICATORS, &kip, sizeof(kip), NULL, 0, &bytesReturned, NULL))
    {
        return -4;
    }

    if (!CloseHandle(device))
    {
        return -5;
    }

    if (!DefineDosDevice(DDD_REMOVE_DEFINITION, TEXT("myKBD"), NULL))
    {
        return -6;
    }

    return 0;
}

int SyncCapsLock()
{
    return SetCapsLockIndicator(IsChineseKeyboardLayout());
}

LRESULT CALLBACK ShellProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HSHELL_LANGUAGE)
    {
        SyncCapsLock();
    }

    return CallNextHookEx(hHook, nCode, wParam, lParam);
}

BOOL __declspec(dllexport) IsInstalled()
{
    return hHook != NULL;
}

void __declspec(dllexport) Install(HINSTANCE hinstDLL)
{
    if ((hHook = SetWindowsHookEx(WH_SHELL, ShellProc, hinstDLL, 0)))
    {
        SyncCapsLock();
    }
}

void __declspec(dllexport) Uninstall(HINSTANCE hinstDLL)
{
    if (hHook && UnhookWindowsHookEx(hHook))
    {
        SetCapsLockIndicator(FALSE);
        hHook = NULL;
    }
}
