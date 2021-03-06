//
//  bitset.h
//  nusolar_lib
//
//  Created by Al Chandel on 5/12/13.
//  Copyright (c) 2013 Alex Chandel. All rights reserved.
//

#ifndef __nusolar_lib__bitset__
#define __nusolar_lib__bitset__

#include "nu/compiler.h"
#include <cstddef>
#include <cstdint>

namespace nu {
	/**
	 * @warning ASSUMES LITTLE-ENDIAN
	 */
	template <size_t N>
	class Bitset {
		union bitcontainer {
			uint8_t array[(N - N%8 + 8)/8];
			uint64_t ulong;
		} bits;
	public:
		class bit {
			friend class Bitset;
			Bitset &par;
			size_t pos;
			bit(Bitset &_par, size_t _pos): par(_par), pos(_pos) {};
		public:
			~bit() {};
			operator bool() const {
				return par.bits.array[pos/8] & (1 << pos%8);
			}
			/** EVIL BIT ARITHMETIC LOOK AWAY */
			bit& operator= (bool rvalue) {
				par.bits.array[pos/8] ^= (uint8_t)(((par.bits.array[pos/8]>>pos%8 & 1) != rvalue) << pos%8);
				return *this;
			}

			bit& operator|= (bool rvalue) {
				par.bits.array[pos/8] |= (uint8_t)((uint8_t)rvalue << pos%8);
				return *this;
			}
		};

		Bitset& operator&= (uint64_t rvalue) {
			bits.ulong &= rvalue;
			return *this;
		}

		operator uint64_t() const {return bits.ulong;}
		bool operator[] (size_t pos) const {return (bool)bit(*this,pos);}
		bit operator[] (size_t pos) {return bit(*this,pos);}
		size_t size() const {return N;}

		Bitset(): bits() {bits.array = {0};}
		Bitset(uint64_t l): bits() {bits.array = {0}; bits.ulong = l;}
	};
}

#endif /* defined(__nusolar_lib__bitset__) */
