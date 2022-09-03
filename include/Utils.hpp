/*
   This is a project that is dedicated to downloading Minecraft Mods from CurseForge using their API
    Copyright (C) <2022>  Timothy Hutchins

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

*/

/*
 * Utils.hpp
 *
 *  Created on: Jun 18, 2022
 *      Author: thetimbrick
 */

#ifndef INCLUDE_UTILS_HPP_
#define INCLUDE_UTILS_HPP_

template <typename T>
class Utils {
	public:

		static const T changeRange(T OldValue, T OldMax, T OldMin, T NewMax, T NewMin) {
			T oldRange = OldMax - OldMin;
		    double oldNorm = (OldValue - OldMin) / (oldRange * 1.0);
		    T newRange = NewMax - NewMin;
		    T newValue = (T)((newRange * oldNorm) + NewMin);
		    return newValue;
		}

	private:

};



#endif /* INCLUDE_UTILS_HPP_ */
