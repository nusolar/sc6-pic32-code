/*
 * File:   lowpassfilter.hpp
 * Author: alex
 *
 * Created on June 27, 2013, 7:22 AM
 */

#ifndef LOWPASSFILTER_HPP
#define	LOWPASSFILTER_HPP

#include "nu/compiler.h"

namespace nu {
	struct LowPassFilter {
		float _x, _alpha;
		LowPassFilter(float x0, float alpha): _x(x0), _alpha(alpha) {}

		float next(float xp) {
			return (_x = _alpha*_x + (1-_alpha)*xp);
		}
	};
}

#endif	/* LOWPASSFILTER_HPP */

