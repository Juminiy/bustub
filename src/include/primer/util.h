#pragma once

#include <utility>


#define __bits_hi_lo_(__64v, __lead_b) \
	(__lead_b == 0) ? std::make_pair(0ULL, __64v) : \
		(__lead_b >= 64) ? std::make_pair(__64v, 0ULL) : \
		std::make_pair((__64v >> (64 - __lead_b)) & ((1ULL << __lead_b) - 1), __64v & ((1ULL << (64 - __lead_b)) - 1))

#define __bits_tail_zero_(__64v) static_cast<size_t>(__builtin_ctzll(__64v))

#define __hll_card_formula_(constant_, m_, sum_) std::floor(constant_ * std::pow(m_*1.0, 2.0) / sum_)

#define __debug_print_(v_, hi_lo) \
	std::cout << "  hash_bin = " << std::bitset<64>(v_) << '\n'; \
 	std::cout << " index_bin = " << std::bitset<64>(hi_lo.first) << '\n'; \
  std::cout << "remain_bin = " << std::bitset<64>(hi_lo.second) << '\n'