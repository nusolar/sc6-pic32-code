#include "nu/compiler.h"

#ifndef NUPP_STACK_HPP
#define	NUPP_STACK_HPP

namespace nu {
	template <typename T, size_t N>
	class Array {
		T array[N];
	public:
		/** @warning NO BOUNDS CHECKING */
		ALWAYSINLINE T& operator[] (const size_t index) {
			return array[index];
		}
		/** @warning NO BOUNDS CHECKING */
		ALWAYSINLINE T operator[] (const size_t index) const {
			return array[index];
		}
		ALWAYSINLINE operator void *() {
			return (void *)array;
		}
		ALWAYSINLINE operator const void *() const {
			return (const void *)array;
		}

		ALWAYSINLINE size_t count() {return N;}

		ALWAYSINLINE Array<T, N>& operator= (T rvalue) {
			for (unsigned i=0; i<N; i++) {
				this->operator[](i) = rvalue;
			}
			return *this;
		}
	};
}

#endif	/* NUPP_STACK_HPP */
