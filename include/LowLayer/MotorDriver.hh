/*
 * MotorDriver.hh
 *
 *  Created on: 2019/06/29
 *      Author: youda, ms0503
 *
 *  This file is part of libmecha.
 *
 *  libmecha is free software: you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 *
 *  libmecha is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License along with libmecha. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef _LIBMECHA_MOTORDRIVER_HH_
#define _LIBMECHA_MOTORDRIVER_HH_

#include "Can.hh"
#include "stm32f4xx_hal.h"
#include <array>
#include <cassert>
#include <cstring>

namespace LibMecha {
    inline namespace v1 {
        namespace LowLayer {
            class MotorDriver {
            public:
                /// モータドライバのコマンドを定義
                enum class DriveCommand : uint8_t {
                    kDuty          = 0x00,
                    kPID           = 0x01,
                    kSetParamP     = 0x02,
                    kSetParamI     = 0x03,
                    kSetParamD     = 0x04,
          canTxHeaderStruct          kSetParamLIMIT = 0x05,
                    kSetParamPPM   = 0x06,
                    kEmergency     = 0x80
                };

                MotorDriver(const CAN_HandleTypeDef &canHandle, const Can &can);
                ~MotorDriver();
                void init();
                void pidInit(uint8_t address, float kp, float ki, float kd, uint32_t maxRpm, uint32_t kppm);
                Can &getCan();
                CAN_HandleTypeDef &getCanHandle();
                bool setTargetRPM(uint8_t address, int32_t targetRpm);
                bool setDuty(uint8_t address, int32_t duty);
                bool setParameter(uint8_t address, DriveCommand mode, float fparamValue);
                bool setParameter(uint8_t address, DriveCommand mode, uint32_t uparamValue);
                //    bool allUpdate(uint8_t firstAddr, uint8_t cmd, uint8_t &sendData);
                bool Emergency(uint8_t address);

            private:
                CAN_HandleTypeDef _hcan;
                Can _can;
                /// MD1枚のみのアップデート
                bool updateDataSend(uint8_t address, DriveCommand cmd, const uint8_t (&sendData)[4]);
            };
        }
    }
}

#endif// _LIBMECHA_MOTORDRIVER_HH_
