#include <iostream>
#include <vector>
#include <array>

using namespace std;


enum struct cellState_t {
    null = ' ',
    peg = 'P',
    empty = '.'
};

enum struct cellShape_t {
    square,
    hexagon
};

enum struct player_t {
    bot,
    human
};

struct game_t {
    vector<vector<cellState_t>> boardState;
    cellShape_t boardType;
    int totalMoves;
    player_t playerType;
};

struct action_t {
    int row, column;
    array<int, 2> direction;
};

/*Compares a certain number of characters between two strings*/
bool strncmp (const string &str1, const string &str2, int numOfChar);

/*Prints the board with row and column markings to the terminal*/
void printBoard (const game_t &currentGame);

/*Prints the board to a file*/
bool printBoard (const game_t &currentGame, ofstream &targetFile);

/*Do the requested action in the currentGame*/
bool move_peg (const action_t &action, game_t &currentGame);

/*Check if the action is viable, returns true or false*/
bool check_movement (const action_t &action, const game_t &currentGame);

/*Translate the string command of the action to an action_t struct*/
bool translate_movement (const string &movement, action_t &new_action);

/*Check the progress of the player, return the final score when no moveable peg are found*/
int progress_checker (const game_t &currentGame);

/*Randomly generate a valid movement and do it*/
bool botMovementEngine (game_t &currentGame);

/*Save the current game to a .txt file with all it's information*/
bool saveToFile (const game_t &currentGame, const string &fileName);

/*Read a game from a file and return it with it's info*/
bool loadFile (game_t &currentGame, const string &fileName);

/*Manage the game with the computer as a player*/
void botGameManager (game_t &currentGame);

/*Manage the game with a human as a player*/
void humanGameManager (game_t &currentGame);

/*Main UI of the game*/
void soloSolitaire_UI ();