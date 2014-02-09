/*
 * File:   buffer.hpp
 * Author: alex
 *
 * Created on May 12, 2013, 5:13 AM
 */

#ifndef NU_BUFFER_HPP
#define	NU_BUFFER_HPP

#include "nu/compiler.h"
#include "nuxx/string.hpp"
#include <cstdlib>
#include <cstddef>
#include <cstdint>
#include <cstdio>

namespace nu {
	class OStream {
		String _str;
		char _buffer[72];

	public:
		ALWAYSINLINE OStream(): _str("") {}
		virtual ~OStream() {}
		virtual void puts(const char *){}

		ALWAYSINLINE OStream& operator<< (const double val) {
			if (likely(snprintf(NULL, 0, "%f", val) <= 72)) {
				sprintf(_buffer, "%f", val);
				_str += _buffer;
			}
			return *this;
		}

		ALWAYSINLINE OStream& operator<< (const unsigned long long val) {
			if (likely(snprintf(NULL, 0, "%llu", val) <= 72)) {
				sprintf(_buffer, "%llu", val);
				_str += _buffer;
			}
			return *this;
		}

		ALWAYSINLINE OStream& operator<< (const unsigned int val) {
			if (likely(snprintf(NULL, 0, "%u", val) <= 72)) {
				sprintf(_buffer, "%u", val);
				_str += _buffer;
			}
			return *this;
		}

		ALWAYSINLINE OStream& operator<< (const int val) {
			if (likely(snprintf(NULL, 0, "%i", val) <= 72)) {
				sprintf(_buffer, "%i", val);
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

		ALWAYSINLINE OStream& operator<< (const unsigned char val) {
			_str += val;
			return *this;
		}

		ALWAYSINLINE OStream& operator<< (const bool val) {
			_str += val? '1': '0';
			return *this;
		}

		ALWAYSINLINE OStream& operator<< (OStream&(*m)(OStream&)) {
			m(*this);
			return *this;
		}

		friend OStream& end(OStream& os);
		void PRINTF(2,3) printf(const char *fmt, ...);
	};

	OStream& end(OStream& os);
}
#endif	/* NU_BUFFER_HPP */
