/*
 * File:   buffer.hpp
 * Author: alex
 *
 * Created on May 12, 2013, 5:13 AM
 */

#ifndef NU_BUFFER_HPP
#define	NU_BUFFER_HPP

#include <sstream>
#include "nupp/allocator.hpp"

namespace nu {
	typedef std::basic_stringbuf<char, std::char_traits<char>, Allocator<char>> stackbuf;
	/**
	 * USES HEAP.
	 * @todo write & instantiate std::allocator that uses Stack
	 */
	template <class _Parent>
	class Buffer: public stackbuf {
		_Parent &parent;
		virtual int sync() {
			parent.puts(str().c_str()); // WARNING: Deep copy str
			str("");
			return 0;
		}
	public:
		ALWAYSINLINE Buffer(_Parent &_parent): stackbuf(), parent(_parent) {}
	};
}
#endif	/* NU_BUFFER_HPP */
