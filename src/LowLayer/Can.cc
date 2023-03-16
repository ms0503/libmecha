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

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wshadow"

namespace LibMecha {
    inline namespace v2 {
        namespace LowLayer {
            Can::Can(const CAN_HandleTypeDef &canHandle):
                _hcan(canHandle) {
            }

            Can::~Can() = default;

            void Can::setFilter(const std::uint8_t address) {
                CAN_FilterTypeDef canFilter;
                canFilter.FilterBank = 0;
                canFilter.FilterMode = CAN_FILTERMODE_IDMASK;
                canFilter.FilterScale = CAN_FILTERSCALE_32BIT;
                canFilter.FilterMaskIdHigh = 0xFFE0;
                canFilter.FilterMaskIdLow = 0x0000;
                canFilter.FilterIdHigh = static_cast<std::uint16_t>(address) << 5;
                canFilter.FilterIdLow = 0x0000;
                canFilter.FilterFIFOAssignment = CAN_FILTER_FIFO1;
                canFilter.FilterActivation = ENABLE;
                canFilter.SlaveStartFilterBank = 14;

                HAL_CAN_ConfigFilter(&_hcan, &canFilter);
            }

            void Can::sendRemote(const std::uint8_t address) {
                CAN_TxHeaderTypeDef canTxHeader;
                std::uint32_t mailBox;
                std::array<std::uint8_t, 1> data {};
                canTxHeader.StdId = address;
                canTxHeader.DLC = 1;
                canTxHeader.ExtId = 0x00;
                canTxHeader.RTR = CAN_RTR_REMOTE;
                canTxHeader.IDE = CAN_ID_STD;
                canTxHeader.TransmitGlobalTime = DISABLE;

                if(HAL_CAN_AddTxMessage(&_hcan, &canTxHeader, data.data(), &mailBox) != HAL_OK) {
                    HAL_CAN_AbortTxRequest(&_hcan, mailBox);
                    if(HAL_CAN_AddTxMessage(&_hcan, &canTxHeader, data.data(), &mailBox) != HAL_OK) {
                        //Error_Handler();
                    }
                }
            }

            bool Can::send(const std::uint8_t address, std::uint8_t *const sendData, const std::size_t sendDataSize) {
                if(sendDataSize == 0) return false;
                CAN_TxHeaderTypeDef canTxHeader;
                std::uint32_t mailBox;
                canTxHeader.StdId = address;
                canTxHeader.DLC = sendDataSize;
                canTxHeader.ExtId = 0x00;
                canTxHeader.RTR = CAN_RTR_DATA;
                canTxHeader.IDE = CAN_ID_STD;
                canTxHeader.TransmitGlobalTime = DISABLE;

                while(!HAL_CAN_GetTxMailboxesFreeLevel(&_hcan))
                    ;
                if(HAL_CAN_AddTxMessage(&_hcan, &canTxHeader, sendData, &mailBox) != HAL_OK) {
                    HAL_CAN_AbortTxRequest(&_hcan, mailBox);
                    if(HAL_CAN_AddTxMessage(&_hcan, &canTxHeader, sendData, &mailBox) != HAL_OK) {
                        //Error_Handler();
                        return false;
                    }
                }

                return true;
            }

            void Can::init(const std::uint8_t address, const std::uint32_t receiveInterrupt) {
                _receiveInterrupt = receiveInterrupt;
                setFilter(address);
                HAL_CAN_ActivateNotification(&_hcan, _receiveInterrupt);
                HAL_CAN_Start(&_hcan);
            }

            std::array<std::uint8_t, 8> Can::getMessage(const std::uint8_t canRxFifo) {
                std::array<std::uint8_t, 8> result {};
                CAN_RxHeaderTypeDef rxHeader;

                HAL_CAN_GetRxMessage(&_hcan, canRxFifo, &rxHeader, result.data());

                return result;
            }
        }// namespace LowLayer
    }// namespace v2
}// namespace LibMecha

#pragma clang diagnostic pop
