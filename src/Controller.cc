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
#include <cmath>
#include <map>

namespace LibMecha {
    inline namespace v2 {
        Controller::Controller():
            _sbdbt(), _bs(), _deadZones() {
        }

        Controller::~Controller() = default;

        std::int32_t Controller::stickToMotor(const std::uint8_t index) {
            const LowLayer::SBDBT::AnalogState as = getStick();
            const float LX = as.LX;
            const float LY = as.LY;
            const float RX = as.RX;
            const float RY = as.RY;
            // 極座標
            const float r = std::hypot(LX, LY) / STICK_MAX; // 動径
            const StickTheta theta = sticksToTheta(LX, LY, RX, RY); // 偏角(右0、反時計回りが正、-π < theta <= π)
            switch(index) {
                case 0:
                    return static_cast<std::int32_t>(-std::sin(theta.left - M_PI_4) * r * Motor::getMaxSpeed());
                case 1:
                    return static_cast<std::int32_t>(std::sin(theta.left + M_PI_4) * r * Motor::getMaxSpeed());
                case 2:
                    return static_cast<std::int32_t>(-std::sin(theta.left - M_PI_4) * r * Motor::getMaxSpeed());
                case 3:
                    return static_cast<std::int32_t>(std::sin(theta.left + M_PI_4) * r * Motor::getMaxSpeed());
                default:
                    break;
            }
            return 0;
        }

        float Controller::stickToTheta(const float x, const float y) {
            return std::atan2(y, x);
        }

        Controller::StickTheta Controller::sticksToTheta(const float leftX, const float leftY, const float rightX, const float rightY) {
            return {
                .left = stickToTheta(leftX, leftY),
                .right = stickToTheta(rightX, rightY)
            };
        }
    } // namespace v2
} // namespace LibMecha