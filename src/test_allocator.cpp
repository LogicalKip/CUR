#include "../include/test_allocator.hpp"
#include <algorithm>
#include "../include/pow.hpp"
#include <iostream>
//#include <thread>
#include <random>
#include "../include/display_node.hpp"

using namespace std;

namespace mcts
{
  test_allocator::test_allocator(unsigned int depth, unsigned int nb_children)
  {
    allocate(depth, nb_children);
    allocate_multi_threaded(depth, nb_children);
    alloc.clear();
    allocate(depth, nb_children);
    allocate_multi_threaded(depth, nb_children);
    alloc.clear();
    node* root = alloc.allocate(1);
    build(depth, nb_children, root);
    cout << "size: " << alloc.size() << endl;
    cout << "free space: " << alloc.free_space() << endl;
    cout << root << endl << *root << endl;
    util::display_node::node_to_dot(cout, root);
    node* new_root = alloc.move(root);
    cout << "free space: " << alloc.free_space() << endl;
    cout << new_root << endl << *new_root << endl;
  }

  void test_allocator::build(unsigned int depth, unsigned int nb_children, node* n)
  {
    uniform_int_distribution<int> dint(1, nb_children);
    statistics& stats = n->get_statistics_ref();
    stats.count = dint(generator);
    uniform_real_distribution<float> dfloat(-1, 1);
    stats.value = dfloat(generator);
    if (depth == 0) return;
    node* children = alloc.allocate(nb_children);
    n->set_children(children);
    n->set_number_of_children(nb_children); 
    for (unsigned int i = 0; i < nb_children; ++i) 
      {
	build(depth - 1, nb_children, children + i);
      }
  }
  
  void test_allocator::collect(vector<node*>& nodes, node* n)
  {
    if (n == nullptr) return;
    nodes.push_back(n);
    uint16_t nb_children = n->get_number_of_children();
    node* children = n->get_children();
    for (unsigned int i = 0; i < nb_children; ++i) 
      {
	collect(nodes, children + i);
      }
  }

  bool test_allocator::check(vector<node*> nodes, unsigned int depth, unsigned int nb_children)
  {
    if ((int)nodes.size() != 1 + (util::pow_l(nb_children, depth + 1) - nb_children) / (nb_children - 1)) return false; 
    sort(nodes.begin(), nodes.end());
    for (unsigned int i = 1; i < nodes.size(); ++i) 
      {
	if (nodes[i-1] + 1 != nodes[i]) return false;
      }
    return true;
  }

  void test_allocator::test(node* root, unsigned int depth, unsigned int nb_children, const string& msg)
  {
    vector<node*> nodes;
    collect(nodes, root);
    if (check(nodes, depth, nb_children)) std::cout << msg + " succeeded" << std::endl;
    else std::cout << msg + " failed" << std::endl;
  }

  void test_allocator::allocate(unsigned int depth, unsigned int nb_children)
  {
    node* root = alloc.allocate(1);
    build(depth, nb_children, root);
    test(root, depth, nb_children, "allocate");
  }

  void test_allocator::allocate_multi_threaded(unsigned int depth, unsigned int nb_children)
  {
    node* root = alloc.allocate(1);
    if (depth != 0)
      {
	root->set_number_of_children(nb_children);
	node* children = alloc.allocate(nb_children);
	root->set_children(children);
	// vector<thread> threads;
	// for (unsigned int i = 0; i < nb_children; ++i)
	//   {
	//     threads.push_back(thread(&test_allocator::build, this, depth - 1, nb_children, children + i));
	//   }
	// for_each(threads.begin(), threads.end(), mem_fn(&thread::join));
#pragma omp parallel for
	for (unsigned int i = 0; i < nb_children; ++i)
	  {
	    build(depth - 1, nb_children, children + i);
	  }
      }
    test(root, depth, nb_children, "allocate multi threaded");
  }
}
