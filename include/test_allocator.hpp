#ifndef __TEST_ALLOCATOR_HPP__
#define __TEST_ALLOCATOR_HPP__

#include "allocator.hpp"
#include <vector>
#include <string>
#include <random>

namespace mcts
{
  class test_allocator
  {
    allocator alloc;
    std::default_random_engine generator;
    
    void allocate(unsigned int depth, unsigned int nb_children);
    void allocate_multi_threaded(unsigned int depth, unsigned int nb_children);
    void build(unsigned int depth, unsigned int nb_children, node* n);
    void collect(std::vector<node*>& nodes, node* n); 
    bool check(std::vector<node*> nodes, unsigned int depth, unsigned int nb_children);
    void test(node* root, unsigned int depth, unsigned int nb_children, const std::string& msg);

  public:
    test_allocator(unsigned int depth = 3, unsigned int nb_children = 2);
    std::string to_string() const;
  };
}

#endif
