#include <Windows.h>

typedef BOOL (*BoolFunc)();
typedef void (*InstallerFunc)(HINSTANCE hinstDLL);

HINSTANCE hinstDLL = NULL;
InstallerFunc InstallHook = NULL;
InstallerFunc UninstallHook = NULL;

void OnAppExit(void)
{
    if (hinstDLL)
    {
        if (UninstallHook)
        {
            UninstallHook(hinstDLL);
        }
        FreeLibrary(hinstDLL);
    }
}

int main()
{
    atexit(OnAppExit);

    if (!(hinstDLL = LoadLibrary(TEXT("capslockled-hook.dll"))))
    {
        return 1;
    }

    if (((BoolFunc)GetProcAddress(hinstDLL, "IsInstalled"))())
    {
        return 2;
    }

    InstallHook = (InstallerFunc)GetProcAddress(hinstDLL, "Install");
    UninstallHook = (InstallerFunc)GetProcAddress(hinstDLL, "Uninstall");

    if (!InstallHook || !UninstallHook)
    {
        return 3;
    }

    InstallHook(hinstDLL);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
