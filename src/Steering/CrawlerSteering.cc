/*
 * CrawlerSteering.cc
 *
 *  Created on: 2023/5/27
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
#include "Steering/CrawlerSteering.hh"
#include <array>
#include <cmath>
#include <cstdint>
#include <utility>

namespace LibMecha::Steering {
    CrawlerSteering::CrawlerSteering(std::array<MiddleLayer::IMotorDriver, 2> md):
        _md(std::move(md)) {
    }

    CrawlerSteering::~CrawlerSteering() = default;

    void CrawlerSteering::polarInput(const float r, const LibMecha::Controller::StickTheta theta) {
        _md.at(0).setTarget(std::sin(theta.left - M_PI_4) * r * _md.at(0).getMaxSpeed());
        _md.at(1).setTarget(std::sin(theta.left + M_PI_4) * r * _md.at(1).getMaxSpeed());
    }

    void CrawlerSteering::forward(const std::int32_t speed) {
        _md.at(0).setTarget(M_PI_4 * std::max(std::abs(speed), _md.at(0).getMaxSpeed()));
        _md.at(1).setTarget(M_PI_4 * std::max(std::abs(speed), _md.at(1).getMaxSpeed()));
    }

    void CrawlerSteering::backward(const std::int32_t speed) {
        _md.at(0).setTarget(-M_PI_4 * std::max(std::abs(speed), _md.at(0).getMaxSpeed()));
        _md.at(1).setTarget(-M_PI_4 * std::max(std::abs(speed), _md.at(1).getMaxSpeed()));
    }

    void CrawlerSteering::turnLeft(const std::int32_t speed) {
        _md.at(0).setTarget(M_PI_4 * std::max(std::abs(speed), _md.at(0).getMaxSpeed()));
        _md.at(1).setTarget(-M_PI_4 * std::max(std::abs(speed), _md.at(1).getMaxSpeed()));
    }

    void CrawlerSteering::turnRight(const std::int32_t speed) {
        _md.at(0).setTarget(-M_PI_4 * std::max(std::abs(speed), _md.at(0).getMaxSpeed()));
        _md.at(1).setTarget(M_PI_4 * std::max(std::abs(speed), _md.at(1).getMaxSpeed()));
    }
}
