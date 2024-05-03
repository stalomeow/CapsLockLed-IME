#include <Windows.h>

typedef void (*InstallerFunc)(HINSTANCE hinstDLL);

HANDLE hMutex = NULL;
HINSTANCE hinstDLL = NULL;
InstallerFunc InstallHook = NULL;
InstallerFunc UninstallHook = NULL;

int Dispose(int retCode)
{
    if (hMutex)
    {
        CloseHandle(hMutex);
    }

    if (hinstDLL)
    {
        if (UninstallHook)
        {
            UninstallHook(hinstDLL);
        }
        FreeLibrary(hinstDLL);
    }

    return retCode;
}

int main()
{
    hMutex = CreateMutex(NULL, FALSE, TEXT("CapsLockLed-IME-Mutex"));

    // 如果已经有一个实例在运行，则退出
    if (hMutex == NULL || GetLastError() == ERROR_ALREADY_EXISTS)
    {
        return Dispose(1);
    }

    // 加载 Hook DLL
    if (!(hinstDLL = LoadLibrary(TEXT("capslockled-hook.dll"))))
    {
        return Dispose(2);
    }

    InstallHook = (InstallerFunc)GetProcAddress(hinstDLL, "Install");
    UninstallHook = (InstallerFunc)GetProcAddress(hinstDLL, "Uninstall");

    if (!InstallHook || !UninstallHook)
    {
        return Dispose(3);
    }

    InstallHook(hinstDLL);

    MSG msg;
    BOOL bRet;
    while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0)
    {
        // GetMessage() 返回 -1 表示出错，返回 0 表示 WM_QUIT 消息
        if (bRet == -1)
        {
            return Dispose(4);
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return Dispose(0);
}
