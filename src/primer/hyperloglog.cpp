//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// hyperloglog.cpp
//
// Identification: src/primer/hyperloglog.cpp
//
// Copyright (c) 2015-2025, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#include "primer/hyperloglog.h"
#include <sys/types.h>
#include <bitset>
#include <cmath>
#include <cstdint>
#include <vector>
#include <utility>

namespace bustub {

/** @brief Parameterized constructor. */
template <typename KeyType>
HyperLogLog<KeyType>::HyperLogLog(int16_t n_bits) : cardinality_(0) {
  if (n_bits > 64 || n_bits < 0) {
    this->b_ = 0;
    this->m_ = 0;
    return ;
  }
  this->b_ = n_bits;
  this->m_ = 1 << n_bits;
  this->reg_.resize(this->m_);
}

/**
 * @brief Function that computes binary.
 *
 * @param[in] hash
 * @returns binary of a given hash
 */
template <typename KeyType>
auto HyperLogLog<KeyType>::ComputeBinary(const hash_t &hash) const -> std::bitset<BITSET_CAPACITY> {
  /** @TODO(student) Implement this function! */
  return {hash};
}

/**
 * @brief Function that computes leading zeros.
 *
 * @param[in] bset - binary values of a given bitset
 * @returns leading zeros of given binary set
 */
template <typename KeyType>
auto HyperLogLog<KeyType>::PositionOfLeftmostOne(const std::bitset<BITSET_CAPACITY> &bset) const -> uint64_t {
  /** @TODO(student) Implement this function! */
  // if (bset == 0) {
  //   return BITSET_CAPACITY - this->b_ + 1;
  // }
  uint64_t board_index = (this->b_ > 0 ? BITSET_CAPACITY - 1 - this->b_ : 0);
  uint64_t cnt_zero = 0;
  for (uint64_t i = BITSET_CAPACITY - 1; i > board_index; i--) {
    if (!bset[i]) {
      cnt_zero++;
    } else {
      // std::cout << "bset = " << bset << " MSB = " << cnt_zero + 1 << '\n';
      return cnt_zero + 1;
    }
  }
  return this->b_ > 0 ? BITSET_CAPACITY - this->b_ + 1 : BITSET_CAPACITY;
}

/**
 * @brief Adds a value into the HyperLogLog.
 *
 * @param[in] val - value that's added into hyperloglog
 */
template <typename KeyType>
auto HyperLogLog<KeyType>::AddElem(KeyType val) -> void {
  /** @TODO(student) Implement this function! */
  auto hash_bin = this->ComputeBinary(this->CalculateHash(val)).to_ullong();
  std::pair<uint64_t, uint64_t> hi_lo = __bits_hi_lo_(hash_bin, this->b_);

  // if (this->b_ == 0){ }
  
  this->reg_[hi_lo.first] =
      std::max(this->reg_[hi_lo.first], this->PositionOfLeftmostOne(std::bitset<BITSET_CAPACITY>(hi_lo.second << this->b_)));
  // if (this->b_ == 0){ std::cout << this->reg_[hi_lo.first] << '\n';}
}

/**
 * @brief Function that computes cardinality.
 */
template <typename KeyType>
auto HyperLogLog<KeyType>::ComputeCardinality() -> void {
  /** @TODO(student) Implement this function! */
  double sum = 0;
  for (size_t i = 0; i < this->m_; i++) {
    sum += std::pow(2.0, -1.0 * static_cast<double>(this->reg_[i]));
  }
  // std::cout << "sum = " << sum << '\n';
  if (sum == 0) {
    this->cardinality_ = 0;
  } else {
    this->cardinality_ = static_cast<size_t>(__hll_card_formula_(this->CONSTANT, this->m_, sum));
  }
}

template class HyperLogLog<int64_t>;
template class HyperLogLog<std::string>;

}  // namespace bustub
