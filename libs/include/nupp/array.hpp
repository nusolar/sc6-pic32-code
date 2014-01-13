#include "nu/compiler.h"

#ifndef NUPP_ARRAY_HPP
#define	NUPP_ARRAY_HPP

namespace nu {
	/**
	 * A stack-allocated array. This replaces std::array and std::vector.
	 * @warning NO BOUNDS CHECKING
	 */
	template <typename T, size_t N>
	class Array {
		T array[N];
	public:
		INLINE T& at(const size_t index) {
			return array[index];
		}
		INLINE T  at(const size_t index) const {
			return array[index];
		}
		
		INLINE T& operator[] (const size_t index) {return at(index);}
		INLINE T  operator[] (const size_t index) const {return at(index);}

		INLINE operator void *() {
			return (void *)array;
		}
		INLINE operator const void *() const {
			return (const void *)array;
		}
		INLINE const T *data() {
			return (const T*)array;
		}

		INLINE size_t count() {return N;}
		INLINE size_t size() {return sizeof(T)*N;}

		INLINE Array<T, N>& operator= (T& rvalue) {
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
		INLINE Array<T, N>& operator= (Array<T2, N>& rvalue) {
			for (unsigned i=0; i<N; i++) {
				at(i) = rvalue[i];
			}
			return *this;
		}

		INLINE bool operator== (Array<T,N>& rvalue) {
			bool result = true;
			for (unsigned i=0; i<N; i++) {
				result *= (at(i) == rvalue[i]);
			}
			return result;
		}

		INLINE bool byte_compare (Array<T,N>& rvalue) {
			bool result = true;
			for (unsigned i=0; i<N; i++) {
				result &= (memcmp(&array[i], &rvalue.array[i], sizeof(array[i])) == 0);
			}
			return result;
		}

		INLINE Array<T,N>() {}
		INLINE Array<T,N>(T init) {*this = init;}
	};
}

#endif	/* NUPP_ARRAY_HPP */
