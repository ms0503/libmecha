/*
 * IMotorDriver.cc
 *
 *  Created on: 2023/06/25
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

#ifdef USE_FULL_LL_DRIVER

#include <MiddleLayer/IMotorDriver.hh>
#include <utility>

namespace LibMecha::MiddleLayer {
    IMotorDriver::IMotorDriver(const std::int32_t maxSpeed, LowLayer::Encoder encoder):
        _maxSpeed(maxSpeed), _encoder(std::move(encoder)) {
    }

    void IMotorDriver::init() {
    }

    bool IMotorDriver::setTarget(std::int32_t targetSpeed) {
        return false;
    }
}

#endif
