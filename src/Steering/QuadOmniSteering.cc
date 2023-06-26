/*
 * QuadOmniSteering.cc
 *
 *  Created on: 2023/06/23
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

#if (!defined DISABLE_LL && !defined DISABLE_MOTORDRIVER && !defined DISABLE_STEERING && !defined DISABLE_QUADOMNISTEERING)

#include "Controller.hh"
#include "MiddleLayer/IMotorDriver.hh"
#include "Steering/QuadOmniSteering.hh"
#include <array>
#include <cmath>
#include <concepts>
#include <cstdint>

namespace LibMecha::Steering {
    template<class MotorDriver> requires std::derived_from<MotorDriver, LowLayer::IMotorDriver> QuadOmniSteering<MotorDriver>::QuadOmniSteering(const std::array<MotorDriver, 4> md):
        _md(md) {
    }

    template<class MotorDriver> requires std::derived_from<MotorDriver, LowLayer::IMotorDriver> QuadOmniSteering<MotorDriver>::~QuadOmniSteering() = default;

    template<class MotorDriver> requires std::derived_from<MotorDriver, LowLayer::IMotorDriver> void QuadOmniSteering<MotorDriver>::polarInput(const float r, const LibMecha::Controller<QuadOmniSteering<MotorDriver>>::StickTheta theta) const {
        _md.at(0).setTarget(std::sin(theta.left - M_PI_4) * r * _md.at(0).getMaxSpeed());
        _md.at(1).setTarget(-std::sin(theta.left + M_PI_4) * r * _md.at(1).getMaxSpeed());
        _md.at(2).setTarget(-std::sin(theta.left - M_PI_4) * r * _md.at(2).getMaxSpeed());
        _md.at(3).setTarget(std::sin(theta.left + M_PI_4) * r * _md.at(3).getMaxSpeed());
    }

    template<class MotorDriver> requires std::derived_from<MotorDriver, LowLayer::IMotorDriver> void QuadOmniSteering<MotorDriver>::forward(const std::int32_t speed) const {
        _md.at(0).setTarget(std::sin(M_PI_4) * std::max(std::abs(speed), _md.at(0).getMaxSpeed()));
        _md.at(1).setTarget(-std::sin(M_PI_4) * std::max(std::abs(speed), _md.at(1).getMaxSpeed()));
        _md.at(2).setTarget(-std::sin(M_PI_4) * std::max(std::abs(speed), _md.at(2).getMaxSpeed()));
        _md.at(3).setTarget(std::sin(M_PI_4) * std::max(std::abs(speed), _md.at(3).getMaxSpeed()));
    }

    template<class MotorDriver> requires std::derived_from<MotorDriver, LowLayer::IMotorDriver> void QuadOmniSteering<MotorDriver>::backward(const std::int32_t speed) const {
        _md.at(0).setTarget(-std::sin(M_PI_4) * std::max(std::abs(speed), _md.at(0).getMaxSpeed()));
        _md.at(1).setTarget(std::sin(M_PI_4) * std::max(std::abs(speed), _md.at(1).getMaxSpeed()));
        _md.at(2).setTarget(std::sin(M_PI_4) * std::max(std::abs(speed), _md.at(2).getMaxSpeed()));
        _md.at(3).setTarget(-std::sin(M_PI_4) * std::max(std::abs(speed), _md.at(3).getMaxSpeed()));
    }

    template<class MotorDriver> requires std::derived_from<MotorDriver, LowLayer::IMotorDriver> void QuadOmniSteering<MotorDriver>::left(const std::int32_t speed) const {
        _md.at(0).setTarget(std::sin(M_PI_4) * std::max(std::abs(speed), _md.at(0).getMaxSpeed()));
        _md.at(1).setTarget(std::sin(M_PI_4) * std::max(std::abs(speed), _md.at(1).getMaxSpeed()));
        _md.at(2).setTarget(-std::sin(M_PI_4) * std::max(std::abs(speed), _md.at(2).getMaxSpeed()));
        _md.at(3).setTarget(-std::sin(M_PI_4) * std::max(std::abs(speed), _md.at(3).getMaxSpeed()));
    }

    template<class MotorDriver> requires std::derived_from<MotorDriver, LowLayer::IMotorDriver> void QuadOmniSteering<MotorDriver>::right(const std::int32_t speed) const {
        _md.at(0).setTarget(-std::sin(M_PI_4) * std::max(std::abs(speed), _md.at(0).getMaxSpeed()));
        _md.at(1).setTarget(-std::sin(M_PI_4) * std::max(std::abs(speed), _md.at(1).getMaxSpeed()));
        _md.at(2).setTarget(std::sin(M_PI_4) * std::max(std::abs(speed), _md.at(2).getMaxSpeed()));
        _md.at(3).setTarget(std::sin(M_PI_4) * std::max(std::abs(speed), _md.at(3).getMaxSpeed()));
    }

    template<class MotorDriver> requires std::derived_from<MotorDriver, LowLayer::IMotorDriver> void QuadOmniSteering<MotorDriver>::turnLeft(const std::int32_t speed) const {
        _md.at(0).setTarget(std::max(std::abs(speed), _md.at(0).getMaxSpeed()));
        _md.at(1).setTarget(std::max(std::abs(speed), _md.at(1).getMaxSpeed()));
        _md.at(2).setTarget(std::max(std::abs(speed), _md.at(2).getMaxSpeed()));
        _md.at(3).setTarget(std::max(std::abs(speed), _md.at(3).getMaxSpeed()));
    }

    template<class MotorDriver> requires std::derived_from<MotorDriver, LowLayer::IMotorDriver> void QuadOmniSteering<MotorDriver>::turnRight(const std::int32_t speed) const {
        _md.at(0).setTarget(-std::max(std::abs(speed), _md.at(0).getMaxSpeed()));
        _md.at(1).setTarget(-std::max(std::abs(speed), _md.at(1).getMaxSpeed()));
        _md.at(2).setTarget(-std::max(std::abs(speed), _md.at(2).getMaxSpeed()));
        _md.at(3).setTarget(-std::max(std::abs(speed), _md.at(3).getMaxSpeed()));
    }
}

#endif
