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

#include "LowLayer/Peripheral.hh"
#include "stm32f4xx_ll_tim.h"
#include <concepts>
#include <cstdint>

namespace LibMecha::LowLayer {
    template<std::integral CNT_TYPE>
    class Encoder : public Peripheral {
    public:
        using Peripheral::Peripheral;

        explicit Encoder(TIM_TypeDef *tim):
            tim(tim) {
        }

        inline CNT_TYPE getRot() const {
            return rot;
        }

        inline CNT_TYPE getDeltaRot() const {
            return deltaRot;
        }

        inline void update() {
            const CNT_TYPE tempRot = LL_TIM_GetCounter(tim);
            deltaRot = reinterpret_cast<const CNT_TYPE &>(tempRot);
            LL_TIM_SetCounter(tim, 0);
            rot += deltaRot;
        }

    private:
        TIM_TypeDef *const tim;
        CNT_TYPE rot;
        CNT_TYPE deltaRot;
    };
}
