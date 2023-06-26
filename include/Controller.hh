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

#include "MiddleLayer/SBDBT.hh"
#include <array>
#include <cmath>
#include <cstdint>
#include <functional>
#include <map>
#include <string>

namespace LibMecha {
    /// スティック入力の最大値
    constexpr const std::int8_t STICK_MAX = 63;

    /// コントローラー
    template<class Steering> class Controller {
    public:
        /// スティックの角度(右0・反時計回り・rad・-π < x <= π)
        struct StickTheta {
            /// 左スティック
            float left;
            /// 右スティック
            float right;
        };

        /**
         * コンストラクター
         */
        explicit Controller(LowLayer::SBDBT &sbdbt);

        /**
         * デストラクター
         */
        ~Controller();

        /**
         * スティック入力の取得
         * @return スティック入力
         */
        [[nodiscard]] inline LowLayer::SBDBT::AnalogState getStick() {
            const LowLayer::SBDBT::AnalogState &as = _sbdbt.getAnalogState();
            return {
                .LX = _deadZones.at(0) < std::abs(as.LX) ? as.LX : static_cast<std::int8_t>(0),
                .LY = _deadZones.at(1) < std::abs(as.LY) ? as.LY : static_cast<std::int8_t>(0),
                .RX = _deadZones.at(2) < std::abs(as.RX) ? as.RX : static_cast<std::int8_t>(0),
                .RY = _deadZones.at(3) < std::abs(as.RY) ? as.RY : static_cast<std::int8_t>(0)
            };
        }

        /**
         * スティック入力を基にしたモーター信号の生成
         */
        void stickToSteering() const;

        /**
         * 初期化
         * @param deadZones デッドゾーンのリスト
         */
        inline void init(const std::int32_t (&deadZones)[4]) {
            std::array<std::int32_t, 4> deadZones1 {};
            std::copy(deadZones, deadZones + 4, deadZones1.begin());
            init(deadZones1);
        }

        /**
         * 初期化
         * @param deadZones デッドゾーンのリスト
         */
        inline void init(const std::array<std::int32_t, 4> &deadZones) {
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
        inline bool receiveCheck(const std::uint8_t (&receiveData)[LowLayer::SBDBT_RECEIVE_SIZE]) {
            std::array<std::uint8_t, LowLayer::SBDBT_RECEIVE_SIZE> receiveData1 {};
            std::copy(receiveData, receiveData + LowLayer::SBDBT_RECEIVE_SIZE, receiveData1.begin());

            return receiveCheck(receiveData1);
        }

        /**
         * 受信データのチェック
         * @param receiveData 受信データ
         * @return 妥当であるか
         */
        inline bool receiveCheck(std::array<std::uint8_t, LowLayer::SBDBT_RECEIVE_SIZE> &receiveData) {
            return _sbdbt.receiveCheck(receiveData);
        }

        /**
         * コントローラー入力の取得
         * @param receiveData 受信データ
         * @return ボタンアサイン
         */
        inline LowLayer::SBDBT::ButtonAssignment &receiveProcessing(const std::uint8_t (&receiveData)[LowLayer::SBDBT_RECEIVE_SIZE]) {
            std::array<std::uint8_t, LowLayer::SBDBT_RECEIVE_SIZE> receiveData1 {};
            std::copy(receiveData, receiveData + LowLayer::SBDBT_RECEIVE_SIZE, receiveData1.begin());

            return receiveProcessing(receiveData1);
        }

        /**
         * コントローラー入力の取得
         * @param receiveData 受信データ
         * @return ボタンアサイン
         */
        inline LowLayer::SBDBT::ButtonAssignment &receiveProcessing(std::array<std::uint8_t, LowLayer::SBDBT_RECEIVE_SIZE> &receiveData) {
            if(_sbdbt.receiveCheck(receiveData)) _sbdbt.receiveProcessing(_bs);
            return _bs;
        }

        /**
         * スティックのデッドゾーンの取得
         * @return スティックのデッドゾーン
         */
        [[nodiscard]] inline std::array<std::int32_t, 4> &getDeadZones() {
            return _deadZones;
        }
        /**
         * スティックのデッドゾーンの設定
         * @param deadZones スティックのデッドゾーン
         */
        inline void setDeadZones(const std::int32_t (&deadZones)[4]) {
            std::array<std::int32_t, 4> deadZones1 {};
            std::copy(deadZones, deadZones + 4, deadZones1.begin());
            setDeadZones(deadZones1);
        }
        /**
         * スティックのデッドゾーンの設定
         * @param deadZones スティックのデッドゾーン
         */
        inline void setDeadZones(const std::array<std::int32_t, 4> &deadZones) {
            std::copy(deadZones.begin(), deadZones.end(), _deadZones.begin());
        }

    private:
        /// SBDBTクラスのインスタンス
        LowLayer::SBDBT &_sbdbt;
        /// ボタンアサイン
        LowLayer::SBDBT::ButtonAssignment _bs;
        /// スティックのデッドゾーン
        std::array<std::int32_t, 4> _deadZones;
        /// 足回り
        const Steering &_steering;

        /**
         * 左右スティック入力による各角度の導出
         * @return 左右スティックの角度(rad)
         */
        [[nodiscard]] inline StickTheta sticksToTheta() const {
            const LowLayer::SBDBT::AnalogState &as = _sbdbt.getAnalogState();
            return {
                .left = std::atan2(_deadZones.at(1) < std::abs(as.LY) ? static_cast<float>(as.LY) : 0.0f, _deadZones.at(0) < std::abs(as.LX) ? static_cast<float>(as.LX) : 0.0f),
                .right = std::atan2(_deadZones.at(3) < std::abs(as.RY) ? static_cast<float>(as.RY) : 0.0f, _deadZones.at(2) < std::abs(as.RX) ? static_cast<float>(as.RX) : 0.0f)
            };
        }
    };
} // namespace LibMecha
