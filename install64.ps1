$AppName = "CapsLockLed-IME"
$AppPath = "C:\Program Files\CapsLockLed-IME\bin\capslockled-main.exe"

# 杀掉旧的进程
Get-Process -Name "capslockled-main" -ErrorAction SilentlyContinue | Stop-Process

# 构建
New-Item -Path "." -Name "build" -ItemType "directory" -Force | Push-Location

cmake -G "Visual Studio 17 2022" -A x64 -S ..
cmake --build . --config Release
cmake --install . --config Release

Pop-Location
Remove-Item "build" -Recurse -Force

# 设置开机自启
Set-ItemProperty -Path 'HKLM:\Software\Microsoft\Windows\CurrentVersion\Run' -Name $AppName -Value $AppPath

# 启动
Start-Process $AppPath
