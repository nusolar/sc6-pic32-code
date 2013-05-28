/*
 * File:   buffer.hpp
 * Author: alex
 *
 * Created on May 12, 2013, 5:13 AM
 */

#ifndef NU_BUFFER_HPP
#define	NU_BUFFER_HPP

#include "nu/compiler.h"
#include <cstdlib>
#include <cstddef>
#include <cstdint>
#include <string>
//#include "nupp/allocator.hpp"

namespace nu {
//	typedef std::basic_ostringstream <char, std::char_traits<char>, Allocator<char>> oss;

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
		std::string _str;
		char _buffer[72];
	public:
		ALWAYSINLINE OStream(): _str("") {}
		virtual ~OStream() {}
		virtual void puts(const char *){}

//		template <typename T>
//		ALWAYSINLINE OStream& operator<< (const T &val) {out << val; return *this;}

		/**
		 * Forward all std stream manipulators.
		 * @warning Inlining breaks MPLAB symbol loading.
         */
		/*NOINLINE OStream& operator<< (std::ostream&(*m)(std::ostream&)) {
			out << m;
			return *this;
		}*/

		ALWAYSINLINE OStream& operator<< (const double val) {
			if (likely(snprintf(NULL, 0, "%f", val) <= 72)) {
				sprintf(_buffer, "%f", val);
				_str += _buffer;
			}
			return *this;
		}

		ALWAYSINLINE OStream& operator<< (const uint64_t val) {
			if (likely(snprintf(NULL, 0, "%llu", val) <= 72)) {
				sprintf(_buffer, "%llu", val);
				_str += _buffer;
			}
			return *this;
		}

		ALWAYSINLINE OStream& operator<< (const uint32_t val) {
			if (likely(snprintf(NULL, 0, "%u", val) <= 72)) {
				sprintf(_buffer, "%u", val);
				_str += _buffer;
			}
			return *this;
		}

		ALWAYSINLINE OStream& operator<< (const char val) {
			_str += val;
			return *this;
		}

		ALWAYSINLINE OStream& operator<< (const char *val) {
			_str += val;
			return *this;
		}

		ALWAYSINLINE OStream& operator<< (bool val) {
			_str += val? '0': '1';
			return *this;
		}

		friend OStream& end(OStream& os);
		ALWAYSINLINE OStream& operator<< (OStream&(*m)(OStream&)) {
			m(*this);
			return *this;
		}

		void PRINTF(2,3) printf(const char *fmt, ...);
	};

	OStream& end(OStream& os);
}
#endif	/* NU_BUFFER_HPP */
