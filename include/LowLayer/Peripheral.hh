/*
 * Peripheral.hh
 *
 *  Created on: 2023/03/20
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

#ifndef LIBMECHA_PERIPHERAL_HH_
#define LIBMECHA_PERIPHERAL_HH_

namespace LibMecha::LowLayer {
    /// ペリフェラル識別用ベースクラス
    class Peripheral {
    public:
        /**
         * コンストラクタ
         */
        explicit Peripheral();
        /**
         * デストラクタ
         */
        virtual ~Peripheral();
    };
} // namespace LibMecha::LowLayer

#endif // LIBMECHA_PERIPHERAL_HH_
