#!/bin/bash
_error () {
    printf "[1;31mError: %s[m\n" "$1"
}
if [[ -z "$1" ]]; then
    _error "クラス名がありません。"
    exit 1
fi
if [[ -f "include/$1.hh" || -f "src/$1.cc" ]]; then
    _error "ファイルが既に存在しています。"
    exit 2
fi
BASENAME="${1##*/}"
cat <<EOF > "include/$1.hh"
/*
 * ${BASENAME}.hh
 *
 *  Created on: $(date +%Y/%m/%d)
 *      Author: ms0503
 *
 *  This file is part of libmecha.
 *
 *  libmecha is free software: you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 *
 *  libmecha is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License along with libmecha. If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

namespace LibMecha {
    class ${BASENAME} {
        explicit ${BASENAME}();
        ~${BASENAME}();
    };
}
EOF
cat <<EOF > "src/$1.cc"
/*
 * ${BASENAME}.cc
 *
 *  Created on: $(date +%y/%m/%d)
 *      Author: ms0503
 *
 *  This file is part of libmecha.
 *
 *  libmecha is free software: you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 *
 *  libmecha is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License along with libmecha. If not, see <https://www.gnu.org/licenses/>.
 */

#include "$1.hh"

namespace LibMecha {
    ${BASENAME}::${BASENAME}() {
    }
    ${BASENAME}::~${BASENAME}() {
    }
}
EOF
exit 0
