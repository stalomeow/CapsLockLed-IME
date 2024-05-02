$AppName = "CapsLockLed-IME"
$AppPath = "C:\Program Files\CapsLockLed-IME\bin\capslockled-main.exe"
$DllPath = "C:\Program Files\CapsLockLed-IME\bin\capslockled-hook.dll"

# 杀掉进程
Get-Process -Name "capslockled-main" -ErrorAction SilentlyContinue | Stop-Process

# 删除开机自启设置
Remove-ItemProperty -Path 'HKLM:\Software\Microsoft\Windows\CurrentVersion\Run' -Name $AppName

# 尝试删除DLL，直到成功
while($true) {
    try {
        Remove-Item -Path $DllPath -ErrorAction Stop
        Write-Output "DLL removed successfully."
        break
    } catch {
        Write-Warning "Removing DLL..."
        Start-Sleep -Seconds 2
    }
}

# 删除应用程序文件
Remove-Item -Path "C:\Program Files\CapsLockLed-IME" -Recurse -Force
