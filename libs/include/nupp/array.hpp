#include "nu/compiler.h"

#ifndef NUPP_STACK_HPP
#define	NUPP_STACK_HPP

namespace nu {
	template <typename T, size_t N>
	class Array {
		T array[N];
	public:
		ALWAYSINLINE Array<T,N>() {memset(array, 0, sizeof(array));}
//		ALWAYSINLINE Array<T,N>(uint8_t init) {memset(array, init, sizeof(array));}

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
		ALWAYSINLINE size_t size() {return sizeof(T)*N;}

		ALWAYSINLINE Array<T, N>& operator= (T& rvalue) {
			for (unsigned i=0; i<N; i++) {
				this->operator[](i) = rvalue;
			}
			return *this;
		}

		ALWAYSINLINE bool operator== (Array<T,N>& rvalue) {
			bool result = true;
			for (unsigned i=0; i<N; i++) {
				result &= (this->operator[](i) == rvalue[i]);
			}
			return result;
		}

		ALWAYSINLINE bool byte_compare (Array<T,N>& rvalue) {
			bool result = true;
			for (unsigned i=0; i<N; i++) {
				result &= (memcmp(&array[i], &rvalue.array[i], sizeof(array[i])) == 0);
			}
			return result;
		}
	};
}

#endif	/* NUPP_STACK_HPP */
