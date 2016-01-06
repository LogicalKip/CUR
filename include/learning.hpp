#ifndef __LEARNING_HPP__
#define __LEARNING_HPP__

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <set>

namespace util
{
  struct learning
  {
    template <typename Game>
    static void display_file(Game& game, const std::string& filename);
  };
  template <typename Game>
  void learning::display_file(Game& game, const std::string& filename)
  {
    using namespace std;
    ifstream file(filename);
    string line;
    while (getline(file, line))
      {
	cout << line << endl;
	double value;
	set<int> input_vector;
	int index, v;
	char c;
	stringstream ss(line);
	ss >> value;
	cout << "game value: " << value << endl;
	while (ss >> index >> c >> v) 
	  {
	    input_vector.insert(index);
	  }
	game.from_input_vector(input_vector);
	cout << "player to move: " << game.player_to_string(game.current_player()) << endl;
	cout << game << endl;
	getline(cin, line);
      }
    file.close();
  }
}

#endif
