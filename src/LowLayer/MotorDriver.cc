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

namespace LibMecha {
    inline namespace v2 {
        namespace LowLayer {
            MotorDriver::MotorDriver(const CAN_HandleTypeDef &canHandle, const Can &can):
                _hcan(canHandle), _can(can) {
            }

            MotorDriver::~MotorDriver() = default;

            void MotorDriver::init() {
            }

            void MotorDriver::pidInit(const std::uint8_t address, const float kp, const float ki, const float kd, const std::uint32_t maxRpm, const std::uint32_t kppm) {
                while(!setParameter(address, DriveCommand::kSetParamP, kp))
                    ;
                HAL_Delay(20);
                while(!setParameter(address, DriveCommand::kSetParamI, ki))
                    ;
                HAL_Delay(20);
                while(!setParameter(address, DriveCommand::kSetParamD, kd))
                    ;
                HAL_Delay(20);
                while(!setParameter(address, DriveCommand::kSetParamLIMIT, maxRpm))
                    ;
                HAL_Delay(20);
                while(!setParameter(address, DriveCommand::kSetParamPPM, kppm))
                    ;
                HAL_Delay(20);
            }

            bool MotorDriver::setTargetRPM(const std::uint8_t address, const std::int32_t targetRpm) {
                const std::uint8_t sendDataArray[4] {
                    static_cast<std::uint8_t>(targetRpm >> 24),
                    static_cast<std::uint8_t>(targetRpm >> 16),
                    static_cast<std::uint8_t>(targetRpm >> 8),
                    static_cast<std::uint8_t>(targetRpm)
                };

                return updateDataSend(address, DriveCommand::kPID, sendDataArray);
            }

            bool MotorDriver::setDuty(const std::uint8_t address, const std::int32_t duty) {
                const std::uint8_t sendDataArray[4] {
                    static_cast<std::uint8_t>(duty >> 24),
                    static_cast<std::uint8_t>(duty >> 16),
                    static_cast<std::uint8_t>(duty >> 8),
                    static_cast<std::uint8_t>(duty)
                };

                return updateDataSend(address, DriveCommand::kDuty, sendDataArray);
            }

            bool MotorDriver::setParameter(const std::uint8_t address, const DriveCommand mode, const std::uint32_t uparamValue) {
                if((mode == DriveCommand::kPID) || (mode == DriveCommand::kDuty) || (mode == DriveCommand::kEmergency)) return false;
                const std::uint8_t sendDataArray[4] {
                    static_cast<std::uint8_t>(uparamValue >> 24),
                    static_cast<std::uint8_t>(uparamValue >> 16),
                    static_cast<std::uint8_t>(uparamValue >> 8),
                    static_cast<std::uint8_t>(uparamValue)
                };

                return updateDataSend(address, mode, sendDataArray);
            }

            bool MotorDriver::emergency(const std::uint8_t address) {
                const std::uint8_t sendDataArray[4] {};

                return updateDataSend(address, DriveCommand::kEmergency, sendDataArray);
            }

            bool MotorDriver::updateDataSend(const std::uint8_t address, const DriveCommand cmd, const std::uint8_t sendData[4]) {
                std::array<std::uint8_t, 4 + 1> sendDataArray {
                    static_cast<std::uint8_t>(cmd),
                    sendData[0],
                    sendData[1],
                    sendData[2],
                    sendData[3]
                };

                return _can.send(address, sendDataArray.data(), 4 + 1);
            }
        }// namespace LowLayer
    }// namespace v2
}// namespace LibMecha
