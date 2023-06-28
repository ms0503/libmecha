/*
 * IMotorDriver.hh
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

#pragma once

#include "LowLayer/Encoder.hh"
#include "LowLayer/Peripheral.hh"
#include <cstdint>

namespace LibMecha::MiddleLayer {
    /// モータードライバーインターフェース
    class IMotorDriver : public LowLayer::Peripheral {
    public:
        using Peripheral::Peripheral;

        /**
         * コンストラクター
         * @param maxSpeed 最高速度
         * @param encoder エンコーダー
         */
        explicit IMotorDriver(std::int32_t maxSpeed, LowLayer::Encoder encoder);

        /**
         * 初期化
         */
        virtual void init();

        /**
         * Duty比の設定
         * @param targetSpeed 目標速度
         * @return 設定完了
         */
        virtual bool setTarget(std::int32_t targetSpeed);

        /**
         * 最高速度の取得
         * @return 最高速度
         */
        [[nodiscard]] inline std::int32_t getMaxSpeed() const {
            return _maxSpeed;
        }

    protected:
        std::int32_t _maxSpeed;
        LowLayer::Encoder _encoder;
    };
}
