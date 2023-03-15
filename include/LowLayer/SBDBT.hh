/*
 * SBDBT.hh
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

#ifndef _LIBMECHA_SBDBT_HH_
#define _LIBMECHA_SBDBT_HH_

#include "LowLayer/SBDBT.h"
#include "stm32f4xx_ll_usart.h"
#include <array>
#include <cstdint>
#include <cstring>

namespace LibMecha {
    inline namespace v2 {
        namespace LowLayer {
            /// SBDBTのデータサイズ
            constexpr const std::uint16_t SBDBT_RECEIVE_SIZE = C_SBDBT_RECEIVE_SIZE;

            /// SBDBT用低レイヤークラス
            class SBDBT {
            public:
                /// ボタンの状態
                enum class ButtonState {
                    /// 押された瞬間
                    kPushEdge,
                    /// 押されている
                    kPush,
                    /// 離された瞬間
                    kReleaseEdge,
                    /// 離れている
                    kRelease
                };

                /// ボタンアサイン
                struct ButtonAssignment {
                    /// 十字ボタン上
                    ButtonState Up;
                    /// 十字ボタン下
                    ButtonState Down;
                    /// 十字ボタン右
                    ButtonState Right;
                    /// 十字ボタン左
                    ButtonState Left;
                    /// △
                    ButtonState Triangle;
                    /// ☓
                    ButtonState Cross;
                    /// ○
                    ButtonState Circle;
                    /// □
                    ButtonState Square;
                    /// 左ショルダー
                    ButtonState L1;
                    /// 左トリガー
                    ButtonState L2;
                    /// 左スティック押し込み
                    ButtonState L3;
                    /// 右ショルダー
                    ButtonState R1;
                    /// 右トリガー
                    ButtonState R2;
                    /// 右スティック押し込み
                    ButtonState R3;
                    /// 中央右ボタン
                    ButtonState Start;
                    /// 中央左ボタン
                    ButtonState Select;
                };

                /// アナログスティックの状態
                struct AnalogState {
                    /// 右スティック横軸
                    std::int8_t RX = 0;
                    /// 右スティック縦軸
                    std::int8_t RY = 0;
                    /// 左スティック横軸
                    std::int8_t LX = 0;
                    /// 左スティック縦軸
                    std::int8_t LY = 0;
                };

                /**
                 * コンストラクタ
                 * @param usart UART/USARTペリフェラル
                 */
                explicit SBDBT(USART_TypeDef *usart);
                /**
                 * デストラクタ
                 */
                ~SBDBT();
                /**
                 * アナログスティックの状態の取得
                 * @return アナログスティックの状態
                 */
                inline AnalogState getAnalogState() {
                    return _as;
                }
                /**
                 * 初期化
                 */
                inline void init() {
                    _lastButtonState = buttonAssignmentInit();
                    _as = {.RX = 0, .RY = 0, .LX = 0, .LY = 0};
                    for(std::uint8_t &i: _processedReceiveData) i = 0;
                }
                /**
                 * 受信データのバリデーション
                 * @param receiveData 受信データ
                 * @return 受信データが有効か
                 */
                bool receiveCheck(std::uint8_t (&receiveData)[SBDBT_RECEIVE_SIZE]);
                /**
                 * 受信データのバリデーション
                 * @param receiveData 受信データ
                 * @return 受信データが有効か
                 */
                bool receiveCheck(std::array<std::uint8_t, SBDBT_RECEIVE_SIZE> receiveData);
                /**
                 * データを受信しボタン入力へ変換
                 * @return ボタン入力
                 */
                ButtonAssignment receiveProcessing();
                /**
                 * ボタンアサインの初期化
                 * @return ボタンアサイン
                 */
                static ButtonAssignment buttonAssignmentInit();

            private:
                /// 開始バイト
                static constexpr const std::uint8_t kStartByte = 0x80;
                /// 十字ボタン上
                static constexpr const std::uint16_t kButtonUp = 0x0001;
                /// 十字ボタン下
                static constexpr const std::uint16_t kButtonDown = 0x0002;
                /// 十字ボタン右
                static constexpr const std::uint16_t kButtonRight = 0x0004;
                /// 十字ボタン左
                static constexpr const std::uint16_t kButtonLeft = 0x0008;
                /// △
                static constexpr const std::uint16_t kButtonTriangle = 0x0010;
                /// ☓
                static constexpr const std::uint16_t kButtonCross = 0x0020;
                /// ○
                static constexpr const std::uint16_t kButtonCircle = 0x0040;
                /// 中央右ボタン
                static constexpr const std::uint16_t kButtonStart = 0x0080;
                /// □
                static constexpr const std::uint16_t kButtonSquare = 0x0100;
                /// 左ショルダー
                static constexpr const std::uint16_t kButtonL1 = 0x0200;
                /// 左トリガー
                static constexpr const std::uint16_t kButtonL2 = 0x0400;
                /// 右ショルダー
                static constexpr const std::uint16_t kButtonR1 = 0x0800;
                /// 右トリガー
                static constexpr const std::uint16_t kButtonR2 = 0x1000;
                /// 中央左ボタン
                static constexpr const std::uint16_t kButtonSelect = 0x2000;
                /// 左スティック押し込み
                static constexpr const std::uint16_t kButtonL3 = 0x4000;
                /// 右スティック押し込み
                static constexpr const std::uint16_t kButtonR3 = 0x8000;
                /// 上位バイト
                static constexpr const std::uint16_t kHighByte = 0xFF00;
                /// 下位バイト
                static constexpr const std::uint16_t kLowByte = 0x00FF;
                /// UART/USARTペリフェラル
                USART_TypeDef *_usart;
                /// 前回取得時のボタンの状態
                ButtonAssignment _lastButtonState;
                /// アナログスティックの状態
                AnalogState _as;
                /// バリデーション済みの受信データ
                std::array<std::uint8_t, SBDBT_RECEIVE_SIZE> _processedReceiveData;

                /**
                 * ボタンの状態を取得
                 * @param lastButtonState 前回取得時のボタンの状態
                 * @param isPush 押されているか
                 * @return ボタンの状態
                 */
                static ButtonState identifyButtonState(ButtonState lastButtonState, bool isPush);
            };
        }// namespace LowLayer
    }    // namespace v2
}// namespace LibMecha

#endif// _LIBMECHA_SBDBT_HH_
