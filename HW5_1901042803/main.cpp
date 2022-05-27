#include <iostream>
#include <vector>
#include <cstdlib>
#include "header.h"

using std::cout;
using std::cerr;
using std::cin;
using std::ofstream;
using std::vector;
using std::endl;
using namespace Gaming;

int main()
{
    int *seed = new (int);
    srand((long)seed);
    vector<BoardGame2D *> gamelist;

    gamelist.push_back(new PegSolitaire());
    gamelist.push_back(new PegSolitaire());
    gamelist.push_back(new EightPuzzle());
    gamelist.push_back(new EightPuzzle());
    gamelist.push_back(new Klotski());
    gamelist.push_back(new Klotski());

    for (auto tempGame : gamelist) tempGame->playAutoAll();

    delete seed;
}