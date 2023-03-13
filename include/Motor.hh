/*
 * Motor.hh
 *
 *  Created on: 2023/03/07
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

#ifndef MYLIBINC_MOTOR_HH_
#define MYLIBINC_MOTOR_HH_

#include "LowLayer/MotorDriver.hh"
#include "params.hh"
#include <cstdint>
#include <map>
#include <string>

namespace LibMecha {
    inline namespace v1 {
        using namespace LowLayer;

        /// モーター信号
        typedef struct {
            /// 左前方
            int32_t FL;
            /// 右前方
            int32_t FR;
            /// 左後方
            int32_t RL;
            /// 右後方
            int32_t RR;
        } MotorState;

        /// モーターアドレス
        typedef struct {
            uint8_t FL;
            uint8_t FR;
            uint8_t RL;
            uint8_t RR;
        } MotorAddress;

        /// モーター
        enum class EnumMotor : uint8_t {
            FL,
            FR,
            RL,
            RR
        };

        /// モーターの回転方向
        enum class EnumMotorRotate : int8_t {
            LEFT = -1,
            RIGHT = 1
        };

        /// モーター制御クラス
        class Motor {
        public:
            explicit Motor(const CAN_HandleTypeDef &canHandle);
            explicit Motor(const CAN_HandleTypeDef &canHandle, MotorAddress addresses);
            explicit Motor(const CAN_HandleTypeDef &canHandle, uint8_t addrFL, uint8_t addrFR, uint8_t addrRL, uint8_t addrRR);
            ~Motor();
            void init(uint8_t canAddr);
            void update(MotorState state);

        private:
            Can _can;
            CAN_HandleTypeDef _hcan;
            MotorDriver _md;
            MotorAddress _addresses;
        };
    }
}

#endif // MYLIBINC_MOTOR_HH_
