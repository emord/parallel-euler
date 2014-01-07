#pragma once
#include <cstdint>
#include <vector>

namespace LibEuler {
  template <typename T> std::vector<T> SieveOfEratosthenes(T limit);
  template <typename T> std::vector<T> PrimeNumGen(T limit);
  template <typename T> std::vector<T> FibsUnderNumber(T limit);
}

template <typename T>
std::vector<T> LibEuler::PrimeNumGen(T limit) {
  return LibEuler::SieveOfEratosthenes(limit);
}

template <typename T>
std::vector<T> LibEuler::SieveOfEratosthenes(T limit) {
  long double finish_point = sqrtl(limit);
  std::vector<bool> candidates(limit, false);
  std::vector<T> results {2};

  for (T i = 3; i < limit; i+=2) {
    candidates[i] = true;
  }

  for (std::vector<bool>::size_type i = 3; i < candidates.size(); i+=2) {
    if (!candidates[i]) continue;
    if (i > finish_point) break;
    results.push_back(i);

    for (auto j = i + i; j < candidates.size(); j += i) {
      candidates[j] = false;
    }
  }

  for (std::vector<bool>::size_type i = finish_point+1;
                                    i < candidates.size(); ++i) {
    if (candidates[i])
      results.push_back(i);
  }

  results.shrink_to_fit();
  return results;
}

template <typename T>
std::vector<T> LibEuler::FibsUnderNumber(T limit) {
  T a = 1, b = 2;
  std::vector<T> result {a, b};

  while (b <= limit) {
    result.push_back(a+b);
    a = b;
    b = result.back();
  }

  return result;
}
