/*
 * Taken from Howard Hinnent's stack_alloc page.
 * http://home.roadrunner.com/~hinnant/stack_alloc.html
 */


#ifndef SHORT_ALLOC_H
#define SHORT_ALLOC_H

#include <cstddef>
#include <cassert>

namespace nu {

	template <std::size_t N>
	class arena {
		static const std::size_t alignment = 16;
		__attribute__((aligned(16))) char buf_[N];
		char* ptr_;

		std::size_t align_up(std::size_t n) {return n + ((alignment-1) & ~(alignment-1));}
		bool pointer_in_buffer(char* p) {return buf_ <= p && p <= buf_ + N;}

	public:
		arena(): ptr_(buf_) {}
		~arena() {ptr_ = NULL;}
		arena(const arena& a): ptr_(a.ptr_) {}// = delete;
		arena& operator=(const arena&) = delete;

		char* allocate(std::size_t n) {
			assert(pointer_in_buffer(ptr_) && "short_alloc has outlived arena");
			n = align_up(n);
			if (buf_ + N - ptr_ >= (int) n) {
				char* r = ptr_;
				ptr_ += n;
				return r;
			}
			return static_cast<char*>(::operator new(n));
		}
		void deallocate(char* p, std::size_t n) {
			assert(pointer_in_buffer(ptr_) && "short_alloc has outlived arena");
			if (pointer_in_buffer(p)) {
				n = align_up(n);
				if (p + n == ptr_) ptr_ = p;
			}
			else ::operator delete(p);
		}

		static std::size_t size() {return N;}
		std::size_t used() const {return static_cast<std::size_t>(ptr_ - buf_);}
		void reset() {ptr_ = buf_;}
	};

	template <class T, std::size_t N>
	class short_alloc {
		arena<N> a_;
	public:
		typedef T value_type;
		
		template <class _Up> struct rebind {typedef short_alloc<_Up, N> other;};

		short_alloc(arena<N> a = arena<N>()): a_(a) {}
		template <class U>
		short_alloc(const short_alloc<U, N>& a): a_(a.a_) {}
		short_alloc(const short_alloc&) = default;
		short_alloc& operator=(const short_alloc&) = delete;

		T* allocate(std::size_t n) {
			return reinterpret_cast<T*>(a_.allocate(n*sizeof(T)));
		}
		void deallocate(T* p, std::size_t n) {
			a_.deallocate(reinterpret_cast<char*>(p), n*sizeof(T));
		}

		template <class T1, std::size_t N1, class U, std::size_t M>
		friend inline
		bool operator==(const short_alloc<T1, N1>& x, const short_alloc<U, M>& y){
			return N == M && &x.a_ == &y.a_;
		}

		template <class U, std::size_t M> friend class short_alloc;
	};

	template <class T, std::size_t N, class U, std::size_t M>
	inline
	bool operator!=(const short_alloc<T, N>& x, const short_alloc<U, M>& y)
	{
		return !(x == y);
	}

	static const unsigned buffersize = 1024;
	typedef std::basic_stringbuf<char, std::char_traits<char>, short_alloc<char, buffersize>> stackbuf;
}
#endif  // SHORT_ALLOC_H
