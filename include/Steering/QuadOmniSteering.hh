/*
 * QuadOmniSteering.hh
 *
 *  Created on: 2023/06/23
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

#include "Controller.hh"
#include "ISteering.hh"
#include "MiddleLayer/IMotorDriver.hh"
#include <array>
#include <concepts>
#include <cstdint>

namespace LibMecha::Steering {
    /**
     * 4輪オムニホイール駆動足回り
     * @tparam MotorDriver モータードライバー
     */
    template<class MotorDriver> requires std::derived_from<MotorDriver, LowLayer::IMotorDriver> class QuadOmniSteering : public ISteering<MotorDriver> {
    public:
        using ISteering<MotorDriver>::ISteering;

        /**
         * コンストラクター
         * @param md モータードライバー
         */
        explicit QuadOmniSteering(std::array<MotorDriver, 4> md);

        /**
         * デストラクター
         */
        ~QuadOmniSteering();

        void polarInput(float f, LibMecha::Controller<QuadOmniSteering<MotorDriver>>::StickTheta theta) const override;

        void forward(std::int32_t speed) const override;

        void backward(std::int32_t speed) const override;

        void left(std::int32_t speed) const override;

        void right(std::int32_t speed) const override;

        void turnLeft(std::int32_t speed) const override;

        void turnRight(std::int32_t speed) const override;

    private:
        /// モータードライバー配列
        const std::array<MotorDriver, 4> _md {};
    };
}
