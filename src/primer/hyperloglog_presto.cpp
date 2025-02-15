//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// hyperloglog_presto.cpp
//
// Identification: src/primer/hyperloglog_presto.cpp
//
// Copyright (c) 2015-2025, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#include "primer/hyperloglog_presto.h"
#include <algorithm>
#include <bitset>
#include <cmath>
#include <cstdint>
#include <vector>
#include "primer/hyperloglog.h"
#include "primer/util.h"

namespace bustub {

/** @brief Parameterized constructor. */
template <typename KeyType>
HyperLogLogPresto<KeyType>::HyperLogLogPresto(int16_t n_leading_bits) : cardinality_(0) {
  if (n_leading_bits < 0 || n_leading_bits > 64) {
    this->b_ = 0;
    this->m_ = 0;
    return ;
  }
  this->b_ = n_leading_bits;
  this->m_ = 1ULL << n_leading_bits;
  this->dense_bucket_.resize(this->m_);
}

/** @brief Element is added for HLL calculation. */
template <typename KeyType>
auto HyperLogLogPresto<KeyType>::AddElem(KeyType val) -> void {
  /** @TODO(student) Implement this function! */
  auto hash_v = static_cast<uint64_t>(this->CalculateHash(val));
  std::pair<uint64_t, uint64_t> hi_lo = __bits_hi_lo_(hash_v, this->b_);
  auto zero_cnt = __bits_tail_zero_(hi_lo.second);

  __debug_print_(hash_v, hi_lo);
  std::cout << "zero_cnt = " << zero_cnt << "\n\n";

  if (zero_cnt < this->m_) { // dense
    this->StoreDenseBucket(hi_lo, zero_cnt);
  } else { // overflow // zero_cnt [0, 64]
    auto of_key = static_cast<uint16_t>(hi_lo.first);
    uint64_t of_val = (static_cast<uint64_t>(zero_cnt) & 0x70ULL) >> 4;
    if (this->overflow_bucket_.find(of_key) != this->overflow_bucket_.end()) {
      of_val = std::max(this->overflow_bucket_[of_key].to_ullong(), of_val);
    }
    // std::cout << "of_key = " << of_key << " of_val = " << of_val << '\n';
    this->overflow_bucket_.insert_or_assign(of_key, std::bitset<3>(of_val));
    this->StoreDenseBucket(hi_lo, zero_cnt & 0x0FULL);
  }
}

template <typename KeyType>
auto HyperLogLogPresto<KeyType>::StoreDenseBucket(std::pair<uint64_t, uint64_t> hi_lo, size_t set_val) -> void {
  this->dense_bucket_[hi_lo.first] = std::bitset<4>(
    std::max(this->dense_bucket_[hi_lo.first].to_ullong(), static_cast<uint64_t>(set_val))
  );
}

/** @brief Function to compute cardinality. */
template <typename T>
auto HyperLogLogPresto<T>::ComputeCardinality() -> void {
  /** @TODO(student) Implement this function! */
  double sum = 0;
  
  // dense contribute
  for (size_t i = 0; i < this->m_; i++){
    sum += std::pow(2.0, -1.0 * static_cast<double>(this->dense_bucket_[i].to_ulong()));
  }

  // overflow contribute
  for (auto hit = this->overflow_bucket_.begin(); hit != this->overflow_bucket_.end(); hit++) {
    sum += std::pow(2.0, -1.0 * static_cast<double>(hit->second.to_ulong()));
  }

  if (sum == 0) {
    this->cardinality_ = 0;
  } else {
    this->cardinality_ = static_cast<uint64_t>(__hll_card_formula_(this->CONSTANT, this->m_, sum));
  }
}

template class HyperLogLogPresto<int64_t>;
template class HyperLogLogPresto<std::string>;
}  // namespace bustub
