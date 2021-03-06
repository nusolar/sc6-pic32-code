/**
 * File:   allocator.hpp
 * Author: alex
 *
 * Created on May 12, 2013, 5:33 PM
 *
 * @file An attempted stack-based Allocator for STL classes. Currently,
 *       deallocations fail unless performed in the precise reverse order.
 */

#ifndef NUXX_ALLOCATOR_HPP
#define	NUXX_ALLOCATOR_HPP

#include "nu/compiler.h"
#include <alloca.h>
//#include <cstdlib>
#include <cstddef>
#include <new>

#define NU_GIFT(x)\
	Arena::N = x;\
	Arena::stack_base = (char *)alloca(x);\
	Arena::ptr = Arena::stack_base;

namespace nu {
	struct Arena {
		static size_t N;
		static char *stack_base;
		static char *ptr;

		ALWAYSINLINE static bool pointer_in_buffer(char *p) {
			return stack_base <= p && p  <= stack_base + N;
		}
	};


	template <typename T>
	class Allocator {
		ALWAYSINLINE Allocator& operator=(const Allocator&);

	public:
		typedef T * pointer;
		typedef const T * const_pointer;
		typedef T& reference;
		typedef const T& const_reference;
		typedef T value_type;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;

		ALWAYSINLINE T *address(T& r) const {
			return &r;
		}
		ALWAYSINLINE const T *address(const T& s) const {
			return &s;
		}

		ALWAYSINLINE size_t max_size() const {
			return (static_cast<size_t>(0) - static_cast<size_t>(1)) / sizeof(T);
		}

		template <typename U> struct rebind {
			typedef Allocator<U> other;
		};

		ALWAYSINLINE void construct(T * const p, const T& t) const {
			void * const pv = static_cast<void *>(p);
			new (pv) T(t);
		}
		ALWAYSINLINE void destroy(T * const p) const {
			p->~T();
		}

		ALWAYSINLINE T *allocate(const size_t n) const {
			if (n == 0) {
				return NULL;
			}
			if (n > max_size()) {
//				throw std::length_error("Allocator<T>::allocate() - Integer overflow.");
			}

			char *pv = NULL;
			if (likely(Arena::pointer_in_buffer(Arena::ptr + n))) {
				pv = Arena::ptr;
				Arena::ptr += n;
			} else {
//				pv = (char *)malloc(n * sizeof(T));
			}
			if (pv == NULL) {
//				throw std::bad_alloc();
			}
			return static_cast<T *>(pv);
		}

		ALWAYSINLINE void deallocate(T * const p, const size_t n) const {
			if (likely(Arena::pointer_in_buffer((char *)p))) {
				if ((char *)p + n == Arena::ptr) {
					Arena::ptr = (char *)p;
				} else {
					// WARNING unimplemented
				}
			} else {
//				free(p);
			}
		}

		/** Allocator is Stateless */
		ALWAYSINLINE Allocator() { }
		ALWAYSINLINE Allocator(const Allocator&) { }
		template <typename U> ALWAYSINLINE Allocator(const Allocator<U>&) { }
		ALWAYSINLINE ~Allocator() { }
		ALWAYSINLINE bool operator!=(const Allocator& other) const {return !(*this == other);}
		ALWAYSINLINE bool operator==(const Allocator& other) const {return true;}

		/** ignores hint */
		template <typename U> ALWAYSINLINE T *allocate(const size_t n, const U*) const {
			return allocate(n);
		}
	};
}
#endif	/* NUXX_ALLOCATOR_HPP */

