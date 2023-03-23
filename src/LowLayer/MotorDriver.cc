/*
 * MotorDriver.cc
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

#include "LowLayer/MotorDriver.hh"

namespace LibMecha::LowLayer {
    MotorDriver::MotorDriver(Can &can, std::uint8_t address):
        _can(can), _address(address) {
    }

    void MotorDriver::init() const {
    }

    void MotorDriver::pidInit(const float kp, const float ki, const float kd, const std::uint32_t maxRpm, const std::uint32_t kppm) const {
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

    bool MotorDriver::setTargetRPM(const std::int32_t targetRpm) const {
        const std::uint8_t sendDataArray[4] {
            static_cast<std::uint8_t>(targetRpm >> 24),
            static_cast<std::uint8_t>(targetRpm >> 16),
            static_cast<std::uint8_t>(targetRpm >> 8),
            static_cast<std::uint8_t>(targetRpm)
        };

        return updateDataSend(DriveCommand::kPID, sendDataArray);
    }

    bool MotorDriver::setDuty(const std::int32_t duty) const {
        const std::uint8_t sendDataArray[4] {
            static_cast<std::uint8_t>(duty >> 24),
            static_cast<std::uint8_t>(duty >> 16),
            static_cast<std::uint8_t>(duty >> 8),
            static_cast<std::uint8_t>(duty)
        };

        return updateDataSend(DriveCommand::kDuty, sendDataArray);
    }

    bool MotorDriver::setParameter(const DriveCommand mode, const std::uint32_t uparamValue) const {
        if((mode == DriveCommand::kPID) || (mode == DriveCommand::kDuty) || (mode == DriveCommand::kEmergency)) return false;
        const std::uint8_t sendDataArray[4] {
            static_cast<std::uint8_t>(uparamValue >> 24),
            static_cast<std::uint8_t>(uparamValue >> 16),
            static_cast<std::uint8_t>(uparamValue >> 8),
            static_cast<std::uint8_t>(uparamValue)
        };

        return updateDataSend(mode, sendDataArray);
    }

    bool MotorDriver::emergency() const {
        const std::uint8_t sendDataArray[4] {};

        return updateDataSend(DriveCommand::kEmergency, sendDataArray);
    }

    bool MotorDriver::updateDataSend(const DriveCommand cmd, const std::uint8_t sendData[4]) const {
        std::array<std::uint8_t, 4 + 1> sendDataArray {
            static_cast<std::uint8_t>(cmd),
            sendData[0],
            sendData[1],
            sendData[2],
            sendData[3]
        };

        return _can.send(_address, sendDataArray.data(), sendDataArray.size());
    }
} // namespace LibMecha::LowLayer
