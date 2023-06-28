#!/usr/bin/node
'use strict';

const { access, writeFile } = require('fs/promises');
const { basename, dirname } = require('path');

const ERR_TOO_FEW_ARGS = 1;
const ERR_FILE_EXISTS = 2;

async function main(argc, argv) {
    if(argc < 2) {
        error("クラス名がありません。");
        return ERR_TOO_FEW_ARGS;
    }
    const [_, filename, ...args] = argv;
    const ns = dirname(filename).split('/').filter(e => e !== '.').reduce((p, c) => `${p}::${c}`, 'LibMecha');
    const bn = basename(filename);
    try {
        await access(`include/${filename}.hh`);
        await access(`src/${filename}.cc`);
        error('ファイルが既に存在しています。');
        return ERR_FILE_EXISTS;
    } catch(e) {
    }
    const date = new Date();
    const d = `${date.getFullYear()}/${date.getMonth()}/${date.getDate()}`;
    await Promise.all([writeFile(`include/${filename}.hh`, `/*
 * ${bn}.hh
 *
 *  Created on: ${d}
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

namespace ${ns} {
    class ${bn} {
        explicit ${bn}();
        ~${bn}();
    };
}
`), writeFile(`src/${filename}.cc`, `/*
 * ${bn}.cc
 *
 *  Created on: ${d}
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

#include "${filename}.hh"

namespace ${ns} {
    ${bn}::${bn}() {
    }
    ${bn}::~${bn}() {
    }
}
`)]);
    return 0;
}

function error(msg) {
    console.error(`[1;31mError: ${msg}[m`);
}

(async (argc, argv) => {
    argv.shift();
    const exitCode = await main(argc, argv);
    process.exit(exitCode);
})(process.argv.length - 1, process.argv);
