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
#include "IMotor.hh"

namespace LibMecha {
    Controller::Controller(const MotorControlType type):
        _sbdbt(), _bs(), _deadZones(), _type(type) {
    }

    Controller::~Controller() = default;

    std::int32_t Controller::stickToMotor(const std::uint8_t index) const {
        const LowLayer::SBDBT::AnalogState as = getStick();
        const float LX = _deadZones.at(0) < std::abs(as.LX) ? static_cast<float>(as.LX) : 0.0f;
        const float LY = _deadZones.at(1) < std::abs(as.LY) ? static_cast<float>(as.LY) : 0.0f;
        const float RX = _deadZones.at(2) < std::abs(as.RX) ? static_cast<float>(as.RX) : 0.0f;
        const float RY = _deadZones.at(3) < std::abs(as.RY) ? static_cast<float>(as.RY) : 0.0f;
        const StickTheta theta = sticksToTheta(LX, LY, RX, RY); // 偏角・スティックの角度(右0、反時計回りが正、-π < x <= π)
        if(_type == MotorControlType::TRIANGLE) {
            const float r = std::hypot(LX, LY) / STICK_MAX; // 動径
            if(RX != 0.0f) return static_cast<std::int32_t>(RX * 0.02f * static_cast<float>(LibMecha::maxSpeed));
            if(index == 0) return static_cast<std::int32_t>(std::sin(theta.left + M_PI_4) * r * LibMecha::maxSpeed);
            if(index == 1) return static_cast<std::int32_t>(-std::sin(theta.left - M_PI_4) * r * LibMecha::maxSpeed);
            if(index == 2) return static_cast<std::int32_t>(-std::sin(theta.left + M_PI_4) * r * LibMecha::maxSpeed);
            if(index == 3) return static_cast<std::int32_t>(std::sin(theta.left - M_PI_4) * r * LibMecha::maxSpeed);
        } else if(_type == MotorControlType::VECTOR) {
            // 右前方
            if(index == 0) {
                const float x = std::sin(theta.left + static_cast<float>(M_PI_4));
                const float steering = RX < 0.0f ? 1.0f + (RX / 64.0f) : 1.0f - (RX / 64.0f);
                return static_cast<std::int32_t>(x * steering * static_cast<float>(LibMecha::maxSpeed));
            }
            // 左前方
            if(index == 1) {
                const float x = -std::sin(theta.left - static_cast<float>(M_PI_4));
                const float steering = 0.0f < RX ? 1.0f + (-RX / 64.0f) : 1.0f - (-RX / 64.0f);
                return static_cast<std::int32_t>(x * steering * static_cast<float>(LibMecha::maxSpeed));
            }
            // 左後方
            if(index == 2) {
                const float x = -std::sin(theta.left + static_cast<float>(M_PI_4));
                const float steering = 0.0f < RX ? 1.0f + (-RX / 64.0f) : 1.0f - (-RX / 64.0f);
                return static_cast<std::int32_t>(x * steering * static_cast<float>(LibMecha::maxSpeed));
            }
            // 右後方
            if(index == 3) {
                const float x = std::sin(theta.left - static_cast<float>(M_PI_4));
                const float steering = RX < 0.0f ? 1.0f + (RX / 64.0f) : 1.0f - (RX / 64.0f);
                return static_cast<std::int32_t>(x * steering * static_cast<float>(LibMecha::maxSpeed));
            }
        }
        return 0;
    }
} // namespace LibMecha
