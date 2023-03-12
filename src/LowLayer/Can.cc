/*
 * Can.cc
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

#include "LowLayer/Can.hh"
#include <array>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wshadow"

using namespace LibMecha::v1::LowLayer;

Can::Can(const CAN_HandleTypeDef &canHandle) : _hcan(canHandle) {
}

Can::~Can() = default;

void Can::sendRemote(const uint8_t address) {
    CAN_TxHeaderTypeDef canTxHeader;
    uint32_t mailBox;
    uint8_t data[1] = {0};
    canTxHeader.StdId              = address;
    canTxHeader.DLC                = 1; // sizeof(sendData)
    canTxHeader.ExtId              = 0x00;
    canTxHeader.RTR                = CAN_RTR_REMOTE;
    canTxHeader.IDE                = CAN_ID_STD;
    canTxHeader.TransmitGlobalTime = DISABLE;

    if(HAL_CAN_AddTxMessage(&_hcan, &canTxHeader, data, &mailBox) != HAL_OK) {
        HAL_CAN_AbortTxRequest(&_hcan, mailBox);
        if(HAL_CAN_AddTxMessage(&_hcan, &canTxHeader, data, &mailBox) != HAL_OK) {
//            Error_Handler();
//            return false;
        }
    }
//    return true;
}

template<std::size_t N> bool Can::send(const uint8_t address, const uint8_t (&sendData)[N]) {
    return send(address, to_array(sendData));
}

template<std::size_t N> bool Can::send(const uint8_t address, const std::array<uint8_t, N> &sendData) {
    CAN_TxHeaderTypeDef canTxHeader;
    uint32_t mailBox;
    canTxHeader.StdId              = address;
    canTxHeader.DLC                = N;
    canTxHeader.ExtId              = 0x00;
    canTxHeader.RTR                = CAN_RTR_DATA;
    canTxHeader.IDE                = CAN_ID_STD;
    canTxHeader.TransmitGlobalTime = DISABLE;

    while(!HAL_CAN_GetTxMailboxesFreeLevel(&_hcan));
    if(HAL_CAN_AddTxMessage(&_hcan, &canTxHeader, sendData, &mailBox) != HAL_OK) {
        HAL_CAN_AbortTxRequest(&_hcan, mailBox);
        if(HAL_CAN_AddTxMessage(&_hcan, &canTxHeader, sendData, &mailBox) != HAL_OK) {
            //            Error_Handler();
            return false;
        }
    }

    return true;
}

void Can::setFilter(uint8_t address) {
    CAN_FilterTypeDef canFilter;
    canFilter.FilterBank           = 0;
    canFilter.FilterMode           = CAN_FILTERMODE_IDMASK;
    canFilter.FilterScale          = CAN_FILTERSCALE_32BIT;
    canFilter.FilterMaskIdHigh     = 0xFFE0;
    canFilter.FilterMaskIdLow      = 0x0000;
    canFilter.FilterIdHigh         = (uint16_t)((uint16_t)address << 5);
    canFilter.FilterIdLow          = 0x0000;
    canFilter.FilterFIFOAssignment = CAN_FILTER_FIFO1;
    canFilter.FilterActivation     = ENABLE;
    canFilter.SlaveStartFilterBank = 14;

    HAL_CAN_ConfigFilter(&_hcan, &canFilter);
}

#pragma clang diagnostic pop
