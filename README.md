# CapsLockLed-IME

将 `caps lock` 指示灯变成中英文指示灯。

## 用途

1. 先用 [PowerToys](https://github.com/microsoft/PowerToys) 把没什么用的 `caps lock` 映射成 `win + space`。之后就能用 `caps lock` 切换键盘布局（中英文）了。
2. 后台运行该程序，将中英文状态同步到 `caps lock` 的 LED 指示灯。

    - 灯亮：当前为中文。
    - 灯灭：当前为英文。

## 64 位构建安装

要求：

- CMake
- Visual Studio 2022
- Windows SDK

PowerShell 中进入仓库目录，执行

``` powershell
sudo ./install64.ps1
```

没有 `sudo` 的话，需要用管理员权限打开 PowerShell。

安装后会直接后台运行，开机时自动启动。

## 64 位卸载

``` powershell
sudo ./uninstall64.ps1
```

因为涉及全局钩子的卸载，所以需要花一点时间。
