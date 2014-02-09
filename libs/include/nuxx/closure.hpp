/* 
 * File:   closure.hpp
 * Author: alex
 *
 * Created on September 16, 2013, 7:09 AM
 */

#ifndef CLOSURE_HPP
#define	CLOSURE_HPP

template <class T, typename R, typename ...ArgP>
class Closure {
public:
	typedef R (*FuncType)(ArgP...);

private:
	static T *global_context;

	template <R (T::*Mptr) (ArgP...)>
	static R callback(ArgP... args) {
		return (global_context->*Mptr)(args...);
	}
public:
	template <R (T::*Mptr) (ArgP...)>
	static FuncType use(T &obj) {
		global_context = &obj;
		return & callback<Mptr>;
	}
};

template<class T, typename R, typename ...ArgP>
T* Closure<T, R, ArgP...>::global_context = NULL;

#endif	/* CLOSURE_HPP */

