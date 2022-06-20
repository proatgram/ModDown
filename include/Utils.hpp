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
