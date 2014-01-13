#ifndef NUPP_ENUM_HPP
#define NUPP_ENUM_HPP 1

#include "nu/compiler.h" // MAXIMUM WARNING MAXIMUM ERROR WTF. MUST BE INCLUDED FIRST
#if defined(__DEBUG)
#undef inline
#endif

#include <cstddef>

namespace nu {
	/**
	 * This template class stores its data on the stack, WHILE ALSO returning
	 * incrementing values as enumerate() is called, to facilitate enumeration.
	 */
	template <class _Tp, size_t _Size>
	struct Enum {
		size_t len;
		_Tp _array[_Size];

		ALWAYSINLINE size_t size() {return _Size;}
		_Tp& operator[] (const size_t index) {
		    return _array[index];
		}
		size_t enumerate(_Tp &&rvalue) {
			if (unlikely(len == _Size))
				return len-1;
			(*this)[len] = rvalue;
			return len++;
		}

		Enum(): len(0) {}
	};
}

#endif
