/*
 * SBDBT.cpp
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

#include "SBDBT.hpp"

SBDBT::SBDBT(const UART_HandleTypeDef &uart_handle) : UARTHandle_(uart_handle) {
}

SBDBT::~SBDBT() = default;

bool SBDBT::receiveCheck(const uint8_t (&receive_data)[8]) {
    uint8_t processed_receive_data[8] = {0};
    uint8_t b;
    uint16_t checksum_check = 0;

    for(uint8_t i = 0; i < sizeof(receive_data); i++) {
        if(receive_data[i] == kStartByte){
            b = i;
            for(uint8_t & a : processed_receive_data) {
                if(7 < b) {
                    b = 0;
                }
//                data[a] = receive_data[((7 - i) + a)];
                a = receive_data[b];
                b++;
            }
        }
    }
    if(processed_receive_data[0] == kStartByte) {
        for(uint8_t i = 1; i < 7; i++) {
            checksum_check += processed_receive_data[i];
        }
        checksum_check &= 0b01111111;
        if(static_cast<uint8_t>(checksum_check) == processed_receive_data[7]) {
            for(uint8_t i = 0; i < 7; i++) {
                processed_receive_data_[i] = processed_receive_data[i];
            }
//            processed_receive_data_ = processed_receive_data;

            return true;
        }
    }

    return false;
}

SBDBT::ButtonAssignment SBDBT::receiveProcessing() {
    ButtonAssignment return_Button{};
    uint16_t button_receive_data = 0;
    return_AnalogState_.LX = static_cast<int8_t>(processed_receive_data_[3] - 64);
    return_AnalogState_.LY = static_cast<int8_t>(processed_receive_data_[4] - 64) * (-1);
    return_AnalogState_.RX = static_cast<int8_t>(processed_receive_data_[5] - 64);
    return_AnalogState_.RY = static_cast<int8_t>(processed_receive_data_[6] - 64) * (-1);
    button_receive_data = (processed_receive_data_[1] << 8) + (processed_receive_data_[2]);
//    std::memcpy(&button_receive_data, &processed_receive_data_[1], 2);
    return_Button.Up       = identifyButtonState(last_ButtonState_.Up      , (button_receive_data & kButtonUp)       == kButtonUp);
    return_Button.Down     = identifyButtonState(last_ButtonState_.Down    , (button_receive_data & kButtonDown)     == kButtonDown);
    return_Button.Right    = identifyButtonState(last_ButtonState_.Right   , (button_receive_data & kButtonRight)    == kButtonRight);
    return_Button.Left     = identifyButtonState(last_ButtonState_.Left    , (button_receive_data & kButtonLeft)     == kButtonLeft);
    return_Button.Triangle = identifyButtonState(last_ButtonState_.Triangle, (button_receive_data & kButtonTriangle) == kButtonTriangle);
    return_Button.Cross    = identifyButtonState(last_ButtonState_.Cross   , (button_receive_data & kButtonCross)    == kButtonCross);
    return_Button.Circle   = identifyButtonState(last_ButtonState_.Circle  , (button_receive_data & kButtonCircle)   == kButtonCircle);
    return_Button.Square   = identifyButtonState(last_ButtonState_.Square  , (button_receive_data & kButtonSquare)   == kButtonSquare);
    return_Button.L1       = identifyButtonState(last_ButtonState_.L1      , (button_receive_data & kButtonL1)       == kButtonL1);
    return_Button.L2       = identifyButtonState(last_ButtonState_.L2      , (button_receive_data & kButtonL2)       == kButtonL2);
    return_Button.L3       = identifyButtonState(last_ButtonState_.L3      , (button_receive_data & kButtonL3)       == kButtonL3);
    return_Button.R1       = identifyButtonState(last_ButtonState_.R1      , (button_receive_data & kButtonR1)       == kButtonR1);
    return_Button.R2       = identifyButtonState(last_ButtonState_.R2      , (button_receive_data & kButtonR2)       == kButtonR2);
    return_Button.R3       = identifyButtonState(last_ButtonState_.R3      , (button_receive_data & kButtonR3)       == kButtonR3);
    return_Button.Start    = identifyButtonState(last_ButtonState_.Start   , (button_receive_data & kButtonStart)    == kButtonStart);
    return_Button.Select   = identifyButtonState(last_ButtonState_.Select  , (button_receive_data & kButtonSelect)   == kButtonSelect);
    last_ButtonState_ = return_Button;

    return return_Button;
}

SBDBT::ButtonAssignment SBDBT::buttonAssignmentInit() {
    SBDBT::ButtonAssignment button_assignment{};
    button_assignment.Up       = SBDBT::ButtonState::kRelease;
    button_assignment.Down     = SBDBT::ButtonState::kRelease;
    button_assignment.Right    = SBDBT::ButtonState::kRelease;
    button_assignment.Left     = SBDBT::ButtonState::kRelease;
    button_assignment.Triangle = SBDBT::ButtonState::kRelease;
    button_assignment.Cross    = SBDBT::ButtonState::kRelease;
    button_assignment.Circle   = SBDBT::ButtonState::kRelease;
    button_assignment.Square   = SBDBT::ButtonState::kRelease;
    button_assignment.L1       = SBDBT::ButtonState::kRelease;
    button_assignment.L2       = SBDBT::ButtonState::kRelease;
    button_assignment.L3       = SBDBT::ButtonState::kRelease;
    button_assignment.R1       = SBDBT::ButtonState::kRelease;
    button_assignment.R2       = SBDBT::ButtonState::kRelease;
    button_assignment.R3       = SBDBT::ButtonState::kRelease;
    button_assignment.Start    = SBDBT::ButtonState::kRelease;
    button_assignment.Select   = SBDBT::ButtonState::kRelease;

    return button_assignment;
}

SBDBT::ButtonState SBDBT::identifyButtonState(const SBDBT::ButtonState last_button_state, const bool is_push) {
    SBDBT::ButtonState button_state;

    switch(last_button_state) {
        case SBDBT::ButtonState::kRelease:
        case SBDBT::ButtonState::kReleaseEdge:
            if(is_push) {
                button_state = SBDBT::ButtonState::kPushEdge;
            } else {
                button_state = SBDBT::ButtonState::kRelease;
            }
            break;
        case SBDBT::ButtonState::kPush:
        case SBDBT::ButtonState::kPushEdge:
            if(is_push) {
                button_state = SBDBT::ButtonState::kPush;
            } else {
                button_state = SBDBT::ButtonState::kReleaseEdge;
            }
            break;
        default:
            button_state = SBDBT::ButtonState::kRelease;
            break;
    }

    return button_state;
}
