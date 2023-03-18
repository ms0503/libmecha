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

#ifndef _LIBMECHA_GPIO_HH_
#define _LIBMECHA_GPIO_HH_

#include "stm32f4xx_ll_gpio.h"
#include <cstdint>

namespace LibMecha {
    inline namespace v2 {
        namespace LowLayer {
            /// GPIO用低レイヤークラス
            class GPIO {
            public:
                /// GPIOピン情報
                struct Pin {
                    /// GPIOペリフェラル
                    GPIO_TypeDef *const gpio;
                    /// GPIOピン
                    const std::uint32_t pin;
                };

                /// 入出力モード
                enum class PinMode : uint8_t {
                    /// 入力
                    INPUT,
                    /// 入力(プルダウン抵抗付き)
                    INPUT_PULL_DOWN,
                    /// 入力(プルアップ抵抗付き)
                    INPUT_PULL_UP,
                    /// 出力
                    OUTPUT
                };

                enum class PinState : uint8_t {
                    /// LOW信号
                    LOW,
                    /// HIGH信号
                    HIGH
                };

                /**
                 * コンストラクタ
                 * @param pin GPIOピン情報
                 * @param mode 入出力モード
                 */
                explicit GPIO(Pin pin, PinMode mode);
                /**
                 * コンストラクタ
                 * @param gpio GPIOペリフェラル
                 * @param pin GPIOピン
                 * @param mode 入出力モード
                 */
                explicit GPIO(GPIO_TypeDef *gpio, std::uint32_t pin, PinMode mode);
                /**
                 * デストラクタ
                 */
                virtual ~GPIO();

            protected:
                /**
                 * HIGH信号の出力
                 */
                void high() const;
                /**
                 * LOW信号の出力
                 */
                void low() const;
                /**
                 * GPIO信号の読み取り
                 * @return 信号
                 */
                PinState read() const;

            private:
                /**
                 * GPIOピン情報
                 */
                Pin _gpio;
                /**
                 * 入出力モード
                 */
                PinMode _mode;
            };
        }
    }
}

#endif// _LIBMECHA_GPIO_HH_
