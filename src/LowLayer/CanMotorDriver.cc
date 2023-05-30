/*
 * CanMotorDriver.cc
 *
 *  Created on: 2019/06/29
 *      Author: youda, ms0503
 *
 *  This file is part of libmecha.
 *
 *  libmecha is free software: you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 *
 *  libmecha is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License along with libmecha. If not, see <https://www.gnu.org/licenses/>.
 */

#include "LowLayer/CanMotorDriver.hh"
#include <algorithm>

namespace LibMecha::LowLayer {
    CanMotorDriver::CanMotorDriver(Can &can, std::uint8_t address):
        _can(can), _address(address) {
    }

    void CanMotorDriver::init() {
    }

    void CanMotorDriver::pidInit(const float kp, const float ki, const float kd, const std::uint32_t maxRpm, const std::uint32_t kppm) const {
        while(!setParameter(DriveCommand::kSetParamP, kp))
            ;
        HAL_Delay(20);
        while(!setParameter(DriveCommand::kSetParamI, ki))
            ;
        HAL_Delay(20);
        while(!setParameter(DriveCommand::kSetParamD, kd))
            ;
        HAL_Delay(20);
        while(!setParameter(DriveCommand::kSetParamLIMIT, maxRpm))
            ;
        HAL_Delay(20);
        while(!setParameter(DriveCommand::kSetParamPPM, kppm))
            ;
        HAL_Delay(20);
    }

    bool CanMotorDriver::setTargetRPM(std::int32_t targetRpm) const {
        auto &sendDataArray = reinterpret_cast<std::uint8_t (&)[4]>(targetRpm);
        std::reverse(sendDataArray, sendDataArray + 4);

        return updateDataSend(DriveCommand::kPID, sendDataArray);
    }

    bool CanMotorDriver::setDuty(std::int32_t duty) {
        auto &sendDataArray = reinterpret_cast<std::uint8_t (&)[4]>(duty);
        std::reverse(sendDataArray, sendDataArray + 4);

        return updateDataSend(DriveCommand::kDuty, sendDataArray);
    }

    bool CanMotorDriver::setParameter(const DriveCommand mode, std::uint32_t uparamValue) const {
        if((mode == DriveCommand::kPID) || (mode == DriveCommand::kDuty) || (mode == DriveCommand::kEmergency)) return false;
        auto &sendDataArray = reinterpret_cast<std::uint8_t (&)[4]>(uparamValue);
        std::reverse(sendDataArray, sendDataArray + 4);

        return updateDataSend(mode, sendDataArray);
    }

    bool CanMotorDriver::emergency() const {
        const std::uint8_t (&sendDataArray)[4] {};

        return updateDataSend(DriveCommand::kEmergency, sendDataArray);
    }

    bool CanMotorDriver::updateDataSend(const DriveCommand cmd, const std::uint8_t (&sendData)[4]) const {
        const std::uint8_t (&sendDataArray)[4 + 1] {
            static_cast<std::uint8_t>(cmd),
            sendData[0],
            sendData[1],
            sendData[2],
            sendData[3]
        };

        return _can.send(_address, sendDataArray);
    }
} // namespace LibMecha::LowLayer
