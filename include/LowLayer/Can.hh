/*
 * Can.hh
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

#ifndef _LIBMECHA_CAN_HH_
#define _LIBMECHA_CAN_HH_

#include <array>
#include "stm32f4xx_hal.h"

namespace LibMecha {
    inline namespace v1 {
        namespace LowLayer {
            class Can {
            public:
                explicit Can(const CAN_HandleTypeDef &canHandle);
                ~Can();
                void setFilter(uint8_t address);
                void sendRemote(uint8_t address);
                template<std::size_t N> bool send(uint8_t address, const uint8_t (&sendData)[N]);
                template<std::size_t N> bool send(uint8_t address, const std::array<uint8_t, N> &sendData);
                inline void setUp(const uint8_t address, const uint32_t receiveInterrupt) {
                    _receiveInterrupt = receiveInterrupt;
                    setFilter(address);
                    HAL_CAN_ActivateNotification(&_hcan, _receiveInterrupt);
                    HAL_CAN_Start(&_hcan);
                }
                inline void setCANHandle(const CAN_HandleTypeDef &canHandle) {
                    _hcan = canHandle;
                }
                inline std::array<uint8_t, 8> getMessage(const uint8_t canRxFifo) {
                    std::array<uint8_t, 8> result = {0};
                    CAN_RxHeaderTypeDef rxHeader;
                    uint8_t rxData[8] = {0};

                    HAL_CAN_GetRxMessage(&_hcan, canRxFifo, &rxHeader, rxData);
                    for (uint8_t i = 0; i < sizeof(rxData); i++) result.at(i) = rxData[i];

                    return result;
                }

            private:
                CAN_HandleTypeDef _hcan{};
                uint32_t _receiveInterrupt = CAN_IT_RX_FIFO0_MSG_PENDING;
            };
        }
    }
}

#endif// _LIBMECHA_CAN_HH_
