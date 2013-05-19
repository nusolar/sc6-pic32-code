/*
 * File:   buffer.hpp
 * Author: alex
 *
 * Created on May 12, 2013, 5:13 AM
 */

#ifndef NU_BUFFER_HPP
#define	NU_BUFFER_HPP

#include <cstdlib>
#include <sstream>
#include "nupp/allocator.hpp"
using namespace std;
namespace nu {
	typedef std::basic_ostringstream <char, std::char_traits<char>, Allocator<char>> oss;

	/**
	 * USES HEAP.
	 * @todo write & instantiate std::allocator that uses Stack
	typedef std::stringbuf stackbuf;
	template <class _Parent>
	class Buffer: public stackbuf {
		_Parent * parent;
		virtual int sync() {
			parent->puts(str().c_str()); // WARNING: Deep copy str
			str("");
			return 0;
		}
	public:
		ALWAYSINLINE Buffer(_Parent * _parent): stackbuf(), parent(_parent) {}
		ALWAYSINLINE Buffer(const Buffer &rval): stackbuf(), parent(rval.parent) {
			memcpy(this, &rval, sizeof(*this));
		}
		Buffer& operator= (const Buffer& rval) {
			if (this != &rval) {
				memcpy(this, &rval, sizeof(*this));
			} return *this;
		}
	};*/

	/*struct OStream;

	class StringStream: public std::ostringstream {
		OStream *parent;
		virtual int sync();
	public:
		ALWAYSINLINE StringStream(OStream *_parent): std::ostringstream(), parent(_parent) {}
		ALWAYSINLINE StringStream(const StringStream &rval): ios(), std::ostringstream(), parent(rval.parent) {
			memcpy(this, &rval, sizeof(*this));
		}
		ALWAYSINLINE StringStream& operator= (const StringStream &rval) {
			if (this != &rval) {
				memcpy(this, &rval, sizeof(*this));
			} return *this;
		}
	};*/

	class OStream {
		std::ostringstream out;
	public:
		ALWAYSINLINE OStream(): out() {}
		virtual ~OStream() {}
		virtual void puts(const char *){}

		template <typename T>
		ALWAYSINLINE OStream& operator<< (const T &val) {out << val; return *this;}

		/**
		 * Forward all std stream manipulators.
		 * @warning Inlining breaks MPLAB symbol loading.
         */
		/*NOINLINE OStream& operator<< (std::ostream&(*m)(std::ostream&)) {
			out << m;
			return *this;
		}*/

		friend OStream& end(OStream& os);
		ALWAYSINLINE OStream& operator<< (OStream&(*m)(OStream&)) {
			m(*this);
			return *this;
		}
	};

	OStream& end(OStream& os);
}
#endif	/* NU_BUFFER_HPP */
