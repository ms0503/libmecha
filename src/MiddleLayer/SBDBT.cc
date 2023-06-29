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

#if (defined USE_FULL_LL_DRIVER && !defined DISABLE_USART && !defined DISABLE_SBDBT)

#include <MiddleLayer/SBDBT.hh>
#include <numeric>

namespace LibMecha::MiddleLayer {
    SBDBT::SBDBT():
        _lastButtonState(), _processedReceiveData() {
    }

    bool SBDBT::receiveCheck(std::array<std::uint8_t, SBDBT_RECEIVE_SIZE> &receiveData) {
        std::array<std::uint8_t, SBDBT_RECEIVE_SIZE> processedReceiveData {};
        std::uint8_t b;
        std::uint8_t checksum;

        for(std::uint8_t i = 0; i < receiveData.size(); i++) {
            if(receiveData.at(i) == kStartByte) {
                b = i;
                for(std::uint8_t &a : processedReceiveData) {
                    if(7 < b) b = 0;
                    //data[a] = receiveData[7 - i + a];
                    a = receiveData.at(b);
                    b++;
                }
            }
        }
        if(processedReceiveData.at(0) == kStartByte) {
            checksum = std::accumulate(processedReceiveData.begin() + 1, processedReceiveData.end() - 1, 0) & 0b0111'1111;
            if(checksum == processedReceiveData.at(7)) {
                std::copy(processedReceiveData.begin(), processedReceiveData.end(), _processedReceiveData.begin());

                return true;
            }
        }

        return false;
    }

    void SBDBT::receiveProcessing(SBDBT::ButtonAssignment &button) {
        _as = {
            .LX = static_cast<std::int8_t>(_processedReceiveData.at(3) - 0b0100'0000),
            .LY = static_cast<std::int8_t>(_processedReceiveData.at(4) - 0b0100'0000),
            .RX = static_cast<std::int8_t>(_processedReceiveData.at(5) - 0b0100'0000),
            .RY = static_cast<std::int8_t>(_processedReceiveData.at(6) - 0b0100'0000)
        };
        std::uint16_t receiveData = (_processedReceiveData.at(1) << 8) + _processedReceiveData.at(2);
        button.Up = identifyButtonState(_lastButtonState.Up, (receiveData & kButtonUp) != 0);
        button.Down = identifyButtonState(_lastButtonState.Down, (receiveData & kButtonDown) != 0);
        button.Right = identifyButtonState(_lastButtonState.Right, (receiveData & kButtonRight) != 0);
        button.Left = identifyButtonState(_lastButtonState.Left, (receiveData & kButtonLeft) != 0);
        button.Triangle = identifyButtonState(_lastButtonState.Triangle, (receiveData & kButtonTriangle) != 0);
        button.Cross = identifyButtonState(_lastButtonState.Cross, (receiveData & kButtonCross) != 0);
        button.Circle = identifyButtonState(_lastButtonState.Circle, (receiveData & kButtonCircle) != 0);
        button.Square = identifyButtonState(_lastButtonState.Square, (receiveData & kButtonSquare) != 0);
        button.L1 = identifyButtonState(_lastButtonState.L1, (receiveData & kButtonL1) != 0);
        button.L2 = identifyButtonState(_lastButtonState.L2, (receiveData & kButtonL2) != 0);
        button.L3 = identifyButtonState(_lastButtonState.L3, (receiveData & kButtonL3) != 0);
        button.R1 = identifyButtonState(_lastButtonState.R1, (receiveData & kButtonR1) != 0);
        button.R2 = identifyButtonState(_lastButtonState.R2, (receiveData & kButtonR2) != 0);
        button.R3 = identifyButtonState(_lastButtonState.R3, (receiveData & kButtonR3) != 0);
        button.Start = identifyButtonState(_lastButtonState.Start, (receiveData & kButtonStart) != 0);
        button.Select = identifyButtonState(_lastButtonState.Select, (receiveData & kButtonSelect) != 0);
        _lastButtonState = button;
    }

    void SBDBT::buttonAssignmentInit(SBDBT::ButtonAssignment &ba) {
        ba.Up = SBDBT::ButtonState::kRelease;
        ba.Down = SBDBT::ButtonState::kRelease;
        ba.Right = SBDBT::ButtonState::kRelease;
        ba.Left = SBDBT::ButtonState::kRelease;
        ba.Triangle = SBDBT::ButtonState::kRelease;
        ba.Cross = SBDBT::ButtonState::kRelease;
        ba.Circle = SBDBT::ButtonState::kRelease;
        ba.Square = SBDBT::ButtonState::kRelease;
        ba.L1 = SBDBT::ButtonState::kRelease;
        ba.L2 = SBDBT::ButtonState::kRelease;
        ba.L3 = SBDBT::ButtonState::kRelease;
        ba.R1 = SBDBT::ButtonState::kRelease;
        ba.R2 = SBDBT::ButtonState::kRelease;
        ba.R3 = SBDBT::ButtonState::kRelease;
        ba.Start = SBDBT::ButtonState::kRelease;
        ba.Select = SBDBT::ButtonState::kRelease;
    }

    SBDBT::ButtonState SBDBT::identifyButtonState(ButtonState &lastButtonState, const bool isPush) {
        if(lastButtonState == ButtonState::kPush || lastButtonState == ButtonState::kPushEdge) {
            if(isPush) return ButtonState::kPush;
            return ButtonState::kReleaseEdge;
        }
        if((lastButtonState == ButtonState::kRelease || lastButtonState == ButtonState::kReleaseEdge) && isPush) return ButtonState::kPushEdge;
        return ButtonState::kRelease;
    }
} // namespace LibMecha::LowLayer

#endif
