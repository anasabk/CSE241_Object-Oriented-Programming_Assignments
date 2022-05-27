#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <cstdlib>
#include "SoloS_lib.hpp"

using namespace std;

/*Prints the board with row and column markings*/
void printBoard (board targetBoard)
{
    int flag_3D = 0,
        column,
        row;

    char column_c;

    /*Printing mode for square shaped cells*/
    if (targetBoard.boardType == Shape::square)
    {
        cout << "     ";

        /*print the column letters first*/
        for ((column = 0) , (column_c = 'a') ; column < targetBoard.boardState.size() ; (++column) && (++column_c))
            cout << column_c << ' ';

        cout << "\n\n";

        /*then print the row numbers with the cells iteratively*/
        for (int row = 0; row < targetBoard.boardState.size(); ++row)
        {
            cout << "  " << row + 1 << "  ";

            for (cellState cell : targetBoard.boardState[row])
                cout << (char)cell << ' ';

            cout << endl;                
        }
    }

    /*Printing mode for hexagon shaped cells, they have a unique structure*/
    else if (targetBoard.boardType == Shape::hexagon)
    {
        cout << "      ";

        /*print the first line of column letters*/
        for ((column = 0) , (column_c = 'a') ; column < targetBoard.boardState.size()-1 ; (++column) , (++column_c))
            cout << column_c << ' ';
        
        cout << endl << "     ";

        /*then the next one*/
        for ((column = 0) , (column_c = 'a') ; column < targetBoard.boardState.size() ; (++column) , (++column_c))
            cout << column_c << ' ';

        cout << endl << endl;
        
        /*print the cells*/
        for (row = 0; row < targetBoard.boardState.size(); ++row)
        {
            cout << "  " << row + 1 << "  ";

            /*print an extra space for the smaller rows to fix the alignment, since the number of cells in the rows (for example) iterates between 7 and 6 cells in a 7x7 board*/
            if (targetBoard.boardState[row].size() < targetBoard.boardState.size()) cout << ' ';
 
            for (cellState cell : targetBoard.boardState[row])
                cout << (char)cell << ' ';

            cout << endl;                
        }
    }
}

/*Do the requested action in the targetBoard*/
bool move_peg (action_t action, board &targetBoard)
{
    /*movement mode for hexagon shaped cells, only used when travelng vertically, use normal algorithm for traveling horizontally*/
    if (targetBoard.boardType == Shape::hexagon && action.direction[1] == 0)
    {
        /*check if there is a peg in the cell*/
        if (targetBoard.boardState[action.row][action.column] == cellState::peg)
        {
            /*check if the target row is the bigger row*/
            if (targetBoard.boardState[action.row].size() == targetBoard.boardState.size())
            {
                /*move to the right side of the vertical movement when available*/
                if (targetBoard.boardState[action.row + action.direction[0]][action.column] == cellState::peg &&
                    targetBoard.boardState[action.row + action.direction[0]*2][action.column + 1] == cellState::empty)
                    {
                        targetBoard.boardState[action.row][action.column] = targetBoard.boardState[action.row + action.direction[0]][action.column] = cellState::empty;
                        targetBoard.boardState[action.row + action.direction[0]*2][action.column + 1] = cellState::peg;
                        return true;
                    }
                
                /*move to the left side of the vertical movement when available*/
                else if (targetBoard.boardState[action.row + action.direction[0]][action.column + action.direction[1] - 1] == cellState::peg &&
                         targetBoard.boardState[action.row + action.direction[0]*2][action.column + action.direction[1]*2 - 1] == cellState::empty)
                        {
                            targetBoard.boardState[action.row][action.column] = targetBoard.boardState[action.row + action.direction[0]][action.column - 1] = cellState::empty;
                            targetBoard.boardState[action.row + action.direction[0]*2][action.column - 1] = cellState::peg;
                            return true;
                        }

                else return false;
            }

            /*if the row is the smaller row, the measurements for vertical left and right change a bit*/
            else
            {
                /*move to the right side of the vertical movement when available*/
                if (targetBoard.boardState[action.row + action.direction[0]][action.column] == cellState::peg &&
                    targetBoard.boardState[action.row + action.direction[0]*2][action.column - 1] == cellState::empty)
                    {
                        targetBoard.boardState[action.row][action.column] = targetBoard.boardState[action.row + action.direction[0]][action.column] = cellState::empty;
                        targetBoard.boardState[action.row + action.direction[0]*2][action.column - 1] = cellState::peg;
                        return true;
                    }
                
                /*move to the left side of the vertical movement when available*/
                else if (targetBoard.boardState[action.row + action.direction[0]][action.column + 1] == cellState::peg &&
                         targetBoard.boardState[action.row + action.direction[0]*2][action.column + 1] == cellState::empty)
                        {
                            targetBoard.boardState[action.row][action.column] = targetBoard.boardState[action.row + action.direction[0]][action.column + 1] = cellState::empty;
                            targetBoard.boardState[action.row + action.direction[0]*2][action.column + 1] = cellState::peg;
                            return true;
                        }

                else return false;
            }
        }

        else return false;
    }

    /*normal movement mode for square shaped cells and traveling horizontally in hexagon shaped cells*/
    else
    {
        /*check if the action is viable then perform it*/
        if (check_movement(action,targetBoard))
        {
            targetBoard.boardState[action.row][action.column] = targetBoard.boardState[action.row + action.direction[0]][action.column + action.direction[1]] = cellState::empty;
            targetBoard.boardState[action.row + action.direction[0]*2][action.column + action.direction[1]*2] = cellState::peg;
            return true;
        }

        else return false;
    }
}

/*Check if the action is viable, returns true or false*/
bool check_movement (action_t action, board targetBoard)
{
    /*check if the target row and column are in the range*/
    if (action.row > targetBoard.boardState.size() || action.column > targetBoard.boardState[action.row].size())  
        return false;

    /*check if the action direction is in the range*/
    if (action.column + action.direction[1]*2 >= targetBoard.boardState[action.row].size() || action.column + action.direction[1]*2 < 0 ||
        action.row + action.direction[0]*2 >= targetBoard.boardState.size() || action.row + action.direction[0]*2 < 0)
        return false;

    /*movement mode for hexagon shaped cells, only used when travelng vertically, use normal algorithm for traveling horizontally*/
    if (targetBoard.boardType == Shape::hexagon && action.direction[1] == 0)
    {
        /*check if there is a peg in the cell*/
        if (targetBoard.boardState[action.row][action.column] == cellState::peg)
        {
            /*check if the target row is the bigger row*/
            if (targetBoard.boardState[action.row].size() == targetBoard.boardState.size())
            {
                /*check the right side of the vertical movement if it's available*/
                if (targetBoard.boardState[action.row + action.direction[0]][action.column + action.direction[1]] == cellState::peg &&
                    targetBoard.boardState[action.row + action.direction[0]*2][action.column + action.direction[1]*2 + 1] == cellState::empty)
                    return true;
                
                /*check the left side of the vertical movement if it's available*/
                else if (targetBoard.boardState[action.row + action.direction[0]][action.column + action.direction[1] - 1] == cellState::peg &&
                         targetBoard.boardState[action.row + action.direction[0]*2][action.column + action.direction[1]*2 - 1] == cellState::empty)
                        return true;

                else return false;
            }

            /*if the row is the smaller row, the measurements for vertical left and right change a bit*/
            else
            {
                /*check the right side of the vertical movement if it's available*/
                if (targetBoard.boardState[action.row + action.direction[0]][action.column + action.direction[1]] == cellState::peg &&
                    targetBoard.boardState[action.row + action.direction[0]*2][action.column + action.direction[1]*2 - 1] == cellState::empty)
                    return true;
                
                /*check the left side of the vertical movement if it's available*/
                else if (targetBoard.boardState[action.row + action.direction[0]][action.column + action.direction[1] + 1] == cellState::peg &&
                         targetBoard.boardState[action.row + action.direction[0]*2][action.column + action.direction[1]*2 + 1] == cellState::empty)
                        return true;

                else return false;
            }
        }

        else return false;
    }

    /*normal movement mode for square shaped cells and traveling horizontally in hexagon shaped cells*/
    else if (targetBoard.boardState[action.row][action.column] == cellState::peg &&
             targetBoard.boardState[action.row + action.direction[0]][action.column + action.direction[1]] == cellState::peg &&
             targetBoard.boardState[action.row + action.direction[0]*2][action.column + action.direction[1]*2] == cellState::empty)
            return true;

    else return false;
}

/*Translate the string command of the action to an action_t struct*/
bool translate_movement (string &movement, action_t &new_action)
{
    /*transform the column letters to integers, weither they are capital or small letters and store them as the column*/
    if (movement[0] >= 'A' && movement[0] <= 'Z')   new_action.column = movement[0] - 'A';
    else if (movement[0] >= 'a' || movement[0] <= 'z')    new_action.column = movement[0] - 'a';
    else return false;
    
    /*store the integer value of the row*/
    if (movement[1] > 0) new_action.row = movement[1] - '1';
    else return false;

    /*store the direction of the action based on the letter of the given direction*/
    if (movement[3] == 'U' || movement[3] == 'u')   new_action.direction = {-1, 0};
    else if (movement[3] == 'D' || movement[3] == 'd')   new_action.direction = {1, 0};
    else if (movement[3] == 'L' || movement[3] == 'l')   new_action.direction = {0, -1};
    else if (movement[3] == 'R' || movement[3] == 'r')   new_action.direction = {0, 1};
    else return false;
    
    return true;
}

/*Check the progress of the player, return the final score when no moveable peg are found*/
int progress_checker (board targetBoard)
{
    int moveable = 0, unmovable = 0;
    array<array<int, 2>, 4> direction_list = { {{-1, 0}, {1, 0}, {0, -1}, {0, 1}} };
    action_t temp_action;

    /*iterate through the cells of the board, and test each direction for valid movements*/
    for (temp_action.row = 0 ; temp_action.row < targetBoard.boardState.size() ; ++temp_action.row)
        for (temp_action.column = 0; temp_action.column < targetBoard.boardState[temp_action.row].size() ; ++temp_action.column)
        {
            for (array<int, 2> direction : direction_list)
            {
                temp_action.direction = direction;

                if (check_movement(temp_action, targetBoard))
                {
                    ++moveable;
                    break;
                }
            }

            if (targetBoard.boardState[temp_action.row][temp_action.column] == cellState::peg &&
                temp_action.direction == direction_list[3])
                ++unmovable;
        }

    if (moveable == 0) return unmovable;

    else return 0;
}

/*Randomly generate a valid movement automatically*/
void botMovementEngine (board &targetBoard) 
{
    array<array<int, 2>, 4> direction_list = { {{-1, 0}, {1, 0}, {0, -1}, {0, 1}} };
    action_t temp_action;

    for (temp_action.row = 0 ; temp_action.row < targetBoard.boardState.size() ; ++temp_action.row)
        for (temp_action.column = 0; temp_action.column < targetBoard.boardState[temp_action.row].size() ; ++temp_action.column)
            for (array<int, 2> direction : direction_list)
            {
                temp_action.direction = direction;
                
                /*used rand as a switch to simulate random chances for every movement to happen*/
                if (rand()%2 == 1 && check_movement(temp_action, targetBoard))
                {
                    /*perform the chosen action*/
                    move_peg (temp_action, targetBoard);
                    printBoard (targetBoard);
                    return;
                }
            }
}


void soloSolitaire_UI ()
{
/* Initializing the boards */
    board   targetBoard;
    string movement, player;
    action_t new_action;
    vector<board> board_list;
    int board_num, unmoveables, counter;

    /*create a random seed using malloc for random numbers*/
    srand((long)malloc(1));

    cout << "\n ------ Welcome to Solo Solitaire game ------ " << endl;

    /*store every board in a vector for the player to choose from*/
    targetBoard.boardState ={{cellState::null, cellState::null, cellState::peg,   cellState::peg, cellState::peg, cellState::null, cellState::null},
                                {cellState::null,  cellState::peg, cellState::peg,   cellState::peg, cellState::peg,  cellState::peg, cellState::null},
                                { cellState::peg,  cellState::peg, cellState::peg, cellState::empty, cellState::peg,  cellState::peg,  cellState::peg},
                                { cellState::peg,  cellState::peg, cellState::peg,   cellState::peg, cellState::peg,  cellState::peg,  cellState::peg},
                                { cellState::peg,  cellState::peg, cellState::peg,   cellState::peg, cellState::peg,  cellState::peg,  cellState::peg},
                                {cellState::null,  cellState::peg, cellState::peg,   cellState::peg, cellState::peg,  cellState::peg, cellState::null},
                                {cellState::null, cellState::null, cellState::peg,   cellState::peg, cellState::peg, cellState::null, cellState::null}};
    
    targetBoard.boardType = Shape::square;

    board_list.push_back(targetBoard);

        
    targetBoard.boardState ={{cellState::null, cellState::null, cellState::null, cellState::peg,   cellState::peg, cellState::peg, cellState::null, cellState::null, cellState::null},
                                {cellState::null, cellState::null, cellState::null, cellState::peg,   cellState::peg, cellState::peg, cellState::null, cellState::null, cellState::null},
                                {cellState::null, cellState::null, cellState::null, cellState::peg,   cellState::peg, cellState::peg, cellState::null, cellState::null, cellState::null},
                                { cellState::peg,  cellState::peg,  cellState::peg, cellState::peg,   cellState::peg, cellState::peg,  cellState::peg,  cellState::peg,  cellState::peg},
                                { cellState::peg,  cellState::peg,  cellState::peg, cellState::peg, cellState::empty, cellState::peg,  cellState::peg,  cellState::peg,  cellState::peg},
                                { cellState::peg,  cellState::peg,  cellState::peg, cellState::peg,   cellState::peg, cellState::peg,  cellState::peg,  cellState::peg,  cellState::peg},
                                {cellState::null, cellState::null, cellState::null, cellState::peg,   cellState::peg, cellState::peg, cellState::null, cellState::null, cellState::null},
                                {cellState::null, cellState::null, cellState::null, cellState::peg,   cellState::peg, cellState::peg, cellState::null, cellState::null, cellState::null},
                                {cellState::null, cellState::null, cellState::null, cellState::peg,   cellState::peg, cellState::peg, cellState::null, cellState::null, cellState::null}};

    targetBoard.boardType = Shape::square;

    board_list.push_back(targetBoard);


    targetBoard.boardState ={{cellState::null, cellState::null, cellState::peg,   cellState::peg, cellState::peg, cellState::null, cellState::null, cellState::null},
                                {cellState::null, cellState::null, cellState::peg,   cellState::peg, cellState::peg, cellState::null, cellState::null, cellState::null},
                                {cellState::null, cellState::null, cellState::peg,   cellState::peg, cellState::peg, cellState::null, cellState::null, cellState::null},
                                { cellState::peg,  cellState::peg, cellState::peg,   cellState::peg, cellState::peg,  cellState::peg,  cellState::peg,  cellState::peg},
                                { cellState::peg,  cellState::peg, cellState::peg, cellState::empty, cellState::peg,  cellState::peg,  cellState::peg,  cellState::peg},
                                { cellState::peg,  cellState::peg, cellState::peg,   cellState::peg, cellState::peg,  cellState::peg,  cellState::peg,  cellState::peg},
                                {cellState::null, cellState::null, cellState::peg,   cellState::peg, cellState::peg, cellState::null, cellState::null, cellState::null},
                                {cellState::null, cellState::null, cellState::peg,   cellState::peg, cellState::peg, cellState::null, cellState::null, cellState::null}};

    targetBoard.boardType = Shape::square;

    board_list.push_back(targetBoard);


    targetBoard.boardState ={{cellState::null, cellState::null, cellState::peg,   cellState::peg, cellState::peg, cellState::null, cellState::null},
                                {cellState::null, cellState::null, cellState::peg,   cellState::peg, cellState::peg, cellState::null, cellState::null},
                                { cellState::peg,  cellState::peg, cellState::peg,   cellState::peg, cellState::peg,  cellState::peg,  cellState::peg},
                                { cellState::peg,  cellState::peg, cellState::peg, cellState::empty, cellState::peg,  cellState::peg,  cellState::peg},
                                { cellState::peg,  cellState::peg, cellState::peg,   cellState::peg, cellState::peg,  cellState::peg,  cellState::peg},
                                {cellState::null, cellState::null, cellState::peg,   cellState::peg, cellState::peg, cellState::null, cellState::null},
                                {cellState::null, cellState::null, cellState::peg,   cellState::peg, cellState::peg, cellState::null, cellState::null}};

    targetBoard.boardType = Shape::square;

    board_list.push_back(targetBoard);


    targetBoard.boardState ={{cellState::null, cellState::null, cellState::null, cellState::null,   cellState::peg, cellState::null, cellState::null, cellState::null, cellState::null},
                                {cellState::null, cellState::null, cellState::null,  cellState::peg,   cellState::peg,  cellState::peg, cellState::null, cellState::null, cellState::null},
                                {cellState::null, cellState::null,  cellState::peg,  cellState::peg,   cellState::peg,  cellState::peg,  cellState::peg, cellState::null, cellState::null},
                                {cellState::null,  cellState::peg,  cellState::peg,  cellState::peg,   cellState::peg,  cellState::peg,  cellState::peg,  cellState::peg, cellState::null},
                                { cellState::peg,  cellState::peg,  cellState::peg,  cellState::peg, cellState::empty,  cellState::peg,  cellState::peg,  cellState::peg,  cellState::peg},
                                {cellState::null,  cellState::peg,  cellState::peg,  cellState::peg,   cellState::peg,  cellState::peg,  cellState::peg,  cellState::peg, cellState::null},
                                {cellState::null, cellState::null,  cellState::peg,  cellState::peg,   cellState::peg,  cellState::peg,  cellState::peg, cellState::null, cellState::null},
                                {cellState::null, cellState::null, cellState::null,  cellState::peg,   cellState::peg,  cellState::peg, cellState::null, cellState::null, cellState::null},
                                {cellState::null, cellState::null, cellState::null, cellState::null,   cellState::peg, cellState::null, cellState::null, cellState::null, cellState::null}};

    targetBoard.boardType = Shape::square;

    board_list.push_back(targetBoard);


    targetBoard.boardState ={{cellState::null, cellState::null, cellState::empty, cellState::null, cellState::null},
                                {        cellState::null,  cellState::peg,   cellState::peg, cellState::null         },
                                {cellState::null,  cellState::peg,   cellState::peg,  cellState::peg, cellState::null},
                                {         cellState::peg,  cellState::peg,   cellState::peg,  cellState::peg         },
                                { cellState::peg,  cellState::peg,   cellState::peg,  cellState::peg,  cellState::peg}};
    
    targetBoard.boardType = Shape::hexagon;

    board_list.push_back(targetBoard);


    targetBoard.boardState ={{          cellState::null,  cellState::peg,  cellState::peg,   cellState::peg,  cellState::peg, cellState::null        },
                                {cellState::null,  cellState::peg,  cellState::peg,  cellState::peg,   cellState::peg,  cellState::peg,  cellState::null},
                                {          cellState::peg,  cellState::peg,  cellState::peg,   cellState::peg,  cellState::peg,  cellState::peg         },
                                {cellState::peg,  cellState::peg,  cellState::peg,  cellState::empty,   cellState::peg,  cellState::peg,  cellState::peg},
                                {          cellState::peg,  cellState::peg,  cellState::peg, cellState::peg,  cellState::peg,  cellState::peg           },
                                {cellState::null,  cellState::peg,  cellState::peg,  cellState::peg,   cellState::peg,  cellState::peg,  cellState::null},
                                {          cellState::null,  cellState::peg,  cellState::peg,   cellState::peg,  cellState::peg,  cellState::null       }};

    targetBoard.boardType = Shape::hexagon;

    board_list.push_back(targetBoard);

    /*keep playing until being prompted to leave*/
    while(1)
    {
        /*selecting a board*/
        counter = 0;
        for (board targetBoard : board_list)
        {   
            ++counter;
            cout << "\n--------------------------\n" << counter << "-\n";
            printBoard (targetBoard);
            cout << endl;
        }
        
        cout << "Please choose a board from the following:\n";

        cout << "\n(0 to exit)\n";

        cin >> movement;

        board_num = movement[0] - '0';

        /*check if the board number is valid*/
        if (board_num < 0 || board_num > board_list.size())
        {
            cout << "Try again\n";
            continue;
        }
        
        else if (board_num == 0) return;

        targetBoard = board_list[board_num - 1];


        /*ask who is going to play, a bot or a human*/
        cout << "Do you want to play? Or do you want to see how intelligent is your PC?\n (Type: \"me\" for you, \"bot\" for the computer, choose wisely\n";

        cin >> player;

        if (player == "me")
            /*keep playing until surrendering or winning*/
            while(1)
            {
                cout << "\n--------------------------\n";

                /*print the board after each action*/
                printBoard (targetBoard);

                cout << " Type the movement you want as in the example:\n b4-U (move the peg in b4 upwards)\n (Type -1 to surrender)\n ";
                cin >> movement;

                /*surrender when the player types -1*/
                if (movement == "-1")   break;

                /*process the movement*/
                if (translate_movement(movement, new_action) == false || 
                    move_peg(new_action, targetBoard) == false)
                {
                    cout << "\n Invalid movement, please try again.\n ";
                    continue;
                }

                /*calculate the score*/
                unmoveables = progress_checker (targetBoard);

                /*unmovable pegs number is only known when no action can be performed*/
                if (unmoveables > 0)
                {
                    cout << "\n--------------------------\n";
                    printBoard (targetBoard);
                    cout << "\n Congratulations! You won, your score is " << unmoveables << endl;
                    break;
                }
            }

        else if (player == "bot")
            /*keep playing until the bot wins*/
            while(1)
            {
                cout << "\n--------------------------\n";
                /*generate and perform a random action*/
                botMovementEngine(targetBoard);

                /*calculate the score*/
                unmoveables = progress_checker (targetBoard);

                /*unmovable pegs number is only known when no action can be performed*/
                if (unmoveables > 0)
                {
                    cout << "\n--------------------------\n";
                    printBoard (targetBoard);
                    cout << "\n The bot won with the score " << unmoveables << endl;
                    break;
                }
            }

        else    cout << "Try again\n";
    }
}


int main ()
{
    soloSolitaire_UI ();
}