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

#include <MiddleLayer/IMotorDriver.hh>
#include <Stick.hh>
#include <vector>

namespace LibMecha::Steering {
    /// 最高速度
    extern std::int32_t maxSpeed;

    /**
     * 足回りインターフェース
     */
    class ISteering {
    public:
        /**
         * コンストラクター
         */
        explicit ISteering();

        /**
         * デストラクター
         */
        virtual ~ISteering();

        /**
         * 初期化
         */
        virtual inline void init() {
            for(auto md : _md) md.init();
        }

        /**
         * モーター信号の手動更新
         * @param index モータードライバー配列のインデックス
         * @param target 目標パルス毎秒
         */
        virtual inline void update(const std::size_t index, const std::int32_t target) {
            _md.at(index).setTarget(target);
        }

        /**
         * 極座標入力によるモーター信号の自動更新
         * @param r 動径
         * @param theta 偏角
         */
        virtual void polarInput(float r, LibMecha::StickTheta theta);

        /**
         * 前進
         * @param speed 速度
         */
        virtual void forward(std::int32_t speed);

        /**
         * 後退
         * @param speed 速度
         */
        virtual void backward(std::int32_t speed);

        /**
         * 左平行移動
         * @param speed 速度
         */
        virtual void left(std::int32_t speed);

        /**
         * 右平行移動
         * @param speed 速度
         */
        virtual void right(std::int32_t speed);

        /**
         * 左回転
         * @param speed 速度
         */
        virtual void turnLeft(std::int32_t speed);

        /**
         * 右回転
         * @param speed 速度
         */
        virtual void turnRight(std::int32_t speed);

    protected:
        /// モータードライバー配列
        std::vector<MiddleLayer::IMotorDriver> _md {};
    };
}
