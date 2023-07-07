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

#include <Steering/QuadOmniSteering.hh>
#include <cmath>
#include <numbers>
#include <utility>

namespace LibMecha::Steering {
    QuadOmniSteering::QuadOmniSteering(std::array<MiddleLayer::IMotorDriver, 4> md):
        _md(std::move(md)) {
    }

    QuadOmniSteering::~QuadOmniSteering() = default;

    void QuadOmniSteering::polarInput(const float r, const LibMecha::StickTheta theta) {
        _md.at(0).setTarget(std::sin(theta.left - std::numbers::pi_v<float> / 4.0f) * r * _md.at(0).getMaxSpeed());
        _md.at(1).setTarget(-std::sin(theta.left + std::numbers::pi_v<float> / 4.0f) * r * _md.at(1).getMaxSpeed());
        _md.at(2).setTarget(-std::sin(theta.left - std::numbers::pi_v<float> / 4.0f) * r * _md.at(2).getMaxSpeed());
        _md.at(3).setTarget(std::sin(theta.left + std::numbers::pi_v<float> / 4.0f) * r * _md.at(3).getMaxSpeed());
    }

    void QuadOmniSteering::forward(const std::int32_t speed) {
        _md.at(0).setTarget(std::sin(std::numbers::pi_v<float> / 4.0f) * std::max(std::abs(speed), _md.at(0).getMaxSpeed()));
        _md.at(1).setTarget(-std::sin(std::numbers::pi_v<float> / 4.0f) * std::max(std::abs(speed), _md.at(1).getMaxSpeed()));
        _md.at(2).setTarget(-std::sin(std::numbers::pi_v<float> / 4.0f) * std::max(std::abs(speed), _md.at(2).getMaxSpeed()));
        _md.at(3).setTarget(std::sin(std::numbers::pi_v<float> / 4.0f) * std::max(std::abs(speed), _md.at(3).getMaxSpeed()));
    }

    void QuadOmniSteering::backward(const std::int32_t speed) {
        _md.at(0).setTarget(-std::sin(std::numbers::pi_v<float> / 4.0f) * std::max(std::abs(speed), _md.at(0).getMaxSpeed()));
        _md.at(1).setTarget(std::sin(std::numbers::pi_v<float> / 4.0f) * std::max(std::abs(speed), _md.at(1).getMaxSpeed()));
        _md.at(2).setTarget(std::sin(std::numbers::pi_v<float> / 4.0f) * std::max(std::abs(speed), _md.at(2).getMaxSpeed()));
        _md.at(3).setTarget(-std::sin(std::numbers::pi_v<float> / 4.0f) * std::max(std::abs(speed), _md.at(3).getMaxSpeed()));
    }

    void QuadOmniSteering::left(const std::int32_t speed) {
        _md.at(0).setTarget(std::sin(std::numbers::pi_v<float> / 4.0f) * std::max(std::abs(speed), _md.at(0).getMaxSpeed()));
        _md.at(1).setTarget(std::sin(std::numbers::pi_v<float> / 4.0f) * std::max(std::abs(speed), _md.at(1).getMaxSpeed()));
        _md.at(2).setTarget(-std::sin(std::numbers::pi_v<float> / 4.0f) * std::max(std::abs(speed), _md.at(2).getMaxSpeed()));
        _md.at(3).setTarget(-std::sin(std::numbers::pi_v<float> / 4.0f) * std::max(std::abs(speed), _md.at(3).getMaxSpeed()));
    }

    void QuadOmniSteering::right(const std::int32_t speed) {
        _md.at(0).setTarget(-std::sin(std::numbers::pi_v<float> / 4.0f) * std::max(std::abs(speed), _md.at(0).getMaxSpeed()));
        _md.at(1).setTarget(-std::sin(std::numbers::pi_v<float> / 4.0f) * std::max(std::abs(speed), _md.at(1).getMaxSpeed()));
        _md.at(2).setTarget(std::sin(std::numbers::pi_v<float> / 4.0f) * std::max(std::abs(speed), _md.at(2).getMaxSpeed()));
        _md.at(3).setTarget(std::sin(std::numbers::pi_v<float> / 4.0f) * std::max(std::abs(speed), _md.at(3).getMaxSpeed()));
    }

    void QuadOmniSteering::turnLeft(const std::int32_t speed) {
        _md.at(0).setTarget(std::max(std::abs(speed), _md.at(0).getMaxSpeed()));
        _md.at(1).setTarget(std::max(std::abs(speed), _md.at(1).getMaxSpeed()));
        _md.at(2).setTarget(std::max(std::abs(speed), _md.at(2).getMaxSpeed()));
        _md.at(3).setTarget(std::max(std::abs(speed), _md.at(3).getMaxSpeed()));
    }

    void QuadOmniSteering::turnRight(const std::int32_t speed) {
        _md.at(0).setTarget(-std::max(std::abs(speed), _md.at(0).getMaxSpeed()));
        _md.at(1).setTarget(-std::max(std::abs(speed), _md.at(1).getMaxSpeed()));
        _md.at(2).setTarget(-std::max(std::abs(speed), _md.at(2).getMaxSpeed()));
        _md.at(3).setTarget(-std::max(std::abs(speed), _md.at(3).getMaxSpeed()));
    }
}
