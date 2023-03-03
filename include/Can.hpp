/*
 * CAN.hpp
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

#ifndef MYLIBINC_CAN_HPP_
#define MYLIBINC_CAN_HPP_

#include <array>
#include "stm32f4xx_hal.h"

class Can {
public:
    explicit Can(const CAN_HandleTypeDef &hcan);
    ~Can();
    void setFilter(uint8_t set_address);
    void sendRemote(uint8_t address);
    template<std::size_t N> bool send(uint8_t address, const uint8_t (&send_data)[N]);
    template<std::size_t N> bool send(uint8_t address, const std::array<uint8_t, N> &send_data);
    inline void setUp(const uint8_t set_address, const uint32_t receive_interrupt) {
        receive_interrupt_ = receive_interrupt;
        setFilter(set_address);
        HAL_CAN_ActivateNotification(&hcan_, receive_interrupt_);
        HAL_CAN_Start(&hcan_);
    }
    inline void setCANHandle(const CAN_HandleTypeDef &hcan) {
        hcan_ = hcan;
    }
    inline std::array<uint8_t, 8> getMessage(const uint8_t can_rx_fifo) {
        std::array<uint8_t, 8> result = {0};
        CAN_RxHeaderTypeDef RxHeader;
        uint8_t rx_data[8] = {0};

        HAL_CAN_GetRxMessage(&hcan_, can_rx_fifo, &RxHeader, rx_data);
        for(uint8_t i = 0; i < sizeof(rx_data); i++){
            result.at(i) = rx_data[i];
        }

        return result;
    }

private:
    CAN_HandleTypeDef hcan_{};
    uint32_t receive_interrupt_ = CAN_IT_RX_FIFO0_MSG_PENDING;
};

#endif /* MYLIBINC_CAN_HPP_ */
