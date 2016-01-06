#ifndef __POW_HPP_
#define __POW_HPP_

namespace util
{
  long pow_l(long x, long n)
  {
    long res = 1;
    while (n != 0)
      {
	if (n & 1) 
	  {
	    n--;
	    res = res * x;
	  }
	else
	  {
	    n = n / 2;
	    x = x * x;
	  }
      }
    return res;
  }
}
#endif
