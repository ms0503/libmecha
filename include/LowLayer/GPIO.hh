/*
 * GPIO.hh
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

#pragma once

#include <LowLayer/Peripheral.hh>
#include <cstddef>
#include <cstdint>
#include <stm32_drivers.h>
#include <vector>

namespace LibMecha::LowLayer {
    /// GPIO
    class GPIO : public Peripheral {
    public:
        using Peripheral::Peripheral;

        /// 入出力モード
        enum class PinMode : std::uint8_t {
            /// 入力
            INPUT,
            /// 入力(プルダウン抵抗付き)
            INPUT_PULL_DOWN,
            /// 入力(プルアップ抵抗付き)
            INPUT_PULL_UP,
            /// 出力
            OUTPUT
        };

        /// GPIOピン
        struct Pin {
            /// GPIO
            GPIO_TypeDef *const gpio;
            /// ピン番号
            const std::uint32_t pin;
            /// 入出力モード
            const PinMode mode;
        };

        /// GPIOピン状態
        enum class PinState : std::uint8_t {
            /// LOW信号
            LOW,
            /// HIGH信号
            HIGH
        };

        /**
         * コンストラクター
         * @param pin GPIOピン
         */
        explicit GPIO(const std::vector<Pin> &pin);

    protected:
        /// GPIOピン状態
        std::vector<PinState> _state {};

        /**
         * HIGH信号の出力
         * @param index ピン配列のインデックス
         */
        inline void high(const std::size_t index) {
            LL_GPIO_SetOutputPin(_gpio.at(index).gpio, _gpio.at(index).pin);
            _state.at(index) = PinState::HIGH;
        }

        /**
         * LOW信号の出力
         * @param index ピン配列のインデックス
         */
        inline void low(std::size_t index) {
            LL_GPIO_ResetOutputPin(_gpio.at(index).gpio, _gpio.at(index).pin);
            _state.at(index) = PinState::LOW;
        }

        /**
         * HIGH/LOW信号の切り替え
         * @param index ピン配列のインデックス
         */
        inline void toggle(const std::size_t index) {
            LL_GPIO_TogglePin(_gpio.at(index).gpio, _gpio.at(index).pin);
            if(_state.at(index) == PinState::LOW) _state.at(index) = PinState::HIGH;
            else _state.at(index) = PinState::LOW;
        }

        /**
         * GPIO信号の読み取り
         * @param index ピン配列のインデックス
         * @return 信号
         */
        [[nodiscard]] inline PinState read(std::size_t index) const {
            std::uint32_t state = LL_GPIO_IsInputPinSet(_gpio.at(index).gpio, _gpio.at(index).pin);
            if(_gpio.at(index).mode == PinMode::INPUT_PULL_UP && state == 0 || _gpio.at(index).mode != PinMode::INPUT_PULL_UP && state == 1) return PinState::HIGH;
            return PinState::LOW;
        }

    private:
        /// GPIOピン配列
        std::vector<Pin> _gpio;
    };
} // namespace LibMecha::LowLayer
