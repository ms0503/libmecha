/*
 * IMotor.hh
 *
 *  Created on: 2023/05/22
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

#ifndef LIBMECHA_IMOTOR_HH_
#define LIBMECHA_IMOTOR_HH_

#include "LowLayer/IMotorDriver.hh"
#include <cstdint>

namespace LibMecha {
    /// モーターの最高速度
    extern std::int32_t maxSpeed;

    template<class MotorDriver>
    class IMotor {
    public:
        /**
         * デストラクタ
         */
        virtual ~IMotor() = 0;
        /**
         * モーター信号の更新
         * @param duty モーター信号
         */
        virtual inline void update(const std::int32_t duty) const {
            _md.setDuty(duty);
        }

    protected:
        /// MotorDriverクラスのインスタンス
        MotorDriver _md;
    };
}

#endif // LIBMECHA_IMOTOR_HH_
