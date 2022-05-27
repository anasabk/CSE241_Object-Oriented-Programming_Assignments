#include <iostream>
#include <vector>
#include <array>

using namespace std;


enum struct cellState {
    null = ' ',
    peg = 'P',
    empty = '.'
};

enum struct Shape {
    square,
    hexagon
};

struct board {
    vector<vector<cellState>> boardState;
    Shape boardType;
};

struct action_t {
    int row, column;
    array<int, 2> direction;
};

void printBoard (board targetBoard);

bool move_peg (action_t action, board &targetBoard);

bool check_movement (action_t action, board targetBoard);

bool translate_movement (string &movement, action_t &new_action);

int progress_checker (board targetBoard);

void botMovementEngine (board &targetBoard);

void soloSolitaire_UI ();