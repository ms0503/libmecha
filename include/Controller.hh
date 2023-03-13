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

#ifndef MYLIBINC_CONTROLLER_HH_
#define MYLIBINC_CONTROLLER_HH_

#include "LowLayer/SBDBT.hh"
#include "Motor.hh"
#include <cstdint>
#include <functional>
#include <map>
#include <string>

namespace LibMecha {
    inline namespace v1 {
        using namespace LowLayer;

        /// スティックの角度(右0・反時計回り・rad)
        typedef struct {
            long double left;
            long double right;
        } StickTheta;

        /// コントローラー入力変換クラス
        class Controller {
        public:
            explicit Controller(const UART_HandleTypeDef &uartHandle);
            ~Controller();
            /**
             * スティック入力を基にモーター信号を生成する
             * @return モーター信号
             */
            MotorState stickToMotor();
            void init();
            bool isPush(SBDBT::ButtonState button);
            bool isPushEdge(SBDBT::ButtonState button);
            bool isRelease(SBDBT::ButtonState button);
            bool isReleaseEdge(SBDBT::ButtonState button);
            void receiveProcessing(const UART_HandleTypeDef *uartHandle, const std::function<void(const SBDBT::ButtonAssignment &bs)> &callback);

        private:
            UART_HandleTypeDef _uartHandle;
            SBDBT _sbdbt;
            SBDBT::ButtonAssignment _bs;
            uint8_t _sbdbtData[SBDBT_RECEIVE_SIZE];

            /// スティックのデッドゾーン
            static const std::map<EnumMotor, int8_t> DEAD_ZONES;

            /**
             * スティック入力から角度を導出する
             * @param x スティック横方向
             * @param y スティック縦方向
             * @return 角度(rad)
             */
            static long double stickToTheta(int8_t x, int8_t y);
            /**
             * 左右スティック入力から各角度を導出する
             * @param leftX 左スティック横方向
             * @param leftY 左スティック縦方向
             * @param rightX 右スティック横方向
             * @param rightY 右スティック縦方向
             * @return 左右スティックの角度(rad)
             */
            static StickTheta sticksToTheta(int8_t leftX, int8_t leftY, int8_t rightX, int8_t rightY);
        };
    }
}

#endif // MYLIBINC_CONTROLLER_HH_
