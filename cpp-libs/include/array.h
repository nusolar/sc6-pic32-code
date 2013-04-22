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
	/**
	 * This template class stores its data on the stack, WHILE ALSO returning
	 * incrementing values as enumerate() is called, to facilitate enumeration.
	 */
	template <class _Tp, size_t _Size>
	struct Enum: public std::array<_Tp, _Size> {
		size_t len; // WARNING: MUST DEFAULT INITIALIZE
		size_t ALWAYSINLINE enumerate(_Tp &&rvalue) {
			if (unlikely(len == this->size()))
				return len-1;
			(*this)[len] = rvalue;
			return len++;
		}
	};
}

#endif /* defined(__nusolar_lib__array__) */
