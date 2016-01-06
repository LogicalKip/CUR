#include "../include/test_fast_log.hpp"
#include <cmath>
#include <chrono>
#include <iostream>
#include <random>

using namespace std;

namespace util
{
  test_fast_log::test_fast_log(unsigned int nb_samples, unsigned int mantissa_nb_bits) 
    : nb_samples(nb_samples), flog(mantissa_nb_bits)
  {
    precision();
    time();
  }

  void test_fast_log::precision()
  {
    default_random_engine generator;
    uniform_real_distribution<float> distribution(0.01,1000000.0);
    double res = 0;
    for (unsigned int i = 0; i < nb_samples; ++i) 
      {
	float v = distribution(generator); 
	res += abs(log(v) - flog.log(v)) / (double)v;
      }
    cout << "precision: " << res / nb_samples << endl;
  }
   
  void test_fast_log::time()
  {
    volatile double res = 0;
    chrono::steady_clock::time_point start = chrono::steady_clock::now();
    for (unsigned int i = 1; i <= nb_samples; ++i) 
      {
	res += log(i);
      }
    cout << "time for log (ms): " << chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - start).count() << endl;

    res = 0;
    start = chrono::steady_clock::now();
    for (unsigned int i = 1; i <= nb_samples; ++i) 
      {
	res += flog.log(i);
      }
    cout << "time for fast log (ms): " << chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - start).count() << endl;
    
  }
}
