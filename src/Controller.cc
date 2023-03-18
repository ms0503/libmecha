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
#include <functional>
#include <map>

namespace LibMecha {
    inline namespace v2 {
        const std::map<EnumMotor, std::int8_t> Controller::DEAD_ZONES = {
            { EnumMotor::FL, 0 },
            { EnumMotor::FR, 0 },
            { EnumMotor::RL, 0 },
            { EnumMotor::RR, 0 }
        };

        Controller::Controller(USART_TypeDef *const usart):
            _usart(usart), _sbdbt(usart), _bs() {
        }

        Controller::~Controller() = default;

        Motor::State Controller::stickToMotor() {
            const LowLayer::SBDBT::AnalogState as = _sbdbt.getAnalogState();
            const float FL = 3.0f / 8.0f * M_PI;
            const float FR = 1.0f / 8.0f * M_PI;
            const float RL = 5.0f / 8.0f * M_PI;
            const float RR = 7.0f / 8.0f * M_PI;
            const StickTheta theta = sticksToTheta(as.LX, as.LY, as.RX, as.RY);
            const float powerFL = std::sin(theta.left + FL) * static_cast<float>(Motor::getMaxSpeed());
            const float powerFR = std::sin(theta.left + FR) * static_cast<float>(Motor::getMaxSpeed());
            const float powerRL = std::sin(theta.left + RL) * static_cast<float>(Motor::getMaxSpeed());
            const float powerRR = std::sin(theta.left + RR) * static_cast<float>(Motor::getMaxSpeed());
            const float powerMax = std::max<float>(std::max<float>(std::max<float>(std::abs(powerFL), std::abs(powerFR)), std::abs(powerRL)), std::abs(powerRR));
            return {
                .FL = static_cast<std::int32_t>(powerFL * powerMax),
                .FR = static_cast<std::int32_t>(powerFR * powerMax),
                .RL = static_cast<std::int32_t>(powerRL * powerMax),
                .RR = static_cast<std::int32_t>(powerRR * powerMax)
            };
        }

        void Controller::init() {
            _sbdbt.init();
        }

        bool Controller::isPush(LowLayer::SBDBT::ButtonState button) {
            return button == LowLayer::SBDBT::ButtonState::kPush;
        }

        bool Controller::isPushEdge(LowLayer::SBDBT::ButtonState button) {
            return button == LowLayer::SBDBT::ButtonState::kPushEdge;
        }

        bool Controller::isRelease(LowLayer::SBDBT::ButtonState button) {
            return button == LowLayer::SBDBT::ButtonState::kRelease;
        }

        bool Controller::isReleaseEdge(LowLayer::SBDBT::ButtonState button) {
            return button == LowLayer::SBDBT::ButtonState::kReleaseEdge;
        }

        void Controller::receiveProcessing(const std::uint8_t (&receiveData)[LowLayer::SBDBT_RECEIVE_SIZE], const std::function<void(const LowLayer::SBDBT::ButtonAssignment &bs)> &callback) {
            if(!_sbdbt.receiveCheck(std::to_array(receiveData))) return;
            _bs = _sbdbt.receiveProcessing();
            callback(_bs);
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
    }// namespace v2
}// namespace LibMecha