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
#include <cmath>
#include <concepts>
#include <cstdint>

namespace LibMecha::LowLayer {
    template<std::integral CNT_TYPE>
    class Encoder : public Peripheral {
    public:
        using Peripheral::Peripheral;

        explicit Encoder(TIM_TypeDef *tim, std::uint16_t ppr):
            _tim(tim), _ppr(ppr), _cpr(ppr * 4) {
        }

        [[nodiscard]] inline CNT_TYPE getRot() const {
            return _rot;
        }

        [[nodiscard]] inline CNT_TYPE getDeltaRot() const {
            return _deltaRot;
        }

        [[nodiscard]] inline float getDegree() const {
            return static_cast<float>(_rot) / static_cast<float>(_cpr) * 360.0f;
        }

        [[nodiscard]] inline float getRadian() const {
            return static_cast<float>(_rot) / static_cast<float>(_cpr) * static_cast<float>(M_PI) * 2.0f;
        }

        [[nodiscard]] inline std::int16_t getRevolution() const {
            return _deltaRot / _cpr;
        }

        inline void update() {
            const CNT_TYPE tempRot = LL_TIM_GetCounter(_tim);
            _deltaRot = reinterpret_cast<const CNT_TYPE &>(tempRot);
            LL_TIM_SetCounter(_tim, 0);
            _rot += _deltaRot;
        }

    private:
        TIM_TypeDef *const _tim;
        CNT_TYPE _rot;
        CNT_TYPE _deltaRot;
        std::uint16_t _ppr;
        std::uint16_t _cpr;
    };
}
