I wanted to learn how concurrency works in the new C++11 so I came up with this.
Runs in .02s with 4 threads on my machine when compiled with O3.

Shared data for now is mutexes, primes, and jobs.
