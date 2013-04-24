//
//  optional.h
//  nusolar_lib
//
//  Created by Al Chandel on 4/24/13.
//  Copyright (c) 2013 Alex Chandel. All rights reserved.
//

#ifndef __nusolar_lib__optional__
#define __nusolar_lib__optional__

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

#endif /* defined(__nusolar_lib__optional__) */
