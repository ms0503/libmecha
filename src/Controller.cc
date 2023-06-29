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

#if (!defined DISABLE_USART && !defined DISABLE_SBDBT && !defined DISABLE_STEERING && !defined DISABLE_CONTROLLER)

#include <Controller.hh>
#include <utility>

namespace LibMecha {
    Controller::Controller(MiddleLayer::SBDBT sbdbt, const Steering::ISteering &steering):
        _sbdbt(std::move(sbdbt)), _bs(), _deadZones(), _steering(steering) {
    }

    Controller::~Controller() = default;

    void Controller::stickToSteering() {
        MiddleLayer::SBDBT::AnalogState as = getStick();
        const float r = std::hypot(as.LX, as.LY) / STICK_MAX; // 動径
        const StickTheta theta = sticksToTheta(); // 偏角・スティックの角度(右0、反時計回りが正、-π < x <= π)
        _steering.polarInput(r, theta);
    }
} // namespace LibMecha

#endif // (!defined DISABLE_HAL && !defined DISABLE_LL && !defined DISABLE_PERIPHERAL && !defined DISABLE_CAN && !defined DISABLE_CANMOTOR && !defined DISABLE_USART && !defined DISABLE_SBDBT && !defined DISABLE_CONTROLLER)
