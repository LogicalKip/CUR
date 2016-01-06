#include "../include/test_bits.hpp"
#include <string.h>
#include <chrono>
#include <iostream>
#include <random>

using namespace std;

namespace util
{
  test_bits::test_bits(unsigned int nb_samples)
  {
    volatile int res = 0;
    default_random_engine generator(11);
    uniform_int_distribution<uint64_t> distribution(0, uint64_t(0xFFFFFFFFFFFFFFFF));

    chrono::steady_clock::time_point start = chrono::steady_clock::now();
    for (unsigned int i = 1; i <= nb_samples; ++i) 
      {
    	uint64_t v = distribution(generator);
    	res += bits::bit_scan_forward(v);
      }
    cout << "res for debrujin: " << res << endl;
    cout << "time for debrujin (ms): " << chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - start).count() << endl;

    generator = default_random_engine(11);
    res = 0;
    start = chrono::steady_clock::now();
    for (unsigned int i = 1; i <= nb_samples; ++i) 
      {
    	uint64_t v = distribution(generator);
    	res += ffsll(v) - 1;
      }
    cout << "res for ffsll: " << res << endl;
    cout << "time for ffsll (ms): " << chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - start).count() << endl;    
  }
}
