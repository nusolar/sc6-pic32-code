#include "nu/compiler.h"

#ifndef NUPP_STACK_HPP
#define	NUPP_STACK_HPP

namespace nu {
	template <typename T, size_t N>
	class stack {
		T array[N];
	public:
		ALWAYSINLINE T& operator[] (const size_t index) {
			return array[index];
		}
		ALWAYSINLINE T operator[] (const size_t index) const {
			return array[index];
		}
		ALWAYSINLINE operator void *() {
			return (void *)array;
		}
		ALWAYSINLINE operator const void *() const {
			return (const void *)array;
		}
	};
}

#endif	/* NUPP_STACK_HPP */
