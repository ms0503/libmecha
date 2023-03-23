/*
 * Controller.cc
 *
 *  Created on: 2023/03/08
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

#include "Controller.hh"
#include "Motor.hh"

namespace LibMecha {
    Controller::Controller():
        _sbdbt(), _bs(), _deadZones() {
    }

    Controller::~Controller() = default;

    std::int32_t Controller::stickToMotor(const std::uint8_t index) const {
        const LowLayer::SBDBT::AnalogState as = getStick();
        const float LX = as.LX;
        const float LY = as.LY;
        const float RX = as.RX;
        const float RY = as.RY;
        const StickTheta theta = sticksToTheta(LX, LY, RX, RY); // スティックの角度(右0、反時計回りが正、-π < x <= π)
        // 右前方
        if(index == 0) {
            const float x = std::sin(theta.left + static_cast<float>(M_PI_4));
            const float steering = RX < 0.0f ? 1.0f + (RX / 64.0f) : 1.0f - (RX / 64.0f);
            return static_cast<std::int32_t>(x * steering * static_cast<float>(Motor::getMaxSpeed()));
        }
        // 左前方
        if(index == 1) {
            const float x = -std::sin(theta.left - static_cast<float>(M_PI_4));
            const float steering = 0.0f < RX ? 1.0f + (-RX / 64.0f) : 1.0f - (-RX / 64.0f);
            return static_cast<std::int32_t>(x * steering * static_cast<float>(Motor::getMaxSpeed()));
        }
        // 左後方
        if(index == 2) {
            const float x = -std::sin(theta.left + static_cast<float>(M_PI_4));
            const float steering = 0.0f < RX ? 1.0f + (-RX / 64.0f) : 1.0f - (-RX / 64.0f);
            return static_cast<std::int32_t>(x * steering * static_cast<float>(Motor::getMaxSpeed()));
        }
        // 右後方
        if(index == 3) {
            const float x = std::sin(theta.left - static_cast<float>(M_PI_4));
            const float steering = RX < 0.0f ? 1.0f + (RX / 64.0f) : 1.0f - (RX / 64.0f);
            return static_cast<std::int32_t>(x * steering * static_cast<float>(Motor::getMaxSpeed()));
        }
        return 0;
    }
} // namespace LibMecha
