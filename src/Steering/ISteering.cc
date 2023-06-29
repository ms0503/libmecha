/*
 * ISteering.cc
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

#if (!defined DISABLE_MOTORDRIVER && !defined DISABLE_STEERING)

#include <Steering/ISteering.hh>
#include <cstdint>

namespace LibMecha::Steering {
    std::int32_t maxSpeed;

    ISteering::ISteering() = default;

    ISteering::~ISteering() = default;

    void ISteering::polarInput(const float r, const LibMecha::StickTheta theta) {
    }

    void ISteering::forward(const std::int32_t speed) {
    }

    void ISteering::backward(const std::int32_t speed) {
    }

    void ISteering::left(const std::int32_t speed) {
    }

    void ISteering::right(const std::int32_t speed) {
    }

    void ISteering::turnLeft(const std::int32_t speed) {
    }

    void ISteering::turnRight(const std::int32_t speed) {
    }
}

#endif
