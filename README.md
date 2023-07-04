# libmecha
libmechaはSTM32 HAL/LL APIの抽象化、各部品向けのクラスの提供を目的としたSTM32 MCU向けライブラリーです。

## Dependencies
- STM32 MCU
- STM32の開発環境
  - 各種IDE
    - JetBrains CLion [推奨]
    - STM32CubeIDE
    - Ac6 System Workbench for STM32 [非推奨]
  - CMake 3.8〜
    - g++(arm-none-eabi) 7.1〜

## Installing
まず、STM32プロジェクトを作ります。
そしてプロジェクトルートにlibmechaディレクトリを配置し、
- Linux/macOSの場合:  
  bash:
  ```
  $ ./libmecha/install.sh
  ```
- Windowsの場合:  
  Powershell:
  ```
  PS> Set-ExecutionPolicy -Scope Process Bypass
  PS> .\libmecha\install.ps1
  ```
  cmd.exe:
  ```
  > powershell -ExecutionPolicy Bypass -Command '.\libmecha\install.ps1'
  ```
をプロジェクトルートで実行。
その後、ルートプロジェクトを再読み込みする事によりインストールが完了します。

## Using
[Installing](#installing)を実行した後であればルートプロジェクトをビルドする際に一緒にビルドされます。

## License
[![GNU Lesser General Public License, Version 3](https://www.gnu.org/graphics/lgplv3-147x51.png)](https://www.gnu.org/licenses/lgpl-3.0.html)

## Author
- youda (low layer code)
- ms0503 (front layer code, build system)

