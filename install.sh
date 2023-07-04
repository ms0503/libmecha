#!/bin/bash
# shellcheck disable=SC2164
cd "$(realpath "$(dirname "$0")")"
SCRIPT_VERSION="1.1.0"
DETECT_ECLIPSE=false
DETECT_IDEA=false

_error() {
    printf "[1;31m[ERROR] %s[m\n" "$1"
}

_info() {
    printf "[32m[INFO]  %s[m\n" "$1"
}

printf "[1;36mlibmecha インストールスクリプト Ver.%s[m\n" "$SCRIPT_VERSION"
printf "[1;36mCreated by M.S [22er][0m\n"
printf "\n"
_info "環境を検出中..."
[[ -f ../.cproject && -f ../.mxproject && -f ../.project ]] && DETECT_ECLIPSE=true
[[ -d ../.idea ]] && DETECT_IDEA=true
if DETECT_IDEA; then
    if [[ ! -f ../CMakeLists.txt || ! -f ../CMakeLists_template.txt ]]; then
        _error "親ディレクトリがSTM32プロジェクト(CMakeプロジェクト)ではありません。"
        _error "STM32プロジェクトルートの直下にlibmechaディレクトリ、その配下にこのファイルが存在するようにして下さい。"
        exit 2
    fi
    if [[ ! -r ../CMakeLists.txt || ! -r ../CMakeLists_template.txt ]]; then
        _error "ルートプロジェクトのCMakeLists.txtに読み取り権限がありません。"
        _error "権限を付与するか適切なユーザーで実行して下さい。"
        exit 3
    fi
    if [[ ! -w ../CMakeLists.txt || ! -w ../CMakeLists_template.txt ]]; then
        _error "ルートプロジェクトのCMakeLists.txtに書き込み権限がありません。"
        _error "権限を付与するか適切なユーザーで実行して下さい。"
        exit 4
    fi
    if grep '# Powered by libmecha from mechalab' ../CMakeLists.txt &>/dev/null || grep '# Powered by libmecha from mechalab' ../CMakeLists_template.txt &>/dev/null; then
        _info "既に組み込まれています。"
        exit 5
    fi
elif DETECT_ECLIPSE; then
    if [[ (! -d ../Core || ! -d ../Core/Inc || ! -d ../Core/Src) && (! -d ../Inc || ! -d ../Src) ]]; then
        _error "ルートプロジェクトのインクルード・ソースディレクトリが見つかりません。"
        exit 6
    fi
    if [[ (! -w ../Core || ! -w ../Core/Inc || ! -w ../Core/Src) && (! -w ../Inc || ! -w ../Src) ]]; then
        _error "ルートプロジェクトのインクルード・ソースディレクトリに書き込み権限がありません。"
        _error "権限を付与するか適切なユーザーで実行して下さい。"
        exit 7
    fi
fi
_info "ルートプロジェクトにlibmechaを組み込みます。"
read -r -n1 -p "よろしいですか？ [Y/n]> " result
if [[ "$result" = [Nn] ]]; then
    _info "中断しました。"
    exit 8
fi
if DETECT_IDEA; then
    cat <<EOA >> ../CMakeLists.txt

# Powered by libmecha from mechalab
target_include_directories(\${PROJECT_NAME}.elf PRIVATE libmecha/include)
add_subdirectory(libmecha)
target_link_libraries(\${PROJECT_NAME}.elf PRIVATE mecha)
EOA
    cat <<EOA >> ../CMakeLists_template.txt

# Powered by libmecha from mechalab
target_include_directories(\$\${PROJECT_NAME}.elf PRIVATE libmecha/include)
add_subdirectory(libmecha)
target_link_libraries(\$\${PROJECT_NAME}.elf PRIVATE mecha)
EOA
elif DETECT_ECLIPSE; then
    if [[ -d ../Core ]]; then
        cp -arv ./include/* ../Core/Inc
        cp -arv ./src/* ../Core/Src
    else
        cp -arv ./include/* ../Inc
        cp -arv ./src/* ../Src
    fi
fi
_info "正常に終了しました。"
exit 0
