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

#ifndef _LIBMECHA_MOTOR_HH_
#define _LIBMECHA_MOTOR_HH_

#include "LowLayer/MotorDriver.hh"
#include <cstdint>
#include <map>
#include <string>

namespace LibMecha {
    /// モーター
    enum class EnumMotor : std::uint8_t {
        FR,
        FL,
        RL,
        RR
    };

    /// モーター制御用クラス
    class Motor {
    public:
        /**
         * コンストラクタ
         * @param canHandle HALのCANハンドル
         * @param can Canクラスのインスタンス
         * @param address CANアドレス
         */
        explicit Motor(CAN_HandleTypeDef &canHandle, LowLayer::Can &can, std::uint8_t address);
        /**
         * デストラクタ
         */
        ~Motor();
        /**
         * モーターの最高速度の取得
         * @return モーターの最高速度
         */
        static inline std::int32_t getMaxSpeed() {
            return _maxSpeed;
        }
        /**
         * モーターの最高速度の設定
         * @param maxSpeed モーターの最高速度
         */
        static inline void setMaxSpeed(const std::int32_t maxSpeed) {
            _maxSpeed = maxSpeed;
        }
        /**
         * 初期化
         * @param canAddr メインボードのCANアドレス
         * @param maxSpeed モーターの最高速度
         */
        void init(std::uint8_t canAddr, std::int32_t maxSpeed);
        /**
         * モーター信号の更新
         * @param duty モーター信号
         */
        inline void update(const std::int32_t duty) const {
            _md.setDuty(duty);
        }

    private:
        /// Canクラスのインスタンス
        LowLayer::Can _can;
        /// HALのCANハンドル
        CAN_HandleTypeDef &_hcan;
        /// MotorDriverクラスのインスタンス
        LowLayer::MotorDriver _md;
        /// モータードライバーのCANアドレス
        std::uint8_t _address;
        /// モーターの最高速度
        static std::int32_t _maxSpeed;
    };
} // namespace LibMecha

#endif // _LIBMECHA_MOTOR_HH_
