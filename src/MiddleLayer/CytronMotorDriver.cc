/*
 * CytronMotorDriver.cc
 *
 *  Created on: 2023/05/23
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

#if (!defined DISABLE_LL && !defined DISABLE_TIM && !defined DISABLE_PERIPHERAL && !defined DISABLE_GPIO && !defined DISABLE_ENCODER && !defined DISABLE_MOTORDRIVER && !defined DISABLE_CYTRONMOTORDRIVER)

#include "LowLayer/Encoder.hh"
#include "LowLayer/GPIO.hh"
#include "MiddleLayer/CytronMotorDriver.hh"
#include "MiddleLayer/IMotorDriver.hh"
#include "stm32f4xx_ll_tim.h"
#include <cmath>
#include <cstdint>

namespace LibMecha::MiddleLayer {
    CytronMotorDriver::CytronMotorDriver(const std::int32_t maxSpeed, LowLayer::Encoder &encoder, TIM_TypeDef *const tim, const std::uint32_t timCh, LowLayer::GPIO::Pin &dirPin):
        IMotorDriver(maxSpeed, encoder), _tim(tim), _timCh(timCh), _dir(dirPin) {
    }

    void CytronMotorDriver::init() {
        LL_TIM_CC_EnableChannel(_tim, _timCh);
        LL_TIM_EnableCounter(_tim);
    }

    bool CytronMotorDriver::setTarget(std::int32_t targetSpeed) {
        const std::uint32_t uTargetSpeed = std::abs(targetSpeed);
        const bool dir = targetSpeed != uTargetSpeed;
        const std::uint16_t nowSpeed = std::abs(_encoder.getDeltaRot()) * 1000;
        const float kn = 4.0f;
        const float gainC1 = kn + 1.0f / kn;
        const float gainC2 = 1.0f / kn;
        const std::uint32_t duty = uTargetSpeed * gainC1 - nowSpeed * gainC2;
        switch(_timCh) {
            case LL_TIM_CHANNEL_CH1:
                LL_TIM_OC_SetCompareCH1(_tim, duty);
                break;
            case LL_TIM_CHANNEL_CH2:
                LL_TIM_OC_SetCompareCH2(_tim, duty);
                break;
            case LL_TIM_CHANNEL_CH3:
                LL_TIM_OC_SetCompareCH3(_tim, duty);
                break;
            case LL_TIM_CHANNEL_CH4:
                LL_TIM_OC_SetCompareCH4(_tim, duty);
                break;
        }
        if(dir) _dir.left();
        else _dir.right();
        return true;
    }

    CytronMotorDriver::Direction::Direction(GPIO::Pin &pin):
        GPIO({ pin }) {
    }
}

#endif
