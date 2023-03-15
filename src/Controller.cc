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
        namespace LMLL = LowLayer;

        const std::map<EnumMotor, int8_t> Controller::DEAD_ZONES = {
            {EnumMotor::FL,
             0},
            {EnumMotor::FR,
             0},
            {EnumMotor::RL,
             0},
            {EnumMotor::RR,
             0}};

        Controller::Controller(USART_TypeDef *const usart) : _usart(usart), _sbdbt(usart), _bs() {
        }

        Controller::~Controller() = default;

        Motor::State Controller::stickToMotor() {
            const LMLL::SBDBT::AnalogState as = _sbdbt.getAnalogState();
            const long double FL = 3. / 8 * M_PI;
            const long double FR = 1. / 8 * M_PI;
            const long double RL = 5. / 8 * M_PI;
            const long double RR = 7. / 8 * M_PI;
            const StickTheta theta = sticksToTheta(as.LX, as.LY, as.RX, as.RY);
            const long double powerFL = sinl(theta.left + FL) * Motor::getMaxSpeed();
            const long double powerFR = sinl(theta.left + FR) * Motor::getMaxSpeed();
            const long double powerRL = sinl(theta.left + RL) * Motor::getMaxSpeed();
            const long double powerRR = sinl(theta.left + RR) * Motor::getMaxSpeed();
            const long double powerMax = fmaxl(fmaxl(fmaxl(fabsl(powerFL), fabsl(powerFR)), fabsl(powerRL)), fabsl(powerRR));
            return {
                .FL = static_cast<int32_t>(powerFL * powerMax),
                .FR = static_cast<int32_t>(powerFR * powerMax),
                .RL = static_cast<int32_t>(powerRL * powerMax),
                .RR = static_cast<int32_t>(powerRR * powerMax)};
        }

        void Controller::init() {
            _sbdbt.init();
        }

        bool Controller::isPush(LMLL::SBDBT::ButtonState button) {
            return button == LMLL::SBDBT::ButtonState::kPush;
        }

        bool Controller::isPushEdge(LMLL::SBDBT::ButtonState button) {
            return button == LMLL::SBDBT::ButtonState::kPushEdge;
        }

        bool Controller::isRelease(LMLL::SBDBT::ButtonState button) {
            return button == LMLL::SBDBT::ButtonState::kRelease;
        }

        bool Controller::isReleaseEdge(LMLL::SBDBT::ButtonState button) {
            return button == LMLL::SBDBT::ButtonState::kReleaseEdge;
        }

        void Controller::receiveProcessing(const std::uint8_t (&receiveData)[LMLL::SBDBT_RECEIVE_SIZE], const std::function<void(const LMLL::SBDBT::ButtonAssignment &bs)> &callback) {
            if(!_sbdbt.receiveCheck(std::to_array(receiveData))) return;
            _bs = _sbdbt.receiveProcessing();
            callback(_bs);
        }

        long double Controller::stickToTheta(const int8_t x, const int8_t y) {
            return atanl(static_cast<float>(y) / static_cast<float>(x));
        }

        Controller::StickTheta Controller::sticksToTheta(const int8_t leftX, const int8_t leftY, const int8_t rightX, const int8_t rightY) {
            return {
                .left = stickToTheta(leftX, leftY),
                .right = stickToTheta(rightX, rightY)};
        }
    }// namespace v2
}// namespace LibMecha