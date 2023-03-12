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

using namespace LibMecha::v1::LowLayer;

MotorDriver::MotorDriver(const CAN_HandleTypeDef &canHandle, const Can &can) : _hcan(canHandle), _can(can) {
}

MotorDriver::~MotorDriver() = default;

void MotorDriver::init() {
    _can.setCANHandle(_hcan);
}

void MotorDriver::pidInit(uint8_t address, float kp, float ki, float kd, uint32_t maxRpm, uint32_t kppm) {
    while(!setParameter(address, DriveCommand::kSetParamP, kp));
    HAL_Delay(20);
    while(!setParameter(address, DriveCommand::kSetParamI, ki));
    HAL_Delay(20);
    while(!setParameter(address, DriveCommand::kSetParamD, kd));
    HAL_Delay(20);
    while(!setParameter(address, DriveCommand::kSetParamLIMIT, maxRpm));
    HAL_Delay(20);
    while(!setParameter(address, DriveCommand::kSetParamPPM, kppm));
    HAL_Delay(20);
}

Can &MotorDriver::getCan() {
    return _can;
}

CAN_HandleTypeDef &MotorDriver::getCanHandle() {
    return _hcan;
}

bool MotorDriver::updateDataSend(const uint8_t address, const DriveCommand cmd, const uint8_t (&sendData)[4]) {
    uint8_t sendDataArray[4 + 1] = {0};
    sendDataArray[0] = static_cast<uint8_t>(cmd);

    for(uint8_t i = 0; i < 4; i++) sendDataArray[i + 1] = sendData[i];

    return _can.send(address, sendDataArray);
}

bool MotorDriver::Emergency(const uint8_t address) {
    uint8_t sendDataArray[4] = {0};

    return updateDataSend(address, DriveCommand::kEmergency, sendDataArray);
    /*
    sendDataArray[0] = static_cast<uint8_t>(DriveCommand::emergency);
    for(uint8_t i = 0; i < 4; i++) sendDataArray[i + 1] = 0x00;

    return _can.send(address, sendDataArray);
     */
}

bool MotorDriver::setParameter(const uint8_t address, const DriveCommand mode, const float fparamValue) {
    if((mode == DriveCommand::kPID) || (mode == DriveCommand::kDuty) || (mode == DriveCommand::kEmergency)) return false;

    uint8_t sendDataArray[4] = {0};
    int32_t escape = 0;
//    sendDataArray[0] = static_cast<uint8_t>(mode);
    std::memcpy(&escape, &fparamValue, 4);
//    std::memcpy(&sendDataArray[1], &fparam_value, 4);
    sendDataArray[0] = static_cast<uint8_t>(escape >> 24);
    sendDataArray[1] = static_cast<uint8_t>(escape >> 16);
    sendDataArray[2] = static_cast<uint8_t>(escape >> 8);
    sendDataArray[3] = static_cast<uint8_t>(escape);

    return updateDataSend(address, mode, sendDataArray);
    /*
    for(uint8_t i = 0; i < 4; i++) sendDataArray[i + 1] = (uint8_t)(fparamValue >> (32 - 8 * (i + 1)));

    return _can.send(address, sendDataArray);
     */
}

bool MotorDriver::setParameter(const uint8_t address, const DriveCommand mode, const uint32_t uparamValue) {
    if((mode == DriveCommand::kPID) || (mode == DriveCommand::kDuty) || (mode == DriveCommand::kEmergency)) return false;

    uint8_t sendDataArray[4] = {0};
    int32_t escape = 0;
//    sendDataArray[0] = static_cast<uint8_t>(mode);
    std::memcpy(&escape, &uparamValue, 4);
//    std::memcpy(&sendDataArray[1], &uparamValue, 4);
    sendDataArray[0] = static_cast<uint8_t>(escape >> 24);
    sendDataArray[1] = static_cast<uint8_t>(escape >> 16);
    sendDataArray[2] = static_cast<uint8_t>(escape >> 8);
    sendDataArray[3] = static_cast<uint8_t>(escape);

    return updateDataSend(address, mode, sendDataArray);
    /*
    for(uint8_t i = 0;i < 4;i++) sendDataArray[i + 1] = (uint8_t)(uparamValue >> (32 - 8*(i + 1)));

    return _can.send(address, sendDataArray);
     */
}

bool MotorDriver::setTargetRPM(const uint8_t address, const int32_t targetRpm) {
    uint8_t sendDataArray[4] = {0};
    sendDataArray[0] = static_cast<uint8_t>(targetRpm >> 24);
    sendDataArray[1] = static_cast<uint8_t>(targetRpm >> 16);
    sendDataArray[2] = static_cast<uint8_t>(targetRpm >> 8);
    sendDataArray[3] = static_cast<uint8_t>(targetRpm);

    return updateDataSend(address, DriveCommand::kPID, sendDataArray);
}

bool MotorDriver::setDuty(const uint8_t address, const int32_t duty) {
    uint8_t sendDataArray[4] = {0};
//    int32_t escape = 0;
//    std::memcpy(sendDataArray, &duty, 4);
    sendDataArray[0] = static_cast<uint8_t>(duty >> 24);
    sendDataArray[1] = static_cast<uint8_t>(duty >> 16);
    sendDataArray[2] = static_cast<uint8_t>(duty >> 8);
    sendDataArray[3] = static_cast<uint8_t>(duty);

    return updateDataSend(address, DriveCommand::kDuty, sendDataArray);
}
