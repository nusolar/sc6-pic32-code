/*
 * File:   allocator.hpp
 * Author: alex
 *
 * Created on May 12, 2013, 5:33 PM
 */

#ifndef NUPP_ALLOCATOR_HPP
#define	NUPP_ALLOCATOR_HPP

#include <cstddef>
#include <new>

struct Arena {
	static size_t N;
	static char *stack;
	static char *ptr;
};


template <typename T>
class Allocator {
	Allocator& operator=(const Allocator&);
	
public:
	typedef T * pointer;
    typedef const T * const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef T value_type;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

	T * address(T& r) const {
        return &r;
    }
	const T * address(const T& s) const {
        return &s;
    }
	
	size_t max_size() const {
		return (static_cast<size_t>(0) - static_cast<size_t>(1)) / sizeof(T);
    }
	
	template <typename U> struct rebind {
        typedef Allocator<U> other;
    };
	
    void construct(T * const p, const T& t) const {
        void * const pv = static_cast<void *>(p);
        new (pv) T(t);
    }
    void destroy(T * const p) const {
		p->~T();
	}
	
	T *allocate(const size_t n) const {
		if (n == 0) {
            return NULL;
        }
		if (n > max_size()) {
            throw std::length_error("Allocator<T>::allocate() - Integer overflow.");
        }
		void * const pv = malloc(n * sizeof(T));
		if (pv == NULL) {
            throw std::bad_alloc();
        }
        return static_cast<T *>(pv);
	}
	
	void deallocate(T * const p, const size_t n) const {
		free(p);
	}
	
	/** Allocator is Stateless */
	Allocator() { }
	Allocator(const Allocator&) { }
    template <typename U> Allocator(const Allocator<U>&) { }
    ~Allocator() { }
	bool operator!=(const Allocator& other) const {return !(*this == other);}
	bool operator==(const Allocator& other) const {return true;}
	
	/** ignores hint */
	template <typename U> T * allocate(const size_t n, const U*) const {
        return allocate(n);
    }
};

#endif	/* NUPP_ALLOCATOR_HPP */

