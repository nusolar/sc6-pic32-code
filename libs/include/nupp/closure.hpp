/* 
 * File:   closure.hpp
 * Author: alex
 *
 * Created on September 16, 2013, 7:09 AM
 */

#ifndef CLOSURE_HPP
#define	CLOSURE_HPP

template <class T, typename Mptr, typename R, typename ...ArgP>
class Closure {
public:
	typedef R (*FuncType)(ArgP...);

private:
	static T *global_context;
	static Mptr method;

	static R callback(ArgP... args) {
		if (global_context) {
			if (method) {
				(global_context->*method)(args...);
			}
		}
	}
public:
	static FuncType use(T &obj, Mptr meth) {
		global_context = &obj;
		method = meth;
		return &callback;
	}
};

template<class T, typename Mptr, typename R, typename ...ArgP>
T* Closure<T, Mptr, R, ArgP...>::global_context = NULL;
template<class T, typename Mptr, typename R, typename ...ArgP>
Mptr Closure<T, Mptr, R, ArgP...>::method = NULL;

#endif	/* CLOSURE_HPP */

