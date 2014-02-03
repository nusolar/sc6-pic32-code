#include "nu/compiler.h"

#ifndef NUPP_ARRAY_HPP
#define	NUPP_ARRAY_HPP

namespace nu {
	/**
	 * A stack-allocated array. This replaces std::array and std::vector.
	 * All methods are ALWAYSINLINE'd for speed.
	 * @warning NO BOUNDS CHECKING
	 */
	template <typename T, size_t N>
	class Array {
		T array[N];
	public:
		ALWAYSINLINE T& at(const size_t index) {
			return array[index];
		}
		ALWAYSINLINE T  at(const size_t index) const {
			return array[index];
		}
		
		ALWAYSINLINE T& operator[] (const size_t index) {return at(index);}
		ALWAYSINLINE T  operator[] (const size_t index) const {return at(index);}

		ALWAYSINLINE operator void *() {
			return (void *)array;
		}
		ALWAYSINLINE operator const void *() const {
			return (const void *)array;
		}
		ALWAYSINLINE T *data() {
			return (T*)array;
		}
		ALWAYSINLINE const T *data() const {
			return (const T*)array;
		}

		ALWAYSINLINE size_t count() {return N;}
		ALWAYSINLINE size_t size() {return sizeof(T)*N;}

		ALWAYSINLINE Array<T, N>& operator= (T& rvalue) {
			for (unsigned i=0; i<N; i++) {
				at(i) = rvalue;
			}
			return *this;
		}

		/**
		 * Copy an identically sized array of any type.
         * @param rvalue The array to be copied.
         */
		template <typename T2>
		ALWAYSINLINE Array<T, N>& operator= (Array<T2, N>& rvalue) {
			for (unsigned i=0; i<N; i++) {
				at(i) = rvalue[i];
			}
			return *this;
		}

		ALWAYSINLINE bool operator== (Array<T,N>& rvalue) {
			bool result = true;
			for (unsigned i=0; i<N; i++) {
				result *= (at(i) == rvalue[i]);
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

		ALWAYSINLINE Array<T,N>() {}
		ALWAYSINLINE Array<T,N>(T init) {*this = init;}
	};
}

#endif	/* NUPP_ARRAY_HPP */
