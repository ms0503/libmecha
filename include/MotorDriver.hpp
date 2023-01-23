/*
 * MotorDriver.hpp
 *
 *  Created on: 2019/06/29
 *      Author: youda
 *
 *  This file is part of libmecha.
 *
 *  libmecha is free software: you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 *
 *  libmecha is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License along with libmecha. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef MYLIBINC_MOTORDRIVER_HPP_
#define MYLIBINC_MOTORDRIVER_HPP_

#include <array>
#include <cstring>
#include <cassert>

#include "stm32f4xx_hal.h"

#include "Can.hpp"



class MotorDriver{
public:
	/*モータドライバのコマンドを定義*/
	enum class drive_command
	{
		kDuty			= 0x00,
		kPID			= 0x01,
		kSetParamP		= 0x02,
		kSetParamI		= 0x03,
		kSetParamD		= 0x04,
		kSetParamLIMIT	= 0x05,
		kSetParamPPM	= 0x06,
		kEmergency      = 0x80
	};

	MotorDriver();
	~MotorDriver();
	void Init(const CAN_HandleTypeDef &can_handle);
	void PIDInit(const uint8_t address, const float kp, const float ki, const float kd, const uint32_t max_rpm, const uint32_t kppm);
	bool setTargetRPM(const uint8_t address, const int32_t target_rpm);
	bool setDuty(const uint8_t address, const int32_t duty);
	bool setParameter(const uint8_t address, const drive_command mode, const float fparam_value);
	bool setParameter(const uint8_t address, const drive_command mode, const uint32_t uparam_value);
//	bool allUpdate(const uint8_t first_address, const uint8_t cmd, const uint8_t &send_data);
	bool Emergency(const uint8_t address);

private:
//	CAN_HandleTypeDef hcan_;
	Can can_;
	bool updateDataSend(const uint8_t address, const drive_command cmd, const uint8_t (&send_data)[4]);  //MD1枚のみのアップデート

};



#endif /* MYLIBINC_MOTORDRIVER_HPP_ */
