/*
 * Encoder.hh
 *
 *  Created on: 2023/05/30
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

#include "Peripheral.hh"
#include "stm32f4xx_ll_tim.h"
#include <cmath>
#include <cstdint>

namespace LibMecha::LowLayer {
    /// エンコーダー
    class Encoder : public Peripheral {
    public:
        using Peripheral::Peripheral;

        /**
         * コンストラクター
         * @param tim タイマー
         * @param ppr パルス毎回転(分解能)
         */
        explicit Encoder(TIM_TypeDef *tim, std::uint16_t ppr);

        /**
         * 累計カウントの取得
         * @return 累計カウント
         */
        [[nodiscard]] inline std::int32_t getRot() const {
            return _rot;
        }

        /**
         * 差分カウントの取得
         * @return 差分カウント
         */
        [[nodiscard]] inline std::int32_t getDeltaRot() const {
            return _deltaRot;
        }

        /**
         * 累計角度の取得
         * @return 累計角度[deg]
         */
        [[nodiscard]] inline float getDegree() const {
            return static_cast<float>(_rot) / static_cast<float>(_cpr) * 360.0f;
        }

        /**
         * 差分角度の取得
         * @return 差分角度[deg]
         */
        [[nodiscard]] inline float getDeltaDegree() const {
            return static_cast<float>(_deltaRot) / static_cast<float>(_cpr) * 360.0f;
        }

        /**
         * 累計角度の取得
         * @return 累計角度[rad]
         */
        [[nodiscard]] inline float getRadian() const {
            return static_cast<float>(_rot) / static_cast<float>(_cpr) * static_cast<float>(M_PI) * 2.0f;
        }

        /**
         * 差分角度の取得
         * @return 差分角度[rad]
         */
        [[nodiscard]] inline float getDeltaRadian() const {
            return static_cast<float>(_deltaRot) / static_cast<float>(_cpr) * static_cast<float>(M_PI) * 2.0f;
        }

        /**
         * 累計回転数の取得
         * @return 累計回転数
         */
        [[nodiscard]] inline std::int32_t getRevolution() const {
            return _rot / _cpr;
        }

        /**
         * 差分回転数の取得
         * @return 差分回転数
         */
        [[nodiscard]] inline std::int32_t getDeltaRevolution() const {
            return _deltaRot / _cpr;
        }

        /**
         * 値の更新
         */
        void update();

    private:
        /// タイマー
        TIM_TypeDef *const _tim;
        /// 累計カウンター
        std::int32_t _rot = 0;
        /// 差分カウンター
        std::int32_t _deltaRot = 0;
        /// パルス毎回転(分解能)
        std::uint16_t _ppr;
        /// カウント毎回転
        std::uint16_t _cpr;
    };
}
