/*
* Motor.cc
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

#include "Motor.hh"
#include "params.hh"

using namespace LibMecha::v1;

Motor::Motor(const CAN_HandleTypeDef &canHandle) : _hcan(canHandle), _can(canHandle), _md(canHandle, _can) {
}

Motor::~Motor() = default;

void Motor::init(const uint8_t canAddr) {
    _can.setUp(canAddr, CAN_IT_RX_FIFO1_MSG_PENDING);
    HAL_CAN_ActivateNotification(&_hcan, CAN_IT_RX_FIFO0_MSG_PENDING);
    _md.init();
}

void Motor::update(const MotorState state) {
    _md.setDuty(ADDR_MOTOR_FL, state.FL);
    _md.setDuty(ADDR_MOTOR_FR, state.FR);
    _md.setDuty(ADDR_MOTOR_RL, state.RL);
    _md.setDuty(ADDR_MOTOR_RR, state.RR);
}
