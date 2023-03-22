/*
* GPIO.cc
*
*  Created on: 2023/03/18
*      Author: ms0503
*
*  This file is part of libmecha.
*
*  libmecha is free software: you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
*
*  libmecha is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.
*
*  You should have received a copy of the GNU Lesser General Public License along with libmecha. If not, see <https://www.gnu.org/licenses/>.
*/

#include "LowLayer/GPIO.hh"
#include "Utils.hh"

namespace LibMecha::LowLayer {
    GPIO::GPIO(const std::vector<Pin> &pin):
        _gpio(pin) {
    }

    void GPIO::high(const std::size_t index) const {
        LL_GPIO_SetOutputPin(_gpio.at(index).gpio, _gpio.at(index).pin);
    }

    void GPIO::low(const std::size_t index) const {
        LL_GPIO_ResetOutputPin(_gpio.at(index).gpio, _gpio.at(index).pin);
    }

    GPIO::PinState GPIO::read(const std::size_t index) const {
        uint32_t state = LL_GPIO_IsInputPinSet(_gpio.at(index).gpio, _gpio.at(index).pin);
        if(_gpio.at(index).mode == PinMode::INPUT_PULL_UP && state == 0 || _gpio.at(index).mode != PinMode::INPUT_PULL_UP && state == 1) return PinState::HIGH;
        return PinState::LOW;
    }
} // namespace LibMecha::LowLayer
