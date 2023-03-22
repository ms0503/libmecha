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
        constexpr const int8_t STICK_MAX = 63;

        /// コントローラー入力変換クラス
        class Controller {
        public:
            /// スティックの角度(右0・反時計回り・rad)
            struct StickTheta {
                /// 左スティック
                float left;
                /// 右スティック
                float right;
            };

            /**
             * コンストラクタ
             */
            explicit Controller();
            /**
             * デストラクタ
             */
            ~Controller();
            inline LowLayer::SBDBT::AnalogState getStick() {
                return _sbdbt.getAnalogState();
            }
            /**
             * スティック入力を基にしたモーター信号の生成
             * @param index モーターの通し番号(右を基準に反時計回り)
             * @return モーター信号
             */
            std::int32_t stickToMotor(std::uint8_t index);
            /**
             * 初期化
             */
            inline void init(const std::int32_t deadZones[4]) {
                _sbdbt.init();
                setDeadZones(deadZones);
            }
            /**
             * 押されているかの取得
             * @param button ボタン
             * @return 押されているか
             */
            inline bool isPush(LowLayer::SBDBT::ButtonState button) {
                return button == LowLayer::SBDBT::ButtonState::kPush;
            }
            /**
             * 押された瞬間かの取得
             * @param button ボタン
             * @return 押された瞬間か
             */
            inline bool isPushEdge(LowLayer::SBDBT::ButtonState button) {
                return button == LowLayer::SBDBT::ButtonState::kPushEdge;
            }
            /**
             * 離れているかの取得
             * @param button ボタン
             * @return 離れているか
             */
            inline bool isRelease(LowLayer::SBDBT::ButtonState button) {
                return button == LowLayer::SBDBT::ButtonState::kRelease;
            }
            /**
             * 離された瞬間かの取得
             * @param button ボタン
             * @return 離された瞬間か
             */
            inline bool isReleaseEdge(LowLayer::SBDBT::ButtonState button) {
                return button == LowLayer::SBDBT::ButtonState::kReleaseEdge;
            }
            inline bool receiveCheck(const std::uint8_t receiveData[LowLayer::SBDBT_RECEIVE_SIZE]) {
                std::uint8_t sizedReceiveData[LowLayer::SBDBT_RECEIVE_SIZE];
                std::copy(receiveData, receiveData + LowLayer::SBDBT_RECEIVE_SIZE, sizedReceiveData);
                return _sbdbt.receiveCheck(sizedReceiveData);
            }
            /**
             * コントローラー入力の取得
             * @param receiveData 受信データ
             * @return ボタンアサイン
             */
            inline LowLayer::SBDBT::ButtonAssignment &receiveProcessing(const std::uint8_t receiveData[LowLayer::SBDBT_RECEIVE_SIZE]) {
                std::uint8_t sizedReceiveData[LowLayer::SBDBT_RECEIVE_SIZE];
                std::copy(receiveData, receiveData + LowLayer::SBDBT_RECEIVE_SIZE, sizedReceiveData);
                if(_sbdbt.receiveCheck(sizedReceiveData)) _bs = _sbdbt.receiveProcessing();

                return _bs;
            }
            /**
             * スティックのデッドゾーンの取得
             * @return スティックのデッドゾーン
             */
            inline std::int32_t *getDeadZones() {
                return _deadZones;
            }
            /**
             * スティックのデッドゾーンの設定
             * @param deadZones スティックのデッドゾーン
             */
            inline void setDeadZones(const std::int32_t deadZones[4]) {
                std::memcpy(_deadZones, deadZones, 4);
            }

        private:
            /// SBDBTクラスのインスタンス
            LowLayer::SBDBT _sbdbt;
            /// ボタンアサイン
            LowLayer::SBDBT::ButtonAssignment _bs;
            /// スティックのデッドゾーン
            std::int32_t _deadZones[4];

            /**
             * スティック入力による角度の導出
             * @param x スティック横方向
             * @param y スティック縦方向
             * @return 角度(rad)
             */
            static float stickToTheta(float x, float y);
            /**
             * 左右スティック入力による各角度の導出
             * @param leftX 左スティック横方向
             * @param leftY 左スティック縦方向
             * @param rightX 右スティック横方向
             * @param rightY 右スティック縦方向
             * @return 左右スティックの角度(rad)
             */
            static StickTheta sticksToTheta(float leftX, float leftY, float rightX, float rightY);
        };
    } // namespace v2
} // namespace LibMecha

#endif // _LIBMECHA_CONTROLLER_HH_
