/*
 * MotorDriver.cpp
 *
 *  Created on: 2019/06/29
 *      Author: youda
 *
 *  This file is part of libmecha.
 *
 *  libmecha is free software: you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 *
 *  libmecha is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License along with libmecha. If not, see <https://www.gnu.org/licenses/>.
 */

#include "MotorDriver.hpp"


MotorDriver::MotorDriver(const CAN_HandleTypeDef &can_handle, const Can &can) : hcan_(can_handle), can_(can) {
}

MotorDriver::~MotorDriver() = default;

void MotorDriver::Init() {
    can_.setCANHandle(hcan_);
}

void MotorDriver::PIDInit(const uint8_t address, const float kp, const float ki, const float kd, const uint32_t max_rpm, const uint32_t kppm) {
    while(!setParameter(address, drive_command::kSetParamP, kp));
    HAL_Delay(20);
    while(!setParameter(address, drive_command::kSetParamI, ki));
    HAL_Delay(20);
    while(!setParameter(address, drive_command::kSetParamD, kd));
    HAL_Delay(20);
    while(!setParameter(address, drive_command::kSetParamLIMIT, max_rpm));
    HAL_Delay(20);
    while(!setParameter(address, drive_command::kSetParamPPM, kppm));
    HAL_Delay(20);
}

bool MotorDriver::updateDataSend(const uint8_t address, const drive_command cmd, const uint8_t (&send_data)[4]){
    uint8_t send_data_array[4 + 1] = {0};
    send_data_array[0] = static_cast<uint8_t>(cmd);

    for(uint8_t i = 0; i < 4; i++) {
        send_data_array[i + 1] = send_data[i];
    }

    return can_.send(address, send_data_array);
}

bool MotorDriver::Emergency(const uint8_t address) {
    uint8_t send_data_array[4] = {0};

    return updateDataSend(address, drive_command::kEmergency, send_data_array);
    /*
    send_data_array[0] = static_cast<uint8_t>(drive_command::emergency);
    for(uint8_t i = 0; i < 4; i++){
        send_data_array[i + 1] = 0x00;
    }

    return can_.send(address, send_data_array);
     */
}

bool MotorDriver::setParameter(const uint8_t address, const drive_command mode, const float fparam_value){
    if((mode == drive_command::kPID) || (mode == drive_command::kDuty) || (mode == drive_command::kEmergency)) {
        return false;
    }

    uint8_t send_data_array[4] = {0};
    int32_t escape = 0;
//    send_data_array[0] = static_cast<uint8_t>(mode);
    std::memcpy(&escape, &fparam_value, 4);
//    std::memcpy(&send_data_array[1], &fparam_value, 4);
    send_data_array[0] = static_cast<uint8_t>(escape >> 24);
    send_data_array[1] = static_cast<uint8_t>(escape >> 16);
    send_data_array[2] = static_cast<uint8_t>(escape >> 8);
    send_data_array[3] = static_cast<uint8_t>(escape);

    return updateDataSend(address, mode, send_data_array);
    /*
    for(uint8_t i = 0;i < 4;i++){
        send_data_array[i + 1] = (uint8_t)(fparam_value >> (32 - 8*(i + 1)));
    }

    return can_.send(address, send_data_array);
     */
}

bool MotorDriver::setParameter(const uint8_t address, const drive_command mode, const uint32_t uparam_value){
    if((mode == drive_command::kPID) || (mode == drive_command::kDuty) || (mode == drive_command::kEmergency)){
        return false;
    }

    uint8_t send_data_array[4] = {0};
    int32_t escape = 0;
//    send_data_array[0] = static_cast<uint8_t>(mode);
    std::memcpy(&escape, &uparam_value, 4);
//    std::memcpy(&send_data_array[1], &uparam_value, 4);
    send_data_array[0] = static_cast<uint8_t>(escape >> 24);
    send_data_array[1] = static_cast<uint8_t>(escape >> 16);
    send_data_array[2] = static_cast<uint8_t>(escape >> 8);
    send_data_array[3] = static_cast<uint8_t>(escape);

    return updateDataSend(address, mode, send_data_array);
    /*
    for(uint8_t i = 0;i < 4;i++){
        send_data_array[i + 1] = (uint8_t)(uparam_value >> (32 - 8*(i + 1)));
    }

    return can_.send(address, send_data_array);
     */
}

bool MotorDriver::setTargetRPM(const uint8_t address, const int32_t target_rpm){
    uint8_t send_data_array[4] = {0};
    send_data_array[0] = static_cast<uint8_t>(target_rpm >> 24);
    send_data_array[1] = static_cast<uint8_t>(target_rpm >> 16);
    send_data_array[2] = static_cast<uint8_t>(target_rpm >> 8);
    send_data_array[3] = static_cast<uint8_t>(target_rpm);

    return updateDataSend(address, drive_command::kPID, send_data_array);
}

bool MotorDriver::setDuty(const uint8_t address, const int32_t duty){
    uint8_t send_data_array[4] = {0};
//    int32_t escape = 0;
//    std::memcpy(send_data_array, &duty, 4);
    send_data_array[0] = static_cast<uint8_t>(duty >> 24);
    send_data_array[1] = static_cast<uint8_t>(duty >> 16);
    send_data_array[2] = static_cast<uint8_t>(duty >> 8);
    send_data_array[3] = static_cast<uint8_t>(duty);

    return updateDataSend(address, drive_command::kDuty, send_data_array);
}
