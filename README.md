# libmecha
libmechaはモータードライバー、SBDBT、CANを操作するためのライブラリです。

## Dependencies
- STM32f407VGT(恐らくSTM32f4ファミリであれば大丈夫...なのだろうか)
- STM32の開発環境
  - 各種IDE
    - STM32CubeIDE
    - System Workbench for STM32 [Deprecated]
    - CLion
  - or
  - CMake 3.1〜
    - g++ 7.1〜

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
単体でビルドする場合、
- Linux/macOSの場合:
  bash:
  ```
  $ cd libmecha
  $ ./build.sh
  ```
- Windowsの場合:
  PowerShell:
  ```
  PS> Set-ExecutionPolicy -Scope Process Bypass
  PS> cd libmecha
  PS> .\build.ps1
  ```
  cmd.exe:
  ```
  > cd libmecha
  > powershell -ExecutionPolicy Bypass -Command '.\build.ps1'
  ```
とする事によりルートプロジェクトをビルドする事なくlibmechaをビルドする事が出来ます。
ただし、単体をビルドする場合でもルートのSTM32プロジェクトは必要です(各プリフェラルなどのヘッダを読み込むため)

## License
未定です。暫定的に[GNU Lesser General Public License, Version 3](https://www.gnu.org/licenses/lgpl-3.0.html)を適用します。
![GNU LGPL Logo](https://www.gnu.org/graphics/lgplv3-147x51.png)

## Author
- youda (library code)
- ms0503 (build system)

