/*
 * CytronMotorDriver.hh
 *
 *  Created on: 2023/05/23
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

#include "IMotorDriver.hh"
#include "LowLayer/Encoder.hh"
#include "LowLayer/GPIO.hh"
#include "stm32f4xx_ll_tim.h"
#include <cstdint>

namespace LibMecha::MiddleLayer {
    /// Cytron
    class CytronMotorDriver : public IMotorDriver {
    public:
        using IMotorDriver::IMotorDriver;

        /**
         * コンストラクター
         * @param maxSpeed 最高速度
         * @param encoder エンコーダー
         * @param tim タイマー
         * @param timCh タイマーのチャンネル
         * @param dirPin 回転方向指定用ピン
         * @param tm 1次遅れ特性時定数
         * @param tm2 目標時定数
         */
        explicit CytronMotorDriver(std::int32_t maxSpeed, LowLayer::Encoder &encoder, TIM_TypeDef *tim, std::uint32_t timCh, LowLayer::GPIO::Pin &dirPin);

        void init() override;

        bool setTarget(std::int32_t targetSpeed) override;

    private:
        /// 回転方向制御
        class Direction : public LowLayer::GPIO {
        public:
            using GPIO::GPIO;

            /**
             * コンストラクター
             * @param pin
             */
            explicit Direction(GPIO::Pin &pin);

            /**
             * 左回転
             */
            inline void left() {
                high(0);
            }

            /**
             * 右回転
             */
            inline void right() {
                low(0);
            }
        };

        /// タイマー
        TIM_TypeDef *const _tim;
        /// タイマーのチャンネル
        const std::uint32_t _timCh;
        /// 回転方向制御
        Direction _dir;
    };
}
