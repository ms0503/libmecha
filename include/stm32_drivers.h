/*
 * stm32_drivers.h
 *
 *  Created on: 2023/06/29
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

#ifdef STM32F3
    #ifdef USE_HAL_DRIVER
        #include <stm32f3xx_hal.h>
    #endif
    #ifdef USE_FULL_LL_DRIVER
        #include <stm32f3xx_ll_bus.h>
        #include <stm32f3xx_ll_cortex.h>
        #include <stm32f3xx_ll_dma.h>
        #include <stm32f3xx_ll_exti.h>
        #ifndef DISABLE_GPIO
            #include <stm32f3xx_ll_gpio.h>
        #endif
        #include <stm32f3xx_ll_pwr.h>
        #include <stm32f3xx_ll_rcc.h>
        #include <stm32f3xx_ll_system.h>
        #ifndef DISABLE_TIM
            #include <stm32f3xx_ll_tim.h>
        #endif
        #ifndef DISABLE_USART
            #include <stm32f3xx_ll_usart.h>
        #endif
        #include <stm32f3xx_ll_utils.h>
    #endif
#elifdef STM32F4
    #ifdef USE_HAL_DRIVER
        #include <stm32f4xx_hal.h>
    #endif
    #ifdef USE_FULL_LL_DRIVER
        #include <stm32f4xx_ll_bus.h>
        #include <stm32f4xx_ll_cortex.h>
        #include <stm32f4xx_ll_dma.h>
        #include <stm32f4xx_ll_exti.h>
        #ifndef DISABLE_GPIO
            #include <stm32f4xx_ll_gpio.h>
        #endif
        #include <stm32f4xx_ll_pwr.h>
        #include <stm32f4xx_ll_rcc.h>
        #include <stm32f4xx_ll_system.h>
        #ifndef DISABLE_TIM
            #include <stm32f4xx_ll_tim.h>
        #endif
        #ifndef DISABLE_USART
            #include <stm32f4xx_ll_usart.h>
        #endif
        #include <stm32f4xx_ll_utils.h>
    #endif
#endif
