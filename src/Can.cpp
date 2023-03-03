/*
 * Can.cpp
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

#include "Can.hpp"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wshadow"

Can::Can(const CAN_HandleTypeDef &hcan) : hcan_(hcan) {
}

Can::~Can() = default;

void Can::sendRemote(const uint8_t address) {
    CAN_TxHeaderTypeDef canTxHeaderStruct;
    uint32_t mail_box;
    uint8_t data[1] = {0};
    canTxHeaderStruct.StdId              = address;
    canTxHeaderStruct.DLC                = 1/*sizeof(send_data)*/;
    canTxHeaderStruct.ExtId              = 0x00;
    canTxHeaderStruct.RTR                = CAN_RTR_REMOTE;
    canTxHeaderStruct.IDE                = CAN_ID_STD;
    canTxHeaderStruct.TransmitGlobalTime = DISABLE;

    if(HAL_CAN_AddTxMessage(&hcan_, &canTxHeaderStruct, data, &mail_box) != HAL_OK) {
        HAL_CAN_AbortTxRequest(&hcan_, mail_box);
        if(HAL_CAN_AddTxMessage(&hcan_, &canTxHeaderStruct, data, &mail_box) != HAL_OK) {
//            Error_Handler();
//            return false;
        }
    }
//    return true;
}

template<std::size_t N> bool Can::send(const uint8_t address, const uint8_t (&send_data)[N]) {
    CAN_TxHeaderTypeDef canTxHeaderStruct;
    uint32_t mail_box;
    canTxHeaderStruct.StdId              = address;
    canTxHeaderStruct.DLC                = N;
    canTxHeaderStruct.ExtId              = 0x00;
    canTxHeaderStruct.RTR                = CAN_RTR_DATA;
    canTxHeaderStruct.IDE                = CAN_ID_STD;
    canTxHeaderStruct.TransmitGlobalTime = DISABLE;

    while(!HAL_CAN_GetTxMailboxesFreeLevel(&hcan_));
    if(HAL_CAN_AddTxMessage(&hcan_, &canTxHeaderStruct, send_data, &mail_box) != HAL_OK) {
        HAL_CAN_AbortTxRequest(&hcan_, mail_box);
        if(HAL_CAN_AddTxMessage(&hcan_, &canTxHeaderStruct, send_data, &mail_box) != HAL_OK) {
            //            Error_Handler();
            return false;
        }
    }

    return true;
}

template<std::size_t N> bool Can::send(const uint8_t address, const std::array<uint8_t, N> &send_data) {
    CAN_TxHeaderTypeDef canTxHeaderStruct;
    uint32_t mail_box;
    uint8_t copy_send_data[N] = {0};
    canTxHeaderStruct.StdId              = address;
    canTxHeaderStruct.DLC                = N;
    canTxHeaderStruct.ExtId              = 0x00;
    canTxHeaderStruct.RTR                = CAN_RTR_DATA;
    canTxHeaderStruct.IDE                = CAN_ID_STD;
    canTxHeaderStruct.TransmitGlobalTime = DISABLE;

    for(auto copy_send_data:send_data)
        ;
    while(!HAL_CAN_GetTxMailboxesFreeLevel(&hcan_));
    if(HAL_CAN_AddTxMessage(&hcan_, &canTxHeaderStruct, send_data, &mail_box) != HAL_OK) {
        HAL_CAN_AbortTxRequest(&hcan_, mail_box);
        if(HAL_CAN_AddTxMessage(&hcan_, &canTxHeaderStruct, send_data, &mail_box) != HAL_OK) {
            //            Error_Handler();
            return false;
        }
    }

    return true;
}

void Can::setFilter(uint8_t address) {
    CAN_FilterTypeDef CAN_FilterStruct;
    CAN_FilterStruct.FilterBank           = 0;
    CAN_FilterStruct.FilterMode           = CAN_FILTERMODE_IDMASK;
    CAN_FilterStruct.FilterScale          = CAN_FILTERSCALE_32BIT;
    CAN_FilterStruct.FilterMaskIdHigh     = 0xFFE0;
    CAN_FilterStruct.FilterMaskIdLow      = 0x0000;
    CAN_FilterStruct.FilterIdHigh         = (uint16_t)((uint16_t)address << 5);
    CAN_FilterStruct.FilterIdLow          = 0x0000;
    CAN_FilterStruct.FilterFIFOAssignment = CAN_FILTER_FIFO1;
    CAN_FilterStruct.FilterActivation     = ENABLE;
    CAN_FilterStruct.SlaveStartFilterBank = 14;

    HAL_CAN_ConfigFilter(&hcan_, &CAN_FilterStruct);
}

#pragma clang diagnostic pop
