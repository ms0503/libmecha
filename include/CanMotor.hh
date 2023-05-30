/*
 * CanMotor.hh
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

#ifndef LIBMECHA_MOTOR_HH_
#define LIBMECHA_MOTOR_HH_

#include "IMotor.hh"
#include "LowLayer/CanMotorDriver.hh"
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
    class CanMotor : public IMotor<LowLayer::CanMotorDriver> {
    public:
        /**
         * コンストラクタ
         * @param canHandle HALのCANハンドル
         * @param can Canクラスのインスタンス
         * @param address CANアドレス
         */
        explicit CanMotor(CAN_HandleTypeDef &canHandle, LowLayer::Can &can, std::uint8_t address);
        /**
         * デストラクタ
         */
        ~CanMotor() override;
        /**
         * 初期化
         * @param canAddr メインボードのCANアドレス
         * @param maxSpeed モーターの最高速度
         */
        void init(std::uint8_t canAddr, std::int32_t maxSpeed);

    private:
        /// Canクラスのインスタンス
        LowLayer::Can _can;
        /// HALのCANハンドル
        CAN_HandleTypeDef &_hcan;
        /// モータードライバーのCANアドレス
        std::uint8_t _address;
    };
} // namespace LibMecha

#endif // LIBMECHA_MOTOR_HH_
