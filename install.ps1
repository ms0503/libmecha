Set-Location "$PSScriptRoot"
[string] $SCRIPT_VERSION = "1.0.2"
[bool] $DETECT_ECLIPSE = $false
[bool] $DETECT_IDEA = $false

function _error {
    Write-Host "[ERROR] $args" -ForegroundColor Red
}

function _info {
    Write-Host "[INFO]  $args" -ForegroundColor Green
}

function _getPerm([string] $name, [System.Security.AccessControl.FileSystemRights] $perm) {
    return ((Get-Acl "$name").Access | Where-Object {$_.IdentityReference -match "$User" -and $_.AccessControlType -match "Allow"} | ForEach-Object {$t = $False} {$t = (($_FileSystemRights -band $perm) -eq $perm) -or $t} {$t}) -eq "False"
}

Write-Host "libmecha インストールスクリプト Ver.$SCRIPT_VERSION" -ForegroundColor Cyan
Write-Host "Created by M.s [22er]" -ForegroundColor Cyan
Write-Host
_info "環境を検出中..."
if((Test-Path "..\.cproject") -and (Test-Path "..\.mxproject") -and (Test-Path "..\.project")) {
    $DETECT_ECLIPSE = $true
}
if(Test-Path "..\.idea") {
    $DETECT_IDEA = $true;
}
if($DETECT_IDEA) {
    if(!(Test-Path "..\CMakeLists.txt") -or !(Test-Path "..\CMakeLists_template.txt")) {
        _error "親ディレクトリがSTM32プロジェクト(CMakeプロジェクト)ではありません。"
        _error "STM32プロジェクトルートの直下にlibmechaディレクトリ、その配下にこのファイルが存在するようにして下さい。"
        exit 2
    }
    if((_getPerm "..\CMakeLists.txt" [System.Security.AccessControl.FileSystemRights]::Read) -and (_getPerm "..\CMakeLists_template.txt" [System.Security.AccessControl.FileSystemRights]::Read)) {
        _error "ルートプロジェクトのCMakeLists.txtに読み取り権限がありません。"
        _error "権限を付与するか適切なユーザーで実行して下さい。"
        exit 3
    }
    if((_getPerm "..\CMakeLists.txt" [System.Security.AccessControl.FileSystemRights]::Write) -and (_getPerm "..\CMakeLists_template.txt" [System.Security.AccessControl.FileSystemRights]::Write)) {
        _error "ルートプロジェクトのCMakeLists.txtに書き込み権限がありません。"
        _error "権限を付与するか適切なユーザーで実行して下さい。"
        exit 4
    }
    if((Get-Content "..\CMakeLists.txt" | Select-String -Quiet "# Powered by libmecha from mechalab") -or (Get-Content "..\CMakeLists_template.txt" | Select-String -Quiet "# Powered by libmecha from mechalab")) {
        _info "既に組み込まれています。"
        exit 5
    }
} elseif($DETECT_ECLIPSE) {
    if((!(Test-Path "..\Core") -or !(Test-Path "..\Core\Inc") -or !(Test-Path "..\Core\Src")) -and (!(Test-Path "..\Inc") -or !(Test-Path "..\Src"))) {
        _error "ルートプロジェクトのインクルード・ソースディレクトリが見つかりません。"
        exit 6
    }
    if((!(_getPerm "..\Core" [System.Security.AccessControl.FileSystemRights]::Write) -or !(_getPerm "..\Core\Inc" [System.Security.AccessControl.FileSystemRights]::Write) -or !(_getPerm "..\Core\Src" [System.Security.AccessControl.FileSystemRights]::Write)) -and (!(_getPerm "..\Inc" [System.Security.AccessControl.FileSystemRights]::Write) -or !(_getPerm "..\Src" [System.Security.AccessControl.FileSystemRights]::Write))) {
        _error "ルートプロジェクトのインクルード・ソースディレクトリに書き込み権限がありません。"
        _error "権限を付与するか適切なユーザーで実行して下さい。"
        exit 7
    }
}
_info "ルートプロジェクトにlibmechaを組み込みます。"
$result = $Host.UI.PromptForChoice("", "よろしいですか？", [System.Management.Automation.Host.ChoiceDescription[]]((New-Object System.Management.Automation.Host.ChoiceDescription "&y" "インストールを実行する。"), (New-Object System.Management.Automation.Host.ChoiceDescription "&n" "インストールを中断する。")), 0)
if($result -eq 1) {
    Write-Host "Info: 中断しました。" -ForegroundColor Green
    exit 8
}
if($DETECT_IDEA) {
    Write-Output @"

# Powered by libmecha from mechalab
target_include_directories(`${PROJECT_NAME}.elf PRIVATE libmecha/include)
add_subdirectory(libmecha)
target_link_libraries(`${PROJECT_NAME}.elf PRIVATE mecha)
"@ | Out-File -Append "..\CMakeLists.txt"
    Write-Output @"

# Powered by libmecha from mechalab
target_include_directories(`$`${PROJECT_NAME}.elf PRIVATE libmecha/include)
add_subdirectory(libmecha)
target_link_libraries(`$`${PROJECT_NAME}.elf PRIVATE mecha)
"@ | Out-File -Append "..\CMakeLists_template.txt"
} elseif($DETECT_ECLIPSE) {
    if(Test-Path "..\Core") {
        Copy-Item -Recurse ".\include\*" -Destination "..\Core\Inc"
        Copy-Item -Recurse ".\src\*" -Destination "..\Core\Src"
    } else {
        Copy-Item -Recurse ".\include\*" -Destination "..\Inc"
        Copy-Item -Recurse ".\src\*" -Destination "..\Src"
    }
}
_info "正常に終了しました。"
exit 0
