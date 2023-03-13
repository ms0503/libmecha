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

using namespace LibMecha::v1;

const std::map<EnumMotor, int8_t> Controller::DEAD_ZONES = {
    {
        EnumMotor::FL,
        0
    },
    {
        EnumMotor::FR,
        0
    },
    {
        EnumMotor::RL,
        0
    },
    {
        EnumMotor::RR,
        0
    }
};

Controller::Controller(const UART_HandleTypeDef &uartHandle) : _uartHandle(uartHandle), _sbdbt(uartHandle), _bs(), _sbdbtData() {
}

Controller::~Controller() = default;

MotorState Controller::stickToMotor() {
    const SBDBT::AnalogState as = _sbdbt.getAnalogState();
    const long double FL = 3. / 8 * M_PI;
    const long double FR = 1. / 8 * M_PI;
    const long double RL = 5. / 8 * M_PI;
    const long double RR = 7. / 8 * M_PI;
    const StickTheta theta = sticksToTheta(as.LX, as.LY, as.RX, as.RY);
    const long double powerFL = sinl(theta.left + FL) * MOTOR_SPEED_MAX;
    const long double powerFR = sinl(theta.left + FR) * MOTOR_SPEED_MAX;
    const long double powerRL = sinl(theta.left + RL) * MOTOR_SPEED_MAX;
    const long double powerRR = sinl(theta.left + RR) * MOTOR_SPEED_MAX;
    const long double powerMax = fmaxl(fmaxl(fmaxl(fabsl(powerFL), fabsl(powerFR)), fabsl(powerRL)), fabsl(powerRR));
    return {
        .FL = static_cast<int32_t>(powerFL * powerMax),
        .FR = static_cast<int32_t>(powerFR * powerMax),
        .RL = static_cast<int32_t>(powerRL * powerMax),
        .RR = static_cast<int32_t>(powerRR * powerMax)
    };
}

void Controller::init() {
    _sbdbt.init();
}

bool Controller::isPush(SBDBT::ButtonState button) {
    return button == SBDBT::ButtonState::kPush;
}

bool Controller::isPushEdge(SBDBT::ButtonState button) {
    return button == SBDBT::ButtonState::kPushEdge;
}

bool Controller::isRelease(SBDBT::ButtonState button) {
    return button == SBDBT::ButtonState::kRelease;
}

bool Controller::isReleaseEdge(SBDBT::ButtonState button) {
    return button == SBDBT::ButtonState::kReleaseEdge;
}

void Controller::receiveProcessing(const UART_HandleTypeDef *uartHandle, const std::function<void(const SBDBT::ButtonAssignment &bs)> &callback) {
    if(&_uartHandle != uartHandle) return;
    if(!_sbdbt.receiveCheck(_sbdbtData)) return;
    _bs = _sbdbt.receiveProcessing();
    callback(_bs);
}

long double Controller::stickToTheta(const int8_t x, const int8_t y) {
    return atanl(static_cast<float>(y) / static_cast<float>(x));
}

StickTheta Controller::sticksToTheta(const int8_t leftX, const int8_t leftY, const int8_t rightX, const int8_t rightY) {
    return {
        .left = stickToTheta(leftX, leftY),
        .right = stickToTheta(rightX, rightY)
    };
}
