#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <fstream>
#include <cstdlib>
#include "SoloS_lib.hpp"

using namespace std;


/*Compares a certain number of characters between two strings*/
bool strncmp (const string &str1, const string &str2, int numOfChar)
{
    /*Check if the number of characters is valid in the strings*/
    if (numOfChar >= str1.size() && numOfChar >= str2.size())
        return false;

    /*Compare the strings character by character*/
    for (; numOfChar > 0 ; --numOfChar)
        if (str1[numOfChar] != str2[numOfChar])
            return false;

    return true;
}

/*Prints the board with row and column markings to the terminal*/
void printBoard (const game_t &currentGame)
{
    int column, row;
    char column_c;

    /*Printing mode for square shaped cells*/
    if (currentGame.boardType == cellShape_t::square)
    {
        cout << "      ";

        /*print the column letters first*/
        for ((column = 0) , (column_c = 'a') ; column < currentGame.boardState[0].size() ; (++column) && (++column_c))
            cout << column_c << ' ';

        cout << "\n\n";

        /*then print the row numbers with the cells iteratively*/
        for (int row = 0; row < currentGame.boardState.size(); ++row)
        {
            cout << "  " << row + 1 << "  ";

            if ( row < 9 )  cout << ' ';

            for (auto cell : currentGame.boardState[row])
                cout << (char)cell << ' ';

            cout << endl;                
        }
    }

    /*Printing mode for hexagon shaped cells, they have a unique structure*/
    else if (currentGame.boardType == cellShape_t::hexagon)
    {
        cout << "       ";

        /*print the first line of column letters*/
        for ((column = 0) , (column_c = 'a') ; column < currentGame.boardState.size()-1 ; (++column) , (++column_c))
            cout << column_c << ' ';
        
        cout << endl << "      ";

        /*then the bigger one*/
        for ((column = 0) , (column_c = 'a') ; column < currentGame.boardState.size() ; (++column) , (++column_c))
            cout << column_c << ' ';

        cout << endl << endl;
        
        /*print the cells*/
        for (row = 0; row < currentGame.boardState.size(); ++row)
        {
            cout << "  " << row + 1 << "  ";

            if ( row < 9 )  cout << ' ';

            /*print an extra space for the smaller rows to fix the alignment, since the number of cells in the rows (for example) changes between 7 and 6 cells in a 7x7 board*/
            if (currentGame.boardState[row].size() < currentGame.boardState[row+1].size()) cout << ' ';
 
            for (auto cell : currentGame.boardState[row])
                cout << (char)cell << ' ';

            cout << endl;                
        }
    }
}

/*Prints the board to a file*/
bool printBoard (const game_t &currentGame, ofstream &targetFile)
{
    int flag_3D = 0,
        column,
        row;

    char column_c;

    /*Check if the file is opened before calling the function*/
    if (targetFile.is_open() == false)
    {
        cerr << "File was not opened\n";
        return false;
    }

    /*then print the row numbers with the cells iteratively*/
    for (int row = 0; row < currentGame.boardState.size(); ++row)
    {
        for (decltype(currentGame.boardState[row][0]) cell : currentGame.boardState[row])
        {
            if (cell == cellState_t::null)  targetFile << 'n';
            else targetFile << (char)cell;
        }
            
        targetFile << endl;                
    }

    return true;
}

/*Do the requested action in the currentGame*/
bool move_peg (const action_t &action, game_t &currentGame)
{
    /*movement mode for hexagon shaped cells, only used when travelng vertically, use normal algorithm for traveling horizontally*/
    if (currentGame.boardType == cellShape_t::hexagon && action.direction[1] == 0)
    {
        /*check if there is a peg in the cell*/
        if (currentGame.boardState[action.row][action.column] == cellState_t::peg)
        {
            /*check if the target row is the bigger row*/
            if (currentGame.boardState[action.row].size() == currentGame.boardState.size())
            {
                /*move to the right side of the vertical movement when available*/
                if (currentGame.boardState[action.row + action.direction[0]][action.column] == cellState_t::peg &&
                    currentGame.boardState[action.row + action.direction[0]*2][action.column + 1] == cellState_t::empty)
                    {
                        currentGame.boardState[action.row][action.column] = currentGame.boardState[action.row + action.direction[0]][action.column] = cellState_t::empty;
                        currentGame.boardState[action.row + action.direction[0]*2][action.column + 1] = cellState_t::peg;
                        return true;
                    }
                
                /*move to the left side of the vertical movement when available*/
                else if (currentGame.boardState[action.row + action.direction[0]][action.column + action.direction[1] - 1] == cellState_t::peg &&
                         currentGame.boardState[action.row + action.direction[0]*2][action.column + action.direction[1]*2 - 1] == cellState_t::empty)
                        {
                            currentGame.boardState[action.row][action.column] = currentGame.boardState[action.row + action.direction[0]][action.column - 1] = cellState_t::empty;
                            currentGame.boardState[action.row + action.direction[0]*2][action.column - 1] = cellState_t::peg;
                            return true;
                        }

                else return false;
            }

            /*if the row is the smaller row, the measurements for vertical left and right change a bit*/
            else
            {
                /*move to the right side of the vertical movement when available*/
                if (currentGame.boardState[action.row + action.direction[0]][action.column] == cellState_t::peg &&
                    currentGame.boardState[action.row + action.direction[0]*2][action.column - 1] == cellState_t::empty)
                    {
                        currentGame.boardState[action.row][action.column] = currentGame.boardState[action.row + action.direction[0]][action.column] = cellState_t::empty;
                        currentGame.boardState[action.row + action.direction[0]*2][action.column - 1] = cellState_t::peg;
                        return true;
                    }
                
                /*move to the left side of the vertical movement when available*/
                else if (currentGame.boardState[action.row + action.direction[0]][action.column + 1] == cellState_t::peg &&
                         currentGame.boardState[action.row + action.direction[0]*2][action.column + 1] == cellState_t::empty)
                        {
                            currentGame.boardState[action.row][action.column] = currentGame.boardState[action.row + action.direction[0]][action.column + 1] = cellState_t::empty;
                            currentGame.boardState[action.row + action.direction[0]*2][action.column + 1] = cellState_t::peg;
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
        if (check_movement(action,currentGame))
        {
            currentGame.boardState[action.row][action.column] = currentGame.boardState[action.row + action.direction[0]][action.column + action.direction[1]] = cellState_t::empty;
            currentGame.boardState[action.row + action.direction[0]*2][action.column + action.direction[1]*2] = cellState_t::peg;
            return true;
        }

        else return false;
    }
}

/*Check if the action is viable, returns true or false*/
bool check_movement (const action_t &action, const game_t &currentGame)
{
    /*check if the target row and column are in the range*/
    if (action.row > currentGame.boardState.size() || action.column > currentGame.boardState[action.row].size())  
        return false;

    /*check if the action direction is in the range*/
    if (action.column + action.direction[1]*2 >= currentGame.boardState[action.row].size() || action.column + action.direction[1]*2 < 0 ||
        action.row + action.direction[0]*2 >= currentGame.boardState.size() || action.row + action.direction[0]*2 < 0)
        return false;

    /*movement mode for hexagon shaped cells, only used when travelng vertically, use normal algorithm for traveling horizontally*/
    if (currentGame.boardType == cellShape_t::hexagon && action.direction[1] == 0)
    {
        /*check if there is a peg in the cell*/
        if (currentGame.boardState[action.row][action.column] == cellState_t::peg)
        {
            /*check if the target row is the bigger row*/
            if (currentGame.boardState[action.row].size() == currentGame.boardState.size())
            {
                /*check the right side of the vertical movement if it's available*/
                if (currentGame.boardState[action.row + action.direction[0]][action.column + action.direction[1]] == cellState_t::peg &&
                    currentGame.boardState[action.row + action.direction[0]*2][action.column + action.direction[1]*2 + 1] == cellState_t::empty)
                    return true;
                
                /*check the left side of the vertical movement if it's available*/
                else if (currentGame.boardState[action.row + action.direction[0]][action.column + action.direction[1] - 1] == cellState_t::peg &&
                         currentGame.boardState[action.row + action.direction[0]*2][action.column + action.direction[1]*2 - 1] == cellState_t::empty)
                        return true;

                else return false;
            }

            /*if the row is the smaller row, the measurements for vertical left and right change a bit*/
            else
            {
                /*check the right side of the vertical movement if it's available*/
                if (currentGame.boardState[action.row + action.direction[0]][action.column + action.direction[1]] == cellState_t::peg &&
                    currentGame.boardState[action.row + action.direction[0]*2][action.column + action.direction[1]*2 - 1] == cellState_t::empty)
                    return true;
                
                /*check the left side of the vertical movement if it's available*/
                else if (currentGame.boardState[action.row + action.direction[0]][action.column + action.direction[1] + 1] == cellState_t::peg &&
                         currentGame.boardState[action.row + action.direction[0]*2][action.column + action.direction[1]*2 + 1] == cellState_t::empty)
                        return true;

                else return false;
            }
        }

        else return false;
    }

    /*normal movement mode for square shaped cells and traveling horizontally in hexagon shaped cells*/
    else if (currentGame.boardState[action.row][action.column] == cellState_t::peg &&
             currentGame.boardState[action.row + action.direction[0]][action.column + action.direction[1]] == cellState_t::peg &&
             currentGame.boardState[action.row + action.direction[0]*2][action.column + action.direction[1]*2] == cellState_t::empty)
            return true;

    else return false;
}

/*Translate the string command of the action to an action_t struct*/
bool translate_movement (const string &movement, action_t &new_action)
{
    int counter = 0;

    /*transform the column letters to integers, weither they are capital or small letters and store them as the column*/
    if (movement[counter] >= 'A' && movement[counter] <= 'Z')   new_action.column = movement[counter] - 'A';
    else if (movement[counter] >= 'a' || movement[counter] <= 'z')    new_action.column = movement[counter] - 'a';
    else return false;

    ++counter;
    
    /*store the integer value of the row*/
    if (movement[counter] > '0' && movement[counter] <= '9') new_action.row = movement[counter] - '0';
    else return false;

    ++counter;

    if (movement[counter] > '0' && movement[counter] <= '9')
    {
        new_action.row = (new_action.row)*10 + movement[counter] - '0';

        ++counter;
        if (movement[counter] != '-')  return false;
    }

    else if (movement[counter] != '-')  return false;

    ++counter;

    /*store the direction of the action based on the letter of the given direction*/
    if (movement[counter] == 'U' || movement[counter] == 'u')   new_action.direction = {-1, 0};
    else if (movement[counter] == 'D' || movement[counter] == 'd')   new_action.direction = {1, 0};
    else if (movement[counter] == 'L' || movement[counter] == 'l')   new_action.direction = {0, -1};
    else if (movement[counter] == 'R' || movement[counter] == 'r')   new_action.direction = {0, 1};
    else return false;
    
    --new_action.row;
    return true;
}

/*Check the progress of the player, return the final score when no moveable peg are found*/
int progress_checker (const game_t &currentGame)
{
    int moveable = 0, unmovable = 0;
    array<array<int, 2>, 4> direction_list = { {{-1, 0}, {1, 0}, {0, -1}, {0, 1}} };
    action_t temp_action;

    /*iterate through the cells of the board, and test each direction for valid movements*/
    for (temp_action.row = 0 ; temp_action.row < currentGame.boardState.size() ; ++temp_action.row)
        for (temp_action.column = 0; temp_action.column < currentGame.boardState[temp_action.row].size() ; ++temp_action.column)
        {
            for (auto direction : direction_list)
            {
                temp_action.direction = direction;

                if (check_movement(temp_action, currentGame))
                {
                    /*increase the number of moveables if a direction was valid*/
                    ++moveable;
                    break;
                }
            }

            /*when all directions are invalid, increment the number of unmoveables*/
            if (currentGame.boardState[temp_action.row][temp_action.column] == cellState_t::peg &&
                temp_action.direction == direction_list[3])
                ++unmovable;
        }

    if (moveable == 0) return unmovable;

    else return 0;
}

/*Randomly generate a valid movement and do it*/
bool botMovementEngine (game_t &currentGame) 
{
    array<array<int, 2>, 4> direction_list = { {{-1, 0}, {1, 0}, {0, -1}, {0, 1}} };
    action_t temp_action;

    /*Keep searching for a valid action as long as there are movable pegs*/
    while (progress_checker(currentGame) == 0)
        for (temp_action.row = 0 ; temp_action.row < currentGame.boardState.size() ; ++temp_action.row)
            for (temp_action.column = 0; temp_action.column < currentGame.boardState[temp_action.row].size() ; ++temp_action.column)
                for (auto direction : direction_list)
                {
                    temp_action.direction = direction;
                    
                    /*used rand as a switch to simulate random chances for every movement to happen*/
                    if (rand()%2 == 1 && check_movement(temp_action, currentGame))
                    {
                        /*perform the chosen action*/
                        move_peg (temp_action, currentGame);
                        ++currentGame.totalMoves;
                        return true;
                    }
                }
    
    return false;
}

/*Save the current game to a .txt file with all it's information*/
bool saveToFile (const game_t &currentGame, const string &fileName)
{
    ofstream saveFile;

    /*open the file in write mode*/
    saveFile.open(fileName, ofstream::out);

    /*Check if the file was opened*/
    if ( !saveFile.is_open() )
    {
        cerr << "Couldn't open " << fileName << endl;
        return false;
    }

    /*write the player type*/
    if (currentGame.playerType == player_t::human)  saveFile << "human ";
    else if (currentGame.playerType == player_t::bot) saveFile << "bot ";

    /*write the board type*/
    if (currentGame.boardType == cellShape_t::square)  saveFile << "square ";
    else if (currentGame.boardType == cellShape_t::hexagon) saveFile << "hexagon ";

    /*write the total moves performed*/
    if (currentGame.totalMoves < 10)    saveFile << 0;
    saveFile << currentGame.totalMoves << endl;

    /*write the board*/
    printBoard (currentGame, saveFile);

    /*close the board*/
    saveFile.close();

    return true;
}

/*Read a game from a file and return it with it's info*/
bool loadFile (game_t &currentGame, const string &fileName)
{
    vector<cellState_t> tempRow;
    ifstream loadFile;
    string buffer;

    loadFile.open (fileName, ifstream::in);

    /*Check if the file was opened*/
    if (loadFile.is_open() == false)
    {
        cerr << "Couldn't open " << fileName << endl;
        return false;
    }

    loadFile >> buffer;
    if (buffer == "human")  currentGame.playerType = player_t::human;
    else if (buffer == "bot") currentGame.playerType = player_t::bot;

    loadFile >> buffer;
    if (buffer == "square")  currentGame.boardType = cellShape_t::square;
    else if (buffer == "hexagon") currentGame.boardType = cellShape_t::hexagon;

    loadFile >> buffer;
    currentGame.totalMoves = (buffer[0] - '0')*10 + buffer[1] - '0';

    getline (loadFile, buffer);

    while (loadFile.peek() != EOF)
    {
        tempRow.clear();
        getline (loadFile, buffer);

        for (decltype(buffer[0]) character : buffer)
        {
            if (character == 'n')   tempRow.push_back(cellState_t::null);
            else if (character == 'P')   tempRow.push_back(cellState_t::peg);
            else if (character == '.')   tempRow.push_back(cellState_t::empty);
        }

        currentGame.boardState.push_back(tempRow);
    }

    loadFile.close();

    return true;
}

/*Manage the game with the computer as a player*/
void botGameManager (game_t &currentGame)
{
    int unmoveables = 0;
    string buffer;

    /*keep playing until the bot wins*/
    while(1)
    {
        /*generate and perform a random action*/
        if (unmoveables > 0)
        {
            /*unmovable pegs number is only known when no action can be performed*/
            cout << "\n--------------------------\n";
            printBoard (currentGame);
            cout << endl;
            cout << "\n The bot won with the score " << unmoveables << endl;
            break;
        }

        cout << "\n--------------------------\n";
        printBoard (currentGame);
        cout << endl;

        while (1)
        {
            cout << " (p) Proceed to next move\n Or\n (SAVE filename.txt) to save the game to a file\n (-1) to leave\n ";
            cin >> buffer;

            if (buffer == "p")    break;

            else if (buffer == "SAVE")
            {
                cin >> buffer;

                if (saveToFile (currentGame, buffer))
                    return;
            } 

            else if (buffer == "-1")  return;

            else cerr << " Wrong input, try again\n";
        }

        botMovementEngine(currentGame);

        unmoveables = progress_checker (currentGame);
    }
}

/*Manage the game with a human as a player*/
void humanGameManager (game_t &currentGame)
{
    int unmoveables = 0;
    string movement;
    action_t new_action;

    while (1)
    {
        /*print the board after each action*/
        cout << "\n--------------------------\n";
        printBoard (currentGame);

        while(1)
        {
            cout << " Type the movement you want to do as in the example:\n b4-U (move the peg in b4 upwards)\n (Type -1 to surrender)\n ";
            cout << "\033[2J";
            cin >> movement;

            /*surrender when the player types -1*/
            if (movement == "-1")   break;

            /*process the movement*/
            else if (translate_movement(movement, new_action) && 
                move_peg(new_action, currentGame) )
            {
                ++currentGame.totalMoves;
                break;
            }

            else if (movement == "SAVE")
            {
                cin >> movement;

                if (saveToFile (currentGame, movement))
                    return;
            } 

            else cerr << "\n Invalid movement, please try again.\n\n ";
        }
        
        if (movement == "-1")   break;

        /*calculate the score*/
        unmoveables = progress_checker (currentGame);

        /*unmovable pegs number is only known when no action can be performed*/
        if (unmoveables > 0)
        {
            cout << "\n--------------------------\n";
            printBoard (currentGame);
            cout << "\n Congratulations! You won, your score is " << unmoveables << endl;
            break;
        }
    }
}

/*Main UI of the game*/
void soloSolitaire_UI ()
{
    /* Initializing the boards */
    game_t   currentGame;
    string movement, player, buffer;
    action_t new_action;
    vector<game_t> board_list;
    int board_num, unmoveables, counter;

    /*create a random seed using malloc for random numbers*/
    srand((long)malloc(1));

    cout << "\n ------ Welcome to Solo Solitaire game ------ " << endl;

    /*store every board in a vector for the player to choose from*/
    currentGame.boardState =   {{cellState_t::null, cellState_t::null, cellState_t::peg,   cellState_t::peg, cellState_t::peg, cellState_t::null, cellState_t::null},
                                {cellState_t::null,  cellState_t::peg, cellState_t::peg,   cellState_t::peg, cellState_t::peg,  cellState_t::peg, cellState_t::null},
                                { cellState_t::peg,  cellState_t::peg, cellState_t::peg, cellState_t::empty, cellState_t::peg,  cellState_t::peg,  cellState_t::peg},
                                { cellState_t::peg,  cellState_t::peg, cellState_t::peg,   cellState_t::peg, cellState_t::peg,  cellState_t::peg,  cellState_t::peg},
                                { cellState_t::peg,  cellState_t::peg, cellState_t::peg,   cellState_t::peg, cellState_t::peg,  cellState_t::peg,  cellState_t::peg},
                                {cellState_t::null,  cellState_t::peg, cellState_t::peg,   cellState_t::peg, cellState_t::peg,  cellState_t::peg, cellState_t::null},
                                {cellState_t::null, cellState_t::null, cellState_t::peg,   cellState_t::peg, cellState_t::peg, cellState_t::null, cellState_t::null}};
    
    currentGame.boardType = cellShape_t::square;

    board_list.push_back(currentGame);

        
    currentGame.boardState =   {{cellState_t::null, cellState_t::null, cellState_t::null, cellState_t::peg,   cellState_t::peg, cellState_t::peg, cellState_t::null, cellState_t::null, cellState_t::null},
                                {cellState_t::null, cellState_t::null, cellState_t::null, cellState_t::peg,   cellState_t::peg, cellState_t::peg, cellState_t::null, cellState_t::null, cellState_t::null},
                                {cellState_t::null, cellState_t::null, cellState_t::null, cellState_t::peg,   cellState_t::peg, cellState_t::peg, cellState_t::null, cellState_t::null, cellState_t::null},
                                { cellState_t::peg,  cellState_t::peg,  cellState_t::peg, cellState_t::peg,   cellState_t::peg, cellState_t::peg,  cellState_t::peg,  cellState_t::peg,  cellState_t::peg},
                                { cellState_t::peg,  cellState_t::peg,  cellState_t::peg, cellState_t::peg, cellState_t::empty, cellState_t::peg,  cellState_t::peg,  cellState_t::peg,  cellState_t::peg},
                                { cellState_t::peg,  cellState_t::peg,  cellState_t::peg, cellState_t::peg,   cellState_t::peg, cellState_t::peg,  cellState_t::peg,  cellState_t::peg,  cellState_t::peg},
                                {cellState_t::null, cellState_t::null, cellState_t::null, cellState_t::peg,   cellState_t::peg, cellState_t::peg, cellState_t::null, cellState_t::null, cellState_t::null},
                                {cellState_t::null, cellState_t::null, cellState_t::null, cellState_t::peg,   cellState_t::peg, cellState_t::peg, cellState_t::null, cellState_t::null, cellState_t::null},
                                {cellState_t::null, cellState_t::null, cellState_t::null, cellState_t::peg,   cellState_t::peg, cellState_t::peg, cellState_t::null, cellState_t::null, cellState_t::null}};

    currentGame.boardType = cellShape_t::square;

    board_list.push_back(currentGame);


    currentGame.boardState =   {{cellState_t::null, cellState_t::null, cellState_t::peg,   cellState_t::peg, cellState_t::peg, cellState_t::null, cellState_t::null, cellState_t::null},
                                {cellState_t::null, cellState_t::null, cellState_t::peg,   cellState_t::peg, cellState_t::peg, cellState_t::null, cellState_t::null, cellState_t::null},
                                {cellState_t::null, cellState_t::null, cellState_t::peg,   cellState_t::peg, cellState_t::peg, cellState_t::null, cellState_t::null, cellState_t::null},
                                { cellState_t::peg,  cellState_t::peg, cellState_t::peg,   cellState_t::peg, cellState_t::peg,  cellState_t::peg,  cellState_t::peg,  cellState_t::peg},
                                { cellState_t::peg,  cellState_t::peg, cellState_t::peg, cellState_t::empty, cellState_t::peg,  cellState_t::peg,  cellState_t::peg,  cellState_t::peg},
                                { cellState_t::peg,  cellState_t::peg, cellState_t::peg,   cellState_t::peg, cellState_t::peg,  cellState_t::peg,  cellState_t::peg,  cellState_t::peg},
                                {cellState_t::null, cellState_t::null, cellState_t::peg,   cellState_t::peg, cellState_t::peg, cellState_t::null, cellState_t::null, cellState_t::null},
                                {cellState_t::null, cellState_t::null, cellState_t::peg,   cellState_t::peg, cellState_t::peg, cellState_t::null, cellState_t::null, cellState_t::null}};

    currentGame.boardType = cellShape_t::square;

    board_list.push_back(currentGame);


    currentGame.boardState =   {{cellState_t::null, cellState_t::null, cellState_t::peg,   cellState_t::peg, cellState_t::peg, cellState_t::null, cellState_t::null},
                                {cellState_t::null, cellState_t::null, cellState_t::peg,   cellState_t::peg, cellState_t::peg, cellState_t::null, cellState_t::null},
                                { cellState_t::peg,  cellState_t::peg, cellState_t::peg,   cellState_t::peg, cellState_t::peg,  cellState_t::peg,  cellState_t::peg},
                                { cellState_t::peg,  cellState_t::peg, cellState_t::peg, cellState_t::empty, cellState_t::peg,  cellState_t::peg,  cellState_t::peg},
                                { cellState_t::peg,  cellState_t::peg, cellState_t::peg,   cellState_t::peg, cellState_t::peg,  cellState_t::peg,  cellState_t::peg},
                                {cellState_t::null, cellState_t::null, cellState_t::peg,   cellState_t::peg, cellState_t::peg, cellState_t::null, cellState_t::null},
                                {cellState_t::null, cellState_t::null, cellState_t::peg,   cellState_t::peg, cellState_t::peg, cellState_t::null, cellState_t::null}};

    currentGame.boardType = cellShape_t::square;

    board_list.push_back(currentGame);


    currentGame.boardState =   {{cellState_t::null, cellState_t::null, cellState_t::null, cellState_t::null,   cellState_t::peg, cellState_t::null, cellState_t::null, cellState_t::null, cellState_t::null},
                                {cellState_t::null, cellState_t::null, cellState_t::null,  cellState_t::peg,   cellState_t::peg,  cellState_t::peg, cellState_t::null, cellState_t::null, cellState_t::null},
                                {cellState_t::null, cellState_t::null,  cellState_t::peg,  cellState_t::peg,   cellState_t::peg,  cellState_t::peg,  cellState_t::peg, cellState_t::null, cellState_t::null},
                                {cellState_t::null,  cellState_t::peg,  cellState_t::peg,  cellState_t::peg,   cellState_t::peg,  cellState_t::peg,  cellState_t::peg,  cellState_t::peg, cellState_t::null},
                                { cellState_t::peg,  cellState_t::peg,  cellState_t::peg,  cellState_t::peg, cellState_t::empty,  cellState_t::peg,  cellState_t::peg,  cellState_t::peg,  cellState_t::peg},
                                {cellState_t::null,  cellState_t::peg,  cellState_t::peg,  cellState_t::peg,   cellState_t::peg,  cellState_t::peg,  cellState_t::peg,  cellState_t::peg, cellState_t::null},
                                {cellState_t::null, cellState_t::null,  cellState_t::peg,  cellState_t::peg,   cellState_t::peg,  cellState_t::peg,  cellState_t::peg, cellState_t::null, cellState_t::null},
                                {cellState_t::null, cellState_t::null, cellState_t::null,  cellState_t::peg,   cellState_t::peg,  cellState_t::peg, cellState_t::null, cellState_t::null, cellState_t::null},
                                {cellState_t::null, cellState_t::null, cellState_t::null, cellState_t::null,   cellState_t::peg, cellState_t::null, cellState_t::null, cellState_t::null, cellState_t::null}};

    currentGame.boardType = cellShape_t::square;

    board_list.push_back(currentGame);


    currentGame.boardState =   {{cellState_t::null, cellState_t::null, cellState_t::empty, cellState_t::null, cellState_t::null},
                                {        cellState_t::null,  cellState_t::peg,   cellState_t::peg, cellState_t::null         },
                                {cellState_t::null,  cellState_t::peg,   cellState_t::peg,  cellState_t::peg, cellState_t::null},
                                {         cellState_t::peg,  cellState_t::peg,   cellState_t::peg,  cellState_t::peg         },
                                { cellState_t::peg,  cellState_t::peg,   cellState_t::peg,  cellState_t::peg,  cellState_t::peg}};
    
    currentGame.boardType = cellShape_t::hexagon;

    board_list.push_back(currentGame);


    currentGame.boardState =   {{          cellState_t::null,  cellState_t::peg,  cellState_t::peg,   cellState_t::peg,  cellState_t::peg, cellState_t::null        },
                                {cellState_t::null,  cellState_t::peg,  cellState_t::peg,  cellState_t::peg,   cellState_t::peg,  cellState_t::peg,  cellState_t::null},
                                {          cellState_t::peg,  cellState_t::peg,  cellState_t::peg,   cellState_t::peg,  cellState_t::peg,  cellState_t::peg         },
                                {cellState_t::peg,  cellState_t::peg,  cellState_t::peg,  cellState_t::empty,   cellState_t::peg,  cellState_t::peg,  cellState_t::peg},
                                {          cellState_t::peg,  cellState_t::peg,  cellState_t::peg, cellState_t::peg,  cellState_t::peg,  cellState_t::peg           },
                                {cellState_t::null,  cellState_t::peg,  cellState_t::peg,  cellState_t::peg,   cellState_t::peg,  cellState_t::peg,  cellState_t::null},
                                {          cellState_t::null,  cellState_t::peg,  cellState_t::peg,   cellState_t::peg,  cellState_t::peg,  cellState_t::null       }};

    currentGame.boardType = cellShape_t::hexagon;

    board_list.push_back(currentGame);

    /*keep playing until being prompted to leave*/
    while(1)
    {
        currentGame.boardState.clear();
        unmoveables = 0;
        cout << "\n--------------------------\n";
        cout << "\n Use numbers to send commands.\n\n 1- New Game\n 2- Load Game\n 0- Exit\n";

        cin >> buffer;

        if (buffer == "1") 
        {
            /*selecting a board*/
            counter = 0;
            for (auto currentGame : board_list)
            {   
                ++counter;
                cout << "\n--------------------------\n" << counter << "-\n";
                printBoard (currentGame);
                cout << endl;
            }

            cout << " Please enter the number of a board from the above:\n";

            cout << "\n(0 to leave)\n";

            cin >> buffer;

            board_num = buffer[0] - '0';

            /*check if the board number is valid*/
            if (board_num < 0 || board_num > board_list.size())
            {
                cerr << "Try again\n";
                continue;
            }
            
            else if (board_num == 0) continue;

            currentGame = board_list[board_num - 1];
            currentGame.totalMoves = 0;

            /*ask who is going to play, a bot or a human*/
            cout << "Do you want to play yourself? Or do you want to see how intelligent is your PC?\n (Type: \"me\" for you, \"bot\" for the computer, choose wisely)\n";
            
            while (1)
            {
                cin >> player;
            
                if (player == "me")
                { 
                    currentGame.playerType = player_t::human;
                    break;
                }
                
                else if (player == "bot")
                {
                    currentGame.playerType = player_t::bot;
                    break;
                }

                else cerr << "Wrong input, try again\n";
            }
        }

        else if (buffer == "2")
        {
            cout << "Please enter the name of the file:\n";
            cin >> buffer;
            if (!loadFile (currentGame, buffer))    continue;
        }

        else if (buffer == "0") return;

        else
        {
            cerr << " Wrong input, try again\n";
            continue;
        }

        if (currentGame.playerType == player_t::human)  humanGameManager(currentGame);

        else if (currentGame.playerType == player_t::bot)   botGameManager(currentGame);

        else    cerr << "Try again\n";
    }
}


int main ()
{
    soloSolitaire_UI ();
}