/*
 * Encoder.cc
 *
 *  Created on: 2023/06/22
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

#if (defined USE_FULL_LL_DRIVER && !defined DISABLE_TIM && !defined DISABLE_ENCODER)

#include <LowLayer/Encoder.hh>

namespace LibMecha::LowLayer {
    Encoder::Encoder(TIM_TypeDef *tim, std::uint16_t ppr):
        _tim(tim), _ppr(ppr), _cpr(ppr * 4) {
    }

    void Encoder::update() {
        const std::uint32_t tempRot = LL_TIM_GetCounter(_tim);
        _deltaRot = reinterpret_cast<const std::int32_t &>(tempRot);
        LL_TIM_SetCounter(_tim, 0);
        _rot += _deltaRot;
    }
}

#endif
