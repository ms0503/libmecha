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
    inline namespace v2 {
        namespace LMLL = LowLayer;

        /// モーター
        enum class EnumMotor : std::uint8_t {
            FL,
            FR,
            RL,
            RR
        };

        /// モーターの回転方向
        enum class EnumMotorRotate : std::int8_t {
            LEFT = -1,
            RIGHT = 1
        };

        /// モーター制御用クラス
        class Motor {
        public:
            /// モーターアドレス
            struct Address {
                /// 左前方
                std::uint8_t FL;
                /// 右前方
                std::uint8_t FR;
                /// 左後方
                std::uint8_t RL;
                /// 右後方
                std::uint8_t RR;
            };

            /// モーター信号
            struct State {
                /// 左前方
                std::int32_t FL;
                /// 右前方
                std::int32_t FR;
                /// 左後方
                std::int32_t RL;
                /// 右後方
                std::int32_t RR;
            };

            /**
             * コンストラクタ
             * @param canHandle HALのCANハンドル
             */
            explicit Motor(const CAN_HandleTypeDef &canHandle);
            /**
             * コンストラクタ
             * @param canHandle HALのCANハンドル
             * @param addresses モータードライバーのCANアドレス
             */
            explicit Motor(const CAN_HandleTypeDef &canHandle, Address addresses);
            /**
             * コンストラクタ
             * @param canHandle HALのCANハンドル
             * @param addrFL 左前方のモータードライバーのCANアドレス
             * @param addrFR 右前方のモータードライバーのCANアドレス
             * @param addrRL 左後方のモータードライバーのCANアドレス
             * @param addrRR 右後方のモータードライバーのCANアドレス
             */
            explicit Motor(const CAN_HandleTypeDef &canHandle, std::uint8_t addrFL, std::uint8_t addrFR, std::uint8_t addrRL, std::uint8_t addrRR);
            /**
             * デストラクタ
             */
            ~Motor();
            /**
             * モーターの最高速度の取得
             * @return モーターの最高速度
             */
            static std::int32_t getMaxSpeed();
            /**
             * モーターの最高速度の設定
             * @param maxSpeed モーターの最高速度
             */
            static void setMaxSpeed(std::int32_t maxSpeed);
            /**
             * 初期化
             * @param canAddr メインボードのCANアドレス
             * @param maxSpeed モーターの最高速度
             */
            void init(std::uint8_t canAddr, std::int32_t maxSpeed);
            /**
             * モーター信号の更新
             * @param state モーター信号
             */
            void update(State state);

        private:
            /// Canクラスのインスタンス
            LMLL::Can _can;
            /// HALのCANハンドル
            CAN_HandleTypeDef _hcan;
            /// MotorDriverクラスのインスタンス
            LMLL::MotorDriver _md;
            /// モータードライバーのCANアドレス
            Address _addresses;
            /// モーターの最高速度
            static std::int32_t _maxSpeed;
        };
    }// namespace v2
}// namespace LibMecha

#endif// _LIBMECHA_MOTOR_HH_
