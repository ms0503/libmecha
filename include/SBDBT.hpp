/*
 * SBDBT.hpp
 *
 *  Created on: 2019/07/06
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

#ifndef MYLIBINC_SBDBT_HPP_
#define MYLIBINC_SBDBT_HPP_

#include <cstring>
#include "stm32f4xx_hal.h"

class SBDBT {
public:
    enum class ButtonState {
        kPushEdge = 0,
        kPush,
        kReleaseEdge,
        kRelease
    };

    struct ButtonAssignment {
        ButtonState Up;
        ButtonState Down;
        ButtonState Right;
        ButtonState Left;
        ButtonState Triangle;
        ButtonState Cross;
        ButtonState Circle;
        ButtonState Square;
        ButtonState L1;
        ButtonState L2;
        ButtonState L3;
        ButtonState R1;
        ButtonState R2;
        ButtonState R3;
        ButtonState Start;
        ButtonState Select;
    };

    struct AnalogState {
        int8_t RX = 0;
        int8_t RY = 0;
        int8_t LX = 0;
        int8_t LY = 0;
    };

    explicit SBDBT(const UART_HandleTypeDef &uart_handle);
    ~SBDBT();
    inline AnalogState getAnalogState() {
        return return_AnalogState_;
    }
    inline void init() {
        last_ButtonState_ = buttonAssignmentInit();
        return_AnalogState_.LX = 0;
        return_AnalogState_.LY = 0;
        return_AnalogState_.RX = 0;
        return_AnalogState_.RY = 0;
        for(uint8_t &i : processed_receive_data_) {
            i = 0;
        }
    }
    bool receiveCheck(const uint8_t (&receive_data)[8]);
    ButtonAssignment receiveProcessing();
    static ButtonAssignment buttonAssignmentInit();

private:
    static constexpr uint8_t  kStartByte      = 0x80;
    static constexpr uint16_t kButtonUp       = 0x0001;
    static constexpr uint16_t kButtonDown     = 0x0002;
    static constexpr uint16_t kButtonRight    = 0x0004;
    static constexpr uint16_t kButtonLeft     = 0x0008;
    static constexpr uint16_t kButtonTriangle = 0x0010;
    static constexpr uint16_t kButtonCross    = 0x0020;
    static constexpr uint16_t kButtonCircle   = 0x0040;
    static constexpr uint16_t kButtonSquare   = 0x0100;
    static constexpr uint16_t kButtonL1       = 0x0200;
    static constexpr uint16_t kButtonL2       = 0x0400;
    static constexpr uint16_t kButtonL3       = 0x4000;
    static constexpr uint16_t kButtonR1       = 0x0800;
    static constexpr uint16_t kButtonR2       = 0x1000;
    static constexpr uint16_t kButtonR3       = 0x8000;
    static constexpr uint16_t kButtonStart    = 0x0080;
    static constexpr uint16_t kButtonSelect   = 0x2000;
    static constexpr uint16_t kHighByte       = 0xFF00;
    static constexpr uint16_t kLowByte        = 0x00FF;
    UART_HandleTypeDef UARTHandle_{};
    ButtonAssignment last_ButtonState_{};
    AnalogState return_AnalogState_;
    uint8_t processed_receive_data_[8] = {0};

    static ButtonState identifyButtonState(ButtonState last_button_state, bool is_push);
};

#endif /* MYLIBINC_SBDBT_HPP_ */
