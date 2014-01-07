#include <cmath>
#include <cstdint>
#include <future>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
#include <queue>

#include <typeinfo>

#include "./libeuler.h"

typedef uint_fast64_t (*EulerProblem)();

std::mutex answer_mutex;
std::mutex job_mutex;

std::vector<uint_fast64_t> primes;
std::queue<std::function<void()>> jobs;
std::vector<std::thread> threads;

const uint_fast64_t max_prime = 2000000;

uint_fast64_t problem1() {
  uint_fast16_t limit = 1000;
  uint_fast64_t result = 0;

  for (uint_fast16_t i= 1; i < limit; ++i)
    if (i % 3 == 0 || i % 5 == 0)
      result += i;

  return result;
}

uint_fast64_t problem2() {
  uint_fast64_t sum = 0;

  for (auto i : LibEuler::FibsUnderNumber(4000000)) {
    if (i % 2 == 0)
      sum += i;
  }
  return sum;
}

uint_fast64_t problem3() {
  uint_fast64_t max = 0, to_be_factored = 600851475143;
  for (auto prime : primes) {
    if (to_be_factored % prime == 0) {
      max = prime;
    }
  }
  return max;
}

uint_fast64_t problem6() {
  uint_fast64_t n = 100;
  uint_fast64_t sum = n * (n + 1) / 2;
  uint_fast64_t square_of_sum = sum * sum;
  for (uint_fast64_t i = 1; i <= n; ++i)
    square_of_sum -= (i * i);
  return square_of_sum;
}

uint_fast64_t problem7() {
  return primes[10000];
}

uint_fast64_t problem9() {
  uint_fast16_t a, b, c;
  bool found = false;
  for (a = 1; a < 1000; ++a) {
    for (b = a; b < 1000; ++b) {
      c = 1000 - a - b;
      if (a*a + b*b == c*c) {
        found = true;
        break;
      }
    }
    if (found) break;
  }
  return a * b * c;
}

uint_fast64_t problem10() {
  uint_fast64_t result = 0;
  auto curPrime = primes.begin();

  do {
    result += *curPrime;
    ++curPrime;
  } while (curPrime != primes.end() && *curPrime < 2000000 );

  return result;
}

uint_fast64_t problem16() {
  double power = pow(2, 1000);
  uint_fast64_t result = 0;
  char buffer[1024];

  snprintf(buffer, sizeof(buffer), "%.0lf", power);
  for (char* i = buffer; *i != '\0'; ++i) {
    result += *i - '0';
  }
  return result;
}

void run_problem(uint_fast16_t prob_num, EulerProblem func) {
  auto result = func();
  answer_mutex.lock();
  std::cout << prob_num << " answer = " << result << std::endl;
  answer_mutex.unlock();
}

void queue_jobs() {
  jobs.push(std::bind(run_problem,  1, *problem1));
  jobs.push(std::bind(run_problem,  2, *problem2));
  jobs.push(std::bind(run_problem,  3, *problem3));
  jobs.push(std::bind(run_problem,  6, *problem6));
  jobs.push(std::bind(run_problem,  7, *problem7));
  jobs.push(std::bind(run_problem,  9, *problem9));
  jobs.push(std::bind(run_problem, 10, *problem10));
  jobs.push(std::bind(run_problem, 16, *problem16));
}

std::function<void()> pull_job() {
  job_mutex.lock();
  if (jobs.size() > 0) {
    auto y = jobs.front();
    jobs.pop();
    job_mutex.unlock();
    return y;
  } else {
    return []() {};
  }
}

void run_jobs() {
  while (!jobs.empty()) {
    pull_job()();
  }
}

int main() {
  primes = LibEuler::PrimeNumGen<uint_fast64_t>(max_prime);
  queue_jobs();
  for (uint_fast8_t i = 0; i < std::thread::hardware_concurrency(); ++i) {
    threads.push_back(std::thread(run_jobs));
  }
  for (auto thread = threads.begin(); thread != threads.end(); ++thread) {
    thread->join();
  }
}
