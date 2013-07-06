/* 
 * File:   string.hpp
 * Author: alex
 *
 * Created on July 5, 2013, 9:41 PM
 */

#ifndef NUPP_STRING_HPP
#define	NUPP_STRING_HPP

#include "nu/compiler.h"

namespace nu {
	/**
	 * Heavyweight, stack-allocated, C++ string. Use only if necessary.
	 */
	struct String {
		static const size_t _limit = 128;
		
		size_t _size;
		char _c_str[_limit];

		ALWAYSINLINE String(): _size(0) {}

		ALWAYSINLINE size_t size() const {
			return _size;
		}

		ALWAYSINLINE size_t max_size() const {
			return _limit;
		}

		ALWAYSINLINE void clear() {
			_size = 0;
		}

		ALWAYSINLINE bool empty() const {
			return _size == 0;
		}

		ALWAYSINLINE char& at (size_t pos) {
			return _c_str[pos];
		}

		ALWAYSINLINE const char& at (size_t pos) const {
			return _c_str[pos];
		}

		ALWAYSINLINE char& operator[] (size_t pos) {
			return _c_str[pos];
		}

		ALWAYSINLINE const char& operator[] (size_t pos) const {
			return _c_str[pos];
		}


		ALWAYSINLINE char& back() {
			return _c_str[_limit - 1];
		}

		ALWAYSINLINE const char& back() const {
			return _c_str[_limit - 1];
		}

		ALWAYSINLINE char& front() {
			return _c_str[0];
		}

		ALWAYSINLINE const char& front() const {
			return _c_str[0];
		}

		ALWAYSINLINE String& operator+= (char c) {
			if (_size != _limit) {
				_c_str[_size] = c;
				_size++;
			}
			return *this;
		}

		ALWAYSINLINE String& operator+= (const char* s) {
			for (size_t i=0; i<strlen(s); i++) {
				if (_size == _limit)
					break;
				_c_str[_size] = s[i];
				_size++;
			}
			return *this;
		}

		ALWAYSINLINE String& operator+= (const String& str) {
			for (size_t i=0; i<str._size; i++) {
				if (_size == _limit)
					break;
				_c_str[_size] = str[i];
				_size++;
			}
		}

		ALWAYSINLINE const char *c_str() const {
			return _c_str;
		}

		ALWAYSINLINE int compare (const String& str) const {
			size_t i=0;
			while (i != _size && i != str._size) {
				if (at(i) != str[i])
					return 0;
				i++;
			}
			if (i == _size && i == str._size)
				return 1;
			return 0;
		}
	};
}

#endif	/* NUPP_STRING_HPP */

