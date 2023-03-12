/*
 * SBDBT.cc
 *
 *  Created on: 2019/07/06
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

#include "LowLayer/SBDBT.hh"

using namespace LibMecha::v1::LowLayer;

SBDBT::SBDBT(const UART_HandleTypeDef &uartHandle) : _uartHandle(uartHandle) {
}

SBDBT::~SBDBT() = default;

bool SBDBT::receiveCheck(const uint8_t (&receiveData)[8]) {
    uint8_t processedReceiveData[8] = {0};
    uint8_t b;
    uint16_t checksum = 0;

    for(uint8_t i = 0; i < sizeof(receiveData); i++) {
        if(receiveData[i] == kStartByte) {
            b = i;
            for(uint8_t &a : processedReceiveData) {
                if(7 < b) b = 0;
//                data[a] = receiveData[7 - i + a];
                a = receiveData[b];
                b++;
            }
        }
    }
    if(processedReceiveData[0] == kStartByte) {
        for(uint8_t i = 1; i < 7; i++) checksum += processedReceiveData[i];
        checksum &= 0b01111111;
        if(static_cast<uint8_t>(checksum) == processedReceiveData[7]) {
            for(uint8_t i = 0; i < 7; i++) _processedReceiveData[i] = processedReceiveData[i];
//            _processedReceiveData = processedReceiveData;

            return true;
        }
    }

    return false;
}

SBDBT::ButtonAssignment SBDBT::receiveProcessing() {
    ButtonAssignment button{};
    uint16_t receiveData = 0;
    _as.LX = static_cast<int8_t>(_processedReceiveData[3] - 64);
    _as.LY = static_cast<int8_t>(_processedReceiveData[4] - 64) * (-1);
    _as.RX = static_cast<int8_t>(_processedReceiveData[5] - 64);
    _as.RY = static_cast<int8_t>(_processedReceiveData[6] - 64) * (-1);
    receiveData = (_processedReceiveData[1] << 8) + (_processedReceiveData[2]);
//    std::memcpy(&receiveData, &_processedReceiveData[1], 2);
    button.Up       = identifyButtonState(_lastButtonState.Up      , (receiveData & kButtonUp)       == kButtonUp);
    button.Down     = identifyButtonState(_lastButtonState.Down    , (receiveData & kButtonDown)     == kButtonDown);
    button.Right    = identifyButtonState(_lastButtonState.Right   , (receiveData & kButtonRight)    == kButtonRight);
    button.Left     = identifyButtonState(_lastButtonState.Left    , (receiveData & kButtonLeft)     == kButtonLeft);
    button.Triangle = identifyButtonState(_lastButtonState.Triangle, (receiveData & kButtonTriangle) == kButtonTriangle);
    button.Cross    = identifyButtonState(_lastButtonState.Cross   , (receiveData & kButtonCross)    == kButtonCross);
    button.Circle   = identifyButtonState(_lastButtonState.Circle  , (receiveData & kButtonCircle)   == kButtonCircle);
    button.Square   = identifyButtonState(_lastButtonState.Square  , (receiveData & kButtonSquare)   == kButtonSquare);
    button.L1       = identifyButtonState(_lastButtonState.L1      , (receiveData & kButtonL1)       == kButtonL1);
    button.L2       = identifyButtonState(_lastButtonState.L2      , (receiveData & kButtonL2)       == kButtonL2);
    button.L3       = identifyButtonState(_lastButtonState.L3      , (receiveData & kButtonL3)       == kButtonL3);
    button.R1       = identifyButtonState(_lastButtonState.R1      , (receiveData & kButtonR1)       == kButtonR1);
    button.R2       = identifyButtonState(_lastButtonState.R2      , (receiveData & kButtonR2)       == kButtonR2);
    button.R3       = identifyButtonState(_lastButtonState.R3      , (receiveData & kButtonR3)       == kButtonR3);
    button.Start    = identifyButtonState(_lastButtonState.Start   , (receiveData & kButtonStart)    == kButtonStart);
    button.Select   = identifyButtonState(_lastButtonState.Select  , (receiveData & kButtonSelect)   == kButtonSelect);
    _lastButtonState = button;

    return button;
}

SBDBT::ButtonAssignment SBDBT::buttonAssignmentInit() {
    ButtonAssignment ba{};
    ba.Up       = SBDBT::ButtonState::kRelease;
    ba.Down     = SBDBT::ButtonState::kRelease;
    ba.Right    = SBDBT::ButtonState::kRelease;
    ba.Left     = SBDBT::ButtonState::kRelease;
    ba.Triangle = SBDBT::ButtonState::kRelease;
    ba.Cross    = SBDBT::ButtonState::kRelease;
    ba.Circle   = SBDBT::ButtonState::kRelease;
    ba.Square   = SBDBT::ButtonState::kRelease;
    ba.L1       = SBDBT::ButtonState::kRelease;
    ba.L2       = SBDBT::ButtonState::kRelease;
    ba.L3       = SBDBT::ButtonState::kRelease;
    ba.R1       = SBDBT::ButtonState::kRelease;
    ba.R2       = SBDBT::ButtonState::kRelease;
    ba.R3       = SBDBT::ButtonState::kRelease;
    ba.Start    = SBDBT::ButtonState::kRelease;
    ba.Select   = SBDBT::ButtonState::kRelease;

    return ba;
}

SBDBT::ButtonState SBDBT::identifyButtonState(const ButtonState lastButtonState, const bool isPush) {
    switch(lastButtonState) {
        case ButtonState::kRelease:
        case ButtonState::kReleaseEdge:
            if(isPush) return ButtonState::kPushEdge;
            else return ButtonState::kRelease;
        case ButtonState::kPush:
        case ButtonState::kPushEdge:
            if(isPush) return ButtonState::kPush;
            else return ButtonState::kReleaseEdge;
        default:
            return ButtonState::kRelease;
    }
}
