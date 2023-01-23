set-location "$PSScriptRoot"
[string] $SCRIPT_VERSION = "1.0.0"
write-host "libmecha インストールスクリプト Ver.$SCRIPT_VERSION" -foregroundcolor cyan
write-host "Created by M.s [22er]" -foregroundcolor cyan
write-host
if((test-path "..\CMakeLists.txt") -eq "False") {
    write-host "Error: 親ディレクトリがSTM32プロジェクト(CMakeプロジェクト)ではありません。" -foregroundcolor red
    write-host "       STM32プロジェクトルートの直下にlibmechaディレクトリ、その配下にこのファイルが存在するようにして下さい。" -foregroundcolor red
    exit 1
}
if(((get-acl "..\CMakeLists.txt").Access | where-object {$_.IdentityReference -match "$User" -and $_.AccessControlType -match "Allow"} | foreach-object {$t = $False} {$t = (($_FileSystemRights -band [System.Security.AccessControl.FileSystemRights]::Read) -eq [System.Security.AccessControl.FileSystemRights]::Read) -or $t} {$t}) -eq "False") {
    write-host "Error: ルートプロジェクトのCMakeLists.txtに読み取り権限がありません。" -foregroundcolor red
    write-host "       権限を付与するか適切なユーザーで実行して下さい。" -foregroundcolor red
    exit 2
}
if(((get-acl "..\CMakeLists.txt").Access | where-object {$_.IdentityReference -match "$User" -and $_.AccessControlType -match "Allow"} | foreach-object {$t = $False} {$t = (($_FileSystemRights -band [System.Security.AccessControl.FileSystemRights]::Write) -eq [System.Security.AccessControl.FileSystemRights]::Write) -or $t} {$t}) -eq "False") {
    write-host "Error: ルートプロジェクトのCMakeLists.txtに書き込み権限がありません。" -foregroundcolor red
    write-host "       権限を付与するか適切なユーザーで実行して下さい。" -foregroundcolor red
    exit 3
}
if((get-content "..\CMakeLists.txt" | select-string -quiet "# Powered by libmecha from mechalab")) {
    write-host "Info: 既に組み込まれています。" -foregroundcolor green
    exit 0
}
write-host "Info: ルートプロジェクトにlibmechaを組み込みます。" -foregroundcolor green
$result = $host.ui.PromptForChoice("", "よろしいですか？", [System.Management.Automation.Host.ChoiceDescription[]]((new-object System.Management.Automation.Host.ChoiceDescription "&y" "インストールを実行する。"), (new-object System.Management.Automation.Host.ChoiceDescription "&n" "インストールを中断する。")), 0)
if($result -eq 1) {
    write-host "Info: 中断しました。" -foregroundcolor green
    exit 0
}
write-output @"

# Powered by libmecha from mechalab
include_directories(libmecha/include)
add_subdirectory(libmecha)
target_link_libraries(`${PROJECT_NAME}.elf mecha)
"@ | out-file -append "..\CMakeLists.txt"
write-host "Info: 正常に終了しました。" -foreground green
exit 0
