/*
 * Controller.hh
 *
 *  Created on: 2023/03/08
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

#ifndef _LIBMECHA_CONTROLLER_HH_
#define _LIBMECHA_CONTROLLER_HH_

#include "LowLayer/SBDBT.hh"
#include "Motor.hh"
#include <cstdint>
#include <functional>
#include <map>
#include <string>

namespace LibMecha {
    inline namespace v2 {
        namespace LMLL = LowLayer;

        /// コントローラー入力変換クラス
        class Controller {
        public:
            /// スティックの角度(右0・反時計回り・rad)
            struct StickTheta {
                /// 左スティック
                long double left;
                /// 右スティック
                long double right;
            };

            /**
             * コンストラクタ
             * @param usart UART/USARTペリフェラル
             */
            explicit Controller(USART_TypeDef *usart);
            /**
             * デストラクタ
             */
            ~Controller();
            /**
             * スティック入力を基にしたモーター信号の生成
             * @return モーター信号
             */
            Motor::State stickToMotor();
            /**
             * 初期化
             */
            void init();
            /**
             * 押されているかの取得
             * @param button ボタン
             * @return 押されているか
             */
            bool isPush(LMLL::SBDBT::ButtonState button);
            /**
             * 押された瞬間かの取得
             * @param button ボタン
             * @return 押された瞬間か
             */
            bool isPushEdge(LMLL::SBDBT::ButtonState button);
            /**
             * 離れているかの取得
             * @param button ボタン
             * @return 離れているか
             */
            bool isRelease(LMLL::SBDBT::ButtonState button);
            /**
             * 離された瞬間かの取得
             * @param button ボタン
             * @return 離された瞬間か
             */
            bool isReleaseEdge(LMLL::SBDBT::ButtonState button);
            /**
             * コントローラー入力の取得
             * @param callback コントローラー入力のハンドラ
             */
            void receiveProcessing(const std::uint8_t (&receiveData)[LMLL::SBDBT_RECEIVE_SIZE], const std::function<void(const LMLL::SBDBT::ButtonAssignment &bs)> &callback);

        private:
            /// UART/USARTのペリフェラル
            USART_TypeDef *_usart;
            /// SBDBTクラスのインスタンス
            LMLL::SBDBT _sbdbt;
            /// ボタンアサイン
            LMLL::SBDBT::ButtonAssignment _bs;

            /// スティックのデッドゾーン
            static const std::map<EnumMotor, int8_t> DEAD_ZONES;

            /**
             * スティック入力による角度の導出
             * @param x スティック横方向
             * @param y スティック縦方向
             * @return 角度(rad)
             */
            static long double stickToTheta(int8_t x, int8_t y);
            /**
             * 左右スティック入力による各角度の導出
             * @param leftX 左スティック横方向
             * @param leftY 左スティック縦方向
             * @param rightX 右スティック横方向
             * @param rightY 右スティック縦方向
             * @return 左右スティックの角度(rad)
             */
            static StickTheta sticksToTheta(int8_t leftX, int8_t leftY, int8_t rightX, int8_t rightY);
        };
    }// namespace v2
}// namespace LibMecha

#endif// _LIBMECHA_CONTROLLER_HH_
