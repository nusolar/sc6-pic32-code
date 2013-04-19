//
//  array.h
//  nusolar_lib
//
//  Created by Al Chandel on 4/19/13.
//  Copyright (c) 2013 Alex Chandel. All rights reserved.
//

#ifndef __nusolar_lib__array__
#define __nusolar_lib__array__

#include "compiler.h"
#include <cstddef>
#include <array>

namespace nu {
	template <class _Tp, size_t _Size>
	struct Enum: public std::array<_Tp, _Size> {
		size_t len; // WARNING: MUST DEFAULT INITIALIZE
		size_t ALWAYSINLINE enumerate(_Tp &&rvalue) {
			(*this)[len] = rvalue;
			return len++;
		}
	};
}

#endif /* defined(__nusolar_lib__array__) */
