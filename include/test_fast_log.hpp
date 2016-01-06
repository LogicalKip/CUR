#ifndef __TEST_FAST_LOG_HPP__
#define __TEST_FAST_LOG_HPP__

#include "fast_log.hpp"

namespace util
{
  class test_fast_log
  {
    unsigned int nb_samples;
    fast_log flog;

    void precision();
    void time();
  public:
    test_fast_log(unsigned int nb_samples = 100000000U, unsigned int mantissa_nb_bits = 11);
  };
}
#endif
