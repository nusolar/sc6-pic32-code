#ifndef NUPP_OPTIONAL_HPP
#define NUPP_OPTIONAL_HPP 1

#include <new>

namespace nu {
	/**
	 * An attempt to implement a reassignable reference variable, that actually
	 * behaves like the underlying type. However function calls & casts didn't
	 * work.
	 */
	template <class A>
	class optional {
		A *member;
	public:
		optional(): member(NULL) {}

		optional(const optional &a) {
			member = a.member;
		}
		optional &operator= (const optional &a) {
			if (this != &a){
				this->optional::~optional();
				new (this) optional(a);
			} return *this;
		}

		optional(const A& a) {
			member = &a;
		}
		optional &operator= (const A &a) {
			member = &a;
			return *this;
		}

		/**
		 * ERROR: Doesn't work. Returned value must be able to cast
		 * itself implicitly.
		 */
		operator A() const {
			return *member;
		}
	};
}

#endif
