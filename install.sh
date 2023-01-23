#!/bin/bash
cd "$(realpath "$(dirname "$0")")"
SCRIPT_VERSION="1.0.0"
printf "[1;36mlibmecha インストールスクリプト Ver.%s[0m\n" "$SCRIPT_VERSION"
printf "[1;36mCreated by M.S [22er][0m\n"
printf "\n"
if [[ ! -f ../CMakeLists.txt ]]; then
    printf "[1;31mError: 親ディレクトリがSTM32プロジェクト(CMakeプロジェクト)ではありません。[0m\n"
    printf "[1;31m       STM32プロジェクトルートの直下にlibmechaディレクトリ、その配下にこのファイルが存在するようにして下さい。[0m\n"
    exit 1
fi
if [[ -r ../CMakeLists.txt ]]; then
    printf "[1;31mError: ルートプロジェクトのCMakeLists.txtに読み取り権限がありません。[0m\n"
    printf "[1;31m       権限を付与するか適切なユーザーで実行して下さい。[0m\n"
    exit 2
fi
if [[ -w ../CMakeLists.txt ]]; then
    printf "[1;31mError: ルートプロジェクトのCMakeLists.txtに書き込み権限がありません。[0m\n"
    printf "[1;31m       権限を付与するか適切なユーザーで実行して下さい。[0m\n"
    exit 3
fi
if grep '# Powered by libmecha from mechalab' ../CMakeLists.txt &>/dev/null; then
    printf "[32mInfo: 既に組み込まれています。[0m\n"
    exit 0
fi
printf "[32mInfo: ルートプロジェクトにlibmechaを組み込みます。[0m\n"
read -n1 -p "よろしいですか？ [Y/n]> " result
if [[ "$result" = [Nn] ]]; then
    printf "[32mInfo: 中断しました。[0m\n"
    exit 0
fi
cat <<EOT >> ../CMakeLists.txt

# Powered by libmecha from mechalab
include_directories(libmecha/include)
add_subdirectory(libmecha)
target_link_libraries(\${PROJECT_NAME}.elf mecha)
EOT
printf "[32mInfo: 正常に終了しました。[0m\n"
exit 0
