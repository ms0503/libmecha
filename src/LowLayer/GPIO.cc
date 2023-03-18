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

namespace LibMecha {
    inline namespace v2 {
        namespace LowLayer {
            GPIO::GPIO(const Pin pin, const PinMode mode):
                _gpio(pin), _mode(mode) {
            }

            GPIO::GPIO(GPIO_TypeDef *const gpio, const std::uint32_t pin, const PinMode mode):
                _gpio({ .gpio = gpio, .pin = pin }), _mode(mode) {
            }

            GPIO::~GPIO() = default;

            void GPIO::high() const {
                LL_GPIO_SetOutputPin(_gpio.gpio, _gpio.pin);
            }

            void GPIO::low() const {
                LL_GPIO_ResetOutputPin(_gpio.gpio, _gpio.pin);
            }

            GPIO::PinState GPIO::read() const {
                uint32_t state = LL_GPIO_IsInputPinSet(_gpio.gpio, _gpio.pin);
                if(_mode == PinMode::INPUT_PULL_UP && state == 0 || _mode != PinMode::INPUT_PULL_UP && state == 1) return PinState::HIGH;
                return PinState::LOW;
            }
        }
    }
}
