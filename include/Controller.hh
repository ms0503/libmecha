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

#pragma once

#include "CanMotor.hh"
#include "LowLayer/SBDBT.hh"
#include <cmath>
#include <cstdint>
#include <functional>
#include <map>
#include <string>

namespace LibMecha {
    /// スティック入力の最大値
    constexpr const std::int8_t STICK_MAX = 63;

    /// コントローラー入力変換クラス
    class Controller {
    public:
        /// スティックの角度(右0・反時計回り・rad・-π < x <= π)
        struct StickTheta {
            /// 左スティック
            float left;
            /// 右スティック
            float right;
        };

        /// モーターの制御方式
        enum class MotorControlType : std::uint8_t {
            /// 三角関数
            TRIANGLE,
            /// ベクトル
            VECTOR
        };

        /**
         * コンストラクタ
         */
        explicit Controller(MotorControlType type);
        /**
         * デストラクタ
         */
        ~Controller();
        /**
         * スティック入力の取得
         * @return スティック入力
         */
        [[nodiscard]] inline LowLayer::SBDBT::AnalogState getStick() const {
            return _sbdbt.getAnalogState();
        }
        /**
         * スティック入力を基にしたモーター信号の生成
         * @param index モーターの通し番号(右を基準に反時計回り)
         * @return モーター信号
         */
        [[nodiscard]] std::int32_t stickToMotor(std::uint8_t index) const;
        /**
         * 初期化
         * @param deadZones デッドゾーンのリスト
         */
        inline void init(const std::int32_t deadZones[4]) {
            std::array<std::int32_t, 4> deadZones1 {};
            std::copy(deadZones, deadZones + 4, deadZones1.begin());
            init(deadZones1);
        }
        /**
         * 初期化
         * @param deadZones デッドゾーンのリスト
         */
        inline void init(const std::array<std::int32_t, 4> deadZones) {
            _sbdbt.init();
            setDeadZones(deadZones);
        }
        /**
         * 押されているかの取得
         * @param button ボタン
         * @return 押されているか
         */
        static inline bool isPush(LowLayer::SBDBT::ButtonState button) {
            return button == LowLayer::SBDBT::ButtonState::kPush;
        }
        /**
         * 押された瞬間かの取得
         * @param button ボタン
         * @return 押された瞬間か
         */
        static inline bool isPushEdge(LowLayer::SBDBT::ButtonState button) {
            return button == LowLayer::SBDBT::ButtonState::kPushEdge;
        }
        /**
         * 離れているかの取得
         * @param button ボタン
         * @return 離れているか
         */
        static inline bool isRelease(LowLayer::SBDBT::ButtonState button) {
            return button == LowLayer::SBDBT::ButtonState::kRelease;
        }
        /**
         * 離された瞬間かの取得
         * @param button ボタン
         * @return 離された瞬間か
         */
        static inline bool isReleaseEdge(LowLayer::SBDBT::ButtonState button) {
            return button == LowLayer::SBDBT::ButtonState::kReleaseEdge;
        }
        /**
         * 受信データのチェック
         * @param receiveData 受信データ
         * @return 妥当であるか
         */
        inline bool receiveCheck(const std::uint8_t receiveData[LowLayer::SBDBT_RECEIVE_SIZE]) {
            std::array<std::uint8_t, LowLayer::SBDBT_RECEIVE_SIZE> receiveData1 {};
            std::copy(receiveData, receiveData + LowLayer::SBDBT_RECEIVE_SIZE, receiveData1.begin());
            return _sbdbt.receiveCheck(receiveData1);
        }
        /**
         * 受信データのチェック
         * @param receiveData 受信データ
         * @return 妥当であるか
         */
        inline bool receiveCheck(const std::array<std::uint8_t, LowLayer::SBDBT_RECEIVE_SIZE> receiveData) {
            return _sbdbt.receiveCheck(receiveData);
        }
        /**
         * コントローラー入力の取得
         * @param receiveData 受信データ
         * @return ボタンアサイン
         */
        inline LowLayer::SBDBT::ButtonAssignment &receiveProcessing(const std::uint8_t receiveData[LowLayer::SBDBT_RECEIVE_SIZE]) {
            std::array<std::uint8_t, LowLayer::SBDBT_RECEIVE_SIZE> receiveData1 {};
            std::copy(receiveData, receiveData + LowLayer::SBDBT_RECEIVE_SIZE, receiveData1.begin());

            return receiveProcessing(receiveData1);
        }
        /**
         * コントローラー入力の取得
         * @param receiveData 受信データ
         * @return ボタンアサイン
         */
        inline LowLayer::SBDBT::ButtonAssignment &receiveProcessing(const std::array<std::uint8_t, LowLayer::SBDBT_RECEIVE_SIZE> receiveData) {
            if(_sbdbt.receiveCheck(receiveData)) _bs = _sbdbt.receiveProcessing();
            return _bs;
        }
        /**
         * スティックのデッドゾーンの取得
         * @return スティックのデッドゾーン
         */
        [[nodiscard]] inline std::array<std::int32_t, 4> getDeadZones() const {
            return _deadZones;
        }
        /**
         * スティックのデッドゾーンの設定
         * @param deadZones スティックのデッドゾーン
         */
        inline void setDeadZones(const std::int32_t deadZones[4]) {
            std::array<std::int32_t, 4> deadZones1 {};
            std::copy(deadZones, deadZones + 4, deadZones1.begin());
            setDeadZones(deadZones1);
        }
        /**
         * スティックのデッドゾーンの設定
         * @param deadZones スティックのデッドゾーン
         */
        inline void setDeadZones(const std::array<std::int32_t, 4> deadZones) {
            std::copy(deadZones.begin(), deadZones.end(), _deadZones.begin());
        }

    private:
        /// SBDBTクラスのインスタンス
        LowLayer::SBDBT _sbdbt;
        /// ボタンアサイン
        LowLayer::SBDBT::ButtonAssignment _bs;
        /// スティックのデッドゾーン
        std::array<std::int32_t, 4> _deadZones;
        /// モーターの制御方式
        MotorControlType _type;

        /**
         * スティック入力による角度の導出
         * @param x スティック横方向
         * @param y スティック縦方向
         * @return 角度(rad)
         */
        static inline float stickToTheta(const float x, const float y) {
            return std::atan2(y, x);
        }
        /**
         * 左右スティック入力による各角度の導出
         * @param leftX 左スティック横方向
         * @param leftY 左スティック縦方向
         * @param rightX 右スティック横方向
         * @param rightY 右スティック縦方向
         * @return 左右スティックの角度(rad)
         */
        static inline StickTheta sticksToTheta(const float leftX, const float leftY, const float rightX, const float rightY) {
            return {
                .left = stickToTheta(leftX, leftY),
                .right = stickToTheta(rightX, rightY)
            };
        }
    };
} // namespace LibMecha
