#ifndef __BITS_HPP__
#define __BITS_HPP__

#include <cstdint>

namespace util
{
  class bits 
  {
    static uint8_t index64[64];

  public:
    /**
     * bit_scan_forward
     * @author Kim Walisch (2012)
     * @param bb bitboard to scan
     * @precondition bb != 0
     * @return index (0..63) of least significant one bit
     */
    static inline int bit_scan_forward(uint64_t bb) 
    {
      const uint64_t debruijn64 = uint64_t(0x03f79d71b4cb0a89);
      return index64[((bb ^ (bb-1)) * debruijn64) >> 58];
    }
  };
}
#endif
