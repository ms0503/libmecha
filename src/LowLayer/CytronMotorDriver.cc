/*
 * CytronMotorDriver.cc
 *
 *  Created on: 23/05/23
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

#if (!defined DISABLE_PERIPHERAL && !defined DISABLE_CYTRONMOTORDRIVER)

#include "LowLayer/CytronMotorDriver.hh"

namespace LibMecha::LowLayer {
    void CytronMotorDriver::init() {
    }

    bool CytronMotorDriver::setDuty(std::int32_t duty) {

    }
}

#endif // (!defined DISABLE_PERIPHERAL && !defined DISABLE_CYTRONMOTORDRIVER)
