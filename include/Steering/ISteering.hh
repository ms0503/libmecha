/*
 * ISteering.hh
 *
 *  Created on: 2023/05/22
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

#include "Controller.hh"
#include "MiddleLayer/IMotorDriver.hh"
#include <concepts>
#include <cstdint>
#include <vector>

namespace LibMecha::Steering {
    /// 最高速度
    extern std::int32_t maxSpeed;

    /**
     * 足回りインターフェース
     * @tparam MotorDriver モータードライバー
     */
    template<class MotorDriver> requires std::derived_from<MotorDriver, LowLayer::IMotorDriver> class ISteering {
    public:
        /**
         * コンストラクター
         */
        explicit ISteering() = 0;

        /**
         * デストラクター
         */
        virtual ~ISteering() = 0;

        /**
         * 初期化
         */
        virtual inline void init() {
            _md.init();
        }

        /**
         * モーター信号の手動更新
         * @param index モータードライバー配列のインデックス
         * @param target 目標パルス毎秒
         */
        virtual inline void update(const std::size_t index, const std::int32_t target) const {
            _md.at(index).setTarget(target);
        }

        /**
         * 極座標入力によるモーター信号の自動更新
         * @param r 動径
         * @param theta 偏角
         */
        virtual void polarInput(float r, LibMecha::Controller<ISteering>::StickTheta theta) const = 0;

        /**
         * 前進
         * @param speed 速度
         */
        virtual void forward(std::int32_t speed) const = 0;

        /**
         * 後退
         * @param speed 速度
         */
        virtual void backward(std::int32_t speed) const = 0;

        /**
         * 左平行移動
         * @param speed 速度
         */
        virtual void left(std::int32_t speed) const = 0;

        /**
         * 右平行移動
         * @param speed 速度
         */
        virtual void right(std::int32_t speed) const = 0;

        /**
         * 左回転
         * @param speed 速度
         */
        virtual void turnLeft(std::int32_t speed) const = 0;

        /**
         * 右回転
         * @param speed 速度
         */
        virtual void turnRight(std::int32_t speed) const = 0;

    protected:
        /// モータードライバー配列
        const std::vector<MotorDriver> _md {};
    };
}
