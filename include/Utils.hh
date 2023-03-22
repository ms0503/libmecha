/*
 * Utils.hh
 *
 *  Created on: 2023/03/22
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

#ifndef _LIBMECHA_UTILS_HH_
#define _LIBMECHA_UTILS_HH_

#include <cstdlib>

namespace LibMecha {
    inline namespace v2 {
        namespace Utils {
            template<typename TYPE, std::size_t SIZE> inline std::size_t getArrayLength(const TYPE (&)[SIZE]) {
                return SIZE;
            }
        }
    }
}

#endif// _LIBMECHA_UTILS_HH_
