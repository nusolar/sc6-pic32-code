/* 
 * File:   buffer.hpp
 * Author: alex
 *
 * Created on May 12, 2013, 5:13 AM
 */

#ifndef NU_BUFFER_HPP
#define	NU_BUFFER_HPP

#include <sstream>

namespace nu {
	/**
	 * USES HEAP.
	 * @todo write & instantiate std::allocator that uses Stack
	 */
	template <class _Parent>
	class Buffer: public std::stringbuf {
		_Parent &parent;
		virtual int sync() {
			parent.puts(str().c_str()); // WARNING: Deep copy str
			str("");
			return 0;
		}
	public:
		ALWAYSINLINE Buffer(_Parent &_parent): std::stringbuf(), parent(_parent) {}
	};
}
#endif	/* NU_BUFFER_HPP */