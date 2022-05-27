#include <iostream>
#include <cstdlib>
#include "header.h"

using std::cin;
using std::cout;
using std::endl;
using std::getline;


namespace Gaming
{

    //BoardGame2D class implementation

    void BoardGame2D::playUser()
    {
        cout << *this;
        while (!endGame())  //Keep taking actions until the game ends
        {
            cout << "Please enter an action in the shape: \n(\"1A U\" for PegSolitaire) (move the cell in row 1 and column A upwards)\n(\"1 U\" for others) (move the block with the label 1 upwards)\n(enter -1 to surrender)\n";
            string buffer;
            getline(cin, buffer);   //Take input
            if (buffer == "-1") return;

            while (!playUser(buffer))
            {
                cout << "\033[1F\033[2K\033[0G" << "Invalid action, try again. ";
                getline(cin, buffer);
                if (buffer == "-1") return;
            }

            buffer.clear();
            cout << *this;
        }

        cout << "\nYou won with the score " << boardScore() << endl;
    }

    void BoardGame2D::playAutoAll()
    {
        long i =0;
        cout << *this;
        
        do {
            playAuto();
            cout << *this;
            sleep(1);
        } while (!endGame());

        cout << "\nGame ended with the score " << boardScore() << endl;
    }

    ostream & operator<<(ostream & output, const BoardGame2D & board)
    {
        output << "\033[2J\033[H";  //Reset the terminal
        board.print();    //Print the board
        output << endl;
        return output;
    }

    ofstream & operator<<(ofstream & output, const BoardGame2D & board)
    {
        output << "\033[2J\033[H";  //Reset the terminal
        board.print();    //Print the board
        output << endl;
        return output;
    }


    //PegSolitaire class implementation

    int PegSolitaire::Action::_directionList[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}; //Initialization of direction list

    bool PegSolitaire::Action::setAction (const Direction newDirection, const unsigned int row, const unsigned int column)
    {
        if (!setDirection(newDirection) || !setTarget(row, column)) return false;
        else return true;
    }

    bool PegSolitaire::Action::setAction (const string & action)
    {
        if (!setTarget(action[0] - '1', action[1] - 'A')|| action[2] != ' ' || !setDirection(action[3]))  return false;
        else return true;
    }

    bool PegSolitaire::Action::setDirection (const char newDirection)
    {
        if (newDirection == 'U' || newDirection == 'u')   setDirection(Direction::UP);
        else if (newDirection == 'D' || newDirection == 'd')   setDirection(Direction::DOWN);
        else if (newDirection == 'L' || newDirection == 'l')   setDirection(Direction::LEFT);
        else if (newDirection == 'R' || newDirection == 'r')   setDirection(Direction::RIGHT);
        else return false;

        return true;
    }

    bool PegSolitaire::Action::setDirection (const Direction newDirection)
    {
        if (newDirection <= Direction::RIGHT && newDirection >= Direction::UP) _direction = newDirection;
        else return false;

        return true;
    }

    bool PegSolitaire::Action::setTarget (const unsigned int row, const unsigned int column)
    {
        if (row >= 0 && row <= 8)   _row = row; 
        else return false;

        if (column >= 0 && column <= 8)   _column = column;
        else if (column >= 32 || column <= 40)    _column = column - 32;    //Incase the column was sent as small character
        else return false;

        return true;
    }

    bool PegSolitaire::playUser(const string strAction)
    {
        Action action;
        if (!action.setAction(strAction) || !movePeg(action))    return false;  //return false if either the action input or the action itself was not valid
        else return true;
    }

    void PegSolitaire::playAuto()
    {
        if(endGame())   return; //Do not search for ,oves if the game has ended

        Action tempAction;
        const Action::Direction directionList[4] = {Action::Direction::UP, Action::Direction::DOWN, Action::Direction::LEFT, Action::Direction::RIGHT}; //List of possible directions

        
        while(1) //keep searching for an action
        {
            /*iterate through the cells of the board, and test each direction for valid movements*/
            for (int row = 0 ; row < 9 ; ++row)
                for (int column = 0 ; column < 9 ; ++column)
                {
                    tempAction.setTarget(row, column);  //Set the location of the chosen target
                    for (auto tempDir : directionList)  //iterate through the direction list
                    {
                        tempAction.setDirection(tempDir);   //Set the chosen direction
                        if (rand()%5 == 1 && movePeg(tempAction))   //The action has a chance of 50% to be used
                            return;
                    }
                } 
        }
    }

    bool PegSolitaire::checkAction(Action action) const
    {
        if (action.getRow() > 8 || action.getColumn() > 8)  return false;

        if (action.getColumn() + action.getDirection2D()[1]*2 > 8 || action.getColumn() + action.getDirection2D()[1]*2 < 0||
            action.getRow() + action.getDirection2D()[0]*2 > 8 || action.getRow() + action.getDirection2D()[0]*2 < 0)
            return false;   //The action is invalid if it goes beyond the board bounds

        else if(getCellState(action.getRow(), action.getColumn()) == CellState::peg &&
                getCellState(action.getRow() + action.getDirection2D()[0], action.getColumn() + action.getDirection2D()[1]) == CellState::peg &&
                getCellState(action.getRow() + action.getDirection2D()[0]*2, action.getColumn() + action.getDirection2D()[1]*2) == CellState::empty)
            return true;    //The action should make an peg go over another adjacent peg and land in an empty cell. If it is, then the action is valid

        else return false;
    }

    bool PegSolitaire::movePeg(const Action action)
    {
        if (checkAction(action))    //Check for the action if it's valid
        {
            setCellState(action.getRow(), action.getColumn(), CellState::empty);
            setCellState(action.getRow() + action.getDirection2D()[0], action.getColumn() + action.getDirection2D()[1], CellState::empty);
            setCellState(action.getRow() + action.getDirection2D()[0]*2, action.getColumn() + action.getDirection2D()[1]*2, CellState::peg);
            return true;    //Return true after the peg is moved
        }

        else return false;
    }

    bool PegSolitaire::endGame() const
    {
        int moveable = 0, unmovable = 0;
        Action tempAction;
        const Action::Direction directionList[5] = {Action::Direction::UP, Action::Direction::DOWN, Action::Direction::LEFT, Action::Direction::RIGHT};

        /*iterate through the cells of the board, and test each direction for valid movements*/
        for (int row = 0 ; row < 9 ; ++row)
            for (int column = 0 ; column < 9 ; ++column)
            {
                tempAction.setTarget(row, column);
                for (auto tempDir : directionList)
                {
                    tempAction.setDirection(tempDir);
                    if (checkAction(tempAction)) return false;  //If a movable peg was found, then the game hasn't finished yet
                }
            }

        return true;    //No movable pegs found
    }

    int PegSolitaire::boardScore() const
    {
        int pegs = 0;
        Action tempAction;
        Action::Direction directionList[5] = {Action::Direction::UP, Action::Direction::DOWN, Action::Direction::LEFT, Action::Direction::RIGHT};

        /*iterate through the cells of the board, and count the remaining pegs*/
        for (int row = 0; row < 9; ++row)
            for (int column = 0; column < 9; ++column)
                if(getCellState(row, column) == CellState::peg) ++pegs;
        
        return pegs;
    }

    void PegSolitaire::initialize()
    {
        //The initial board of the game
        CellState baseBoard[9][9] ={{CellState::null, CellState::null, CellState::null, CellState::peg,   CellState::peg, CellState::peg, CellState::null, CellState::null, CellState::null},
                                    {CellState::null, CellState::null, CellState::null, CellState::peg,   CellState::peg, CellState::peg, CellState::null, CellState::null, CellState::null},
                                    {CellState::null, CellState::null, CellState::null, CellState::peg,   CellState::peg, CellState::peg, CellState::null, CellState::null, CellState::null},
                                    { CellState::peg,  CellState::peg,  CellState::peg, CellState::peg,   CellState::peg, CellState::peg,  CellState::peg,  CellState::peg,  CellState::peg},
                                    { CellState::peg,  CellState::peg,  CellState::peg, CellState::peg, CellState::empty, CellState::peg,  CellState::peg,  CellState::peg,  CellState::peg},
                                    { CellState::peg,  CellState::peg,  CellState::peg, CellState::peg,   CellState::peg, CellState::peg,  CellState::peg,  CellState::peg,  CellState::peg},
                                    {CellState::null, CellState::null, CellState::null, CellState::peg,   CellState::peg, CellState::peg, CellState::null, CellState::null, CellState::null},
                                    {CellState::null, CellState::null, CellState::null, CellState::peg,   CellState::peg, CellState::peg, CellState::null, CellState::null, CellState::null},
                                    {CellState::null, CellState::null, CellState::null, CellState::peg,   CellState::peg, CellState::peg, CellState::null, CellState::null, CellState::null}};

        //Copy the cells of the initial board to the current board
        for (int row; row < 9; ++row)
            for (int column = 0; column < 9; ++column)
                _board[row][column] = baseBoard[row][column];
    }

    void PegSolitaire::print() const
    {
        int column, row;
        char column_c = 'a';

        cout << "     ";
        
        /*print the column letters first*/
        for (auto temp : _board[0])
            cout << column_c++ << ' ';

        cout << "\n\n";

        /*then print the row numbers with the cells iteratively*/
        for (int row = 0; row < 9; ++row)
        {
            cout << "  " << row + 1 << "  ";

            for (auto cell : _board[row])
                cout << (char)cell << ' ';

            cout << endl;                
        }
    }


    int EightPuzzle::Action::_directionList[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}; //Initialization of direction list

    bool EightPuzzle::Action::setTarget(const int Target)
    {
        if (Target < 9 && Target > 0)
        {
            _Target = Target;
            return true;
        }

        else return false;
    }

    bool EightPuzzle::Action::setDirection (const char newDirection)
    {
        if (newDirection == 'U' || newDirection == 'u')   setDirection(Direction::UP);
        else if (newDirection == 'D' || newDirection == 'd')   setDirection(Direction::DOWN);
        else if (newDirection == 'L' || newDirection == 'l')   setDirection(Direction::LEFT);
        else if (newDirection == 'R' || newDirection == 'r')   setDirection(Direction::RIGHT);
        else return false;

        return true;
    }

    bool EightPuzzle::Action::setDirection (const Direction newDirection)
    {
        if (newDirection <= Direction::RIGHT && newDirection >= Direction::UP) _direction = newDirection;
        else return false;

        return true;
    }

    bool EightPuzzle::Action::setAction (const Direction newDirection, const unsigned int Target)
    {
        if (!setDirection(newDirection)) return false;
        if (setTarget(Target)) return false;
        else return true;
    }

    bool EightPuzzle::Action::setAction (const string & action)
    {
        if (!setTarget(action[0] - '1'))   return false;
        else if (!setDirection(action[1]))  return false;
        else return true;
    }

    bool EightPuzzle::playUser(const string strAction)
    {
        Action action;
        if (!action.setAction(strAction) || !moveCell(action)) return false;
        else return true;
    }

    void EightPuzzle::playAuto()
    {
        if(endGame())   return; //Do not search for ,oves if the game has ended

        Action tempAction;
        const Action::Direction directionList[5] = {Action::Direction::UP, Action::Direction::DOWN, Action::Direction::LEFT, Action::Direction::RIGHT}; //List of possible directions

       
        while(1)
        {
            /*iterate through the labels, and test each direction for valid movements*/
            for (int Target = 1 ; (Target < 9) && (Target > 0); ++Target)
                {
                    tempAction.setTarget(Target); //Set the label of the chosen target
                    for (auto tempDir : directionList) //iterate through the direction list
                    {
                        tempAction.setDirection(tempDir);   //Set the chosen direction
                        if (rand()%5 == 1 && moveCell(tempAction))  //The action has a chance of 20% to be used
                            return;
                    }
                } 
        }
    }

    bool EightPuzzle::checkAction(Action action) const
    {
        for (int row = 0; row < 3; ++row)
            for (int column = 0; column < 3; ++column)
                if (getCellState(row, column) == action.getTarget()) //Find the target with given label
                {
                    if (!(column + action.getDirection2D()[1] >= 3 || column + action.getDirection2D()[1] < 0 ||
                          row + action.getDirection2D()[0] >= 3 || row + action.getDirection2D()[0] < 0)) //Check if the action is inside the bounds
                    {
                        if (getCellState(row  + action.getDirection2D()[0], column + action.getDirection2D()[1]) == 0)  //Check if an empty Cell is adjacent to the target in the direction
                            return true;

                        //Keep calling the function recursively until finding an empty cell in the direction
                        else return checkAction(Action(action.getDirection(), getCellState(row  + action.getDirection2D()[0], column + action.getDirection2D()[1])));
                    }

                    else return false; //Return false if action is out of bounds
                }
            

        return false;
    }

    bool EightPuzzle::moveCell(const Action action)
    {
        const int * direction2D = action.getDirection2D();

        for (int row = 0; row < 3; ++row)
            for (int column = 0; column < 3; ++column)
                if (getCellState(row, column) == action.getTarget())    //Find the target with given label
                {
                    if (!(column + action.getDirection2D()[1] >= 3 || column + action.getDirection2D()[1] < 0 ||
                        row + action.getDirection2D()[0] >= 3 || row + action.getDirection2D()[0] < 0)) //Check if the action is inside the bounds
                    {
                        if (getCellState(row  + action.getDirection2D()[0], column + action.getDirection2D()[1]) == 0)  //Check if an empty Cell is adjacent to the target in the direction
                        {
                            setCellState(row  + action.getDirection2D()[0], column + action.getDirection2D()[1], action.getTarget());
                            setCellState(row, column, 0);
                            return true;
                        }

                        //Keep calling the function recursively until finding an empty cell in the direction
                        else if (moveCell(Action(action.getDirection(), getCellState(row  + action.getDirection2D()[0], column + action.getDirection2D()[1]))))
                        {
                            setCellState(row  + action.getDirection2D()[0], column + action.getDirection2D()[1], action.getTarget());
                            setCellState(row, column, 0);
                            return true;
                        }

                        else return false; //Return false if no empty cell was found in the direction
                    }

                    else return false; //Return false if action is out of bounds
                }


        return false;
    }

    bool EightPuzzle::endGame() const
    {
        EightPuzzle tempBoard; 
        tempBoard.initialize();

        //The game will end if the current board is the same as the initial one
        for (int row = 0; row < 3; ++row)
            for (int column = 0; column < 3; ++column)
                if (_board[row][column] != tempBoard.getCellState(row, column)) 
                    return false;   //Return false if a difference was found

        return true;
    }

    int EightPuzzle::boardScore() const
    {

        EightPuzzle tempBoard;
        int score = 0;

        tempBoard.initialize();
        for (int row = 0; row < 3; ++row)
            for (int column = 0; column < 3; ++column)
                if (_board[row][column] != tempBoard.getCellState(row, column)) ++score;    //Count the Block in the current board that differ from the initial board with their position

        return score;
    }

    void EightPuzzle::initialize()
    {
        //Initial board
        int baseBoard[3][3] =  {{1, 2, 3},
                                {4, 5, 6},
                                {7, 8, 0}};

        //Copy the initial board to the current one
        for (int row = 0; row < 3; ++row)
            for (int column = 0; column < 3; ++column)
                setCellState(row, column, baseBoard[row][column]);
    }

    void EightPuzzle::shuffle()
    {
        Action action;
        Action::Direction directionList[5] = {Action::Direction::UP, Action::Direction::DOWN, Action::Direction::LEFT, Action::Direction::RIGHT};

        //Do random actions on the board about 50 times to shuffle it
        for (int i = 0; i < 50; ++i) 
            for (int Target = 1; Target < 9; ++Target)
                for (auto direction : directionList)
                {
                    if (rand()%5 == 1)  //50% chance for an action to bge used
                    {
                        action.setAction(direction, Target);
                        moveCell(action);
                    }
                }
    }

    void EightPuzzle::print() const
    {
        /*print each cell with it's label inside*/
        for (int row = 0; row < 3; ++row)
        {
            cout << "-------------\n";

            for (auto cell : _board[row])
                cout << "| " << cell << ' ';

            cout << '|' << endl;                
        }
    }


    int Klotski::Action::_directionList[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    int Klotski::Block::_extraBlockList[4][2] = {{-1, 0}, {0, -1}, {-1, -1}, {0, 0}};
    
    bool Klotski::Action::setTarget(const int Target)
    {
        if (Target <= 10 && Target > 0)
        {
            _Target = Target;
            return true;
        }

        else return false;
    }

    bool Klotski::Action::setDirection (const char newDirection)
    {
        if (newDirection == 'U' || newDirection == 'u')   setDirection(Direction::UP);
        else if (newDirection == 'D' || newDirection == 'd')   setDirection(Direction::DOWN);
        else if (newDirection == 'L' || newDirection == 'l')   setDirection(Direction::LEFT);
        else if (newDirection == 'R' || newDirection == 'r')   setDirection(Direction::RIGHT);
        else return false;

        return true;
    }

    bool Klotski::Action::setDirection (const Direction newDirection)
    {
        if (newDirection <= Direction::RIGHT && newDirection >= Direction::UP) _direction = newDirection;
        else return false;

        return true;
    }

    bool Klotski::Action::setAction (const Direction newDirection, const int Target)
    {
        if (!setDirection(newDirection)) return false;
        if (setTarget(Target)) return false;
        else return true;
    }

    bool Klotski::Action::setAction (const string & action)
    {
        if (action[1] == '0')
        {
            if (!setTarget((action[0] - '0')*10)) return false;
            else if (!setDirection(action[3]))  return false;
            else return true;
        }

        else if (!setTarget(action[0] - '0'))   return false;
        else if (!setDirection(action[2]))  return false;
        else return true;
    }
 
    bool Klotski::playUser(const string strAction)
    {
        Action action;
        if (!action.setAction(strAction) || !moveBlock(action))    return false;
        else return true;
    }

    void Klotski::playAuto()
    {
        if(endGame())   return;

        Action tempAction;
        const Direction directionList[5] = {Direction::DOWN, Direction::LEFT, Direction::RIGHT, Direction::UP};

        while(1)
        {
            /*iterate through the labels*/
            for (int label = 10; label > 0; --label)
            {
                tempAction.setTarget(label);    //Set the label
                for (auto tempDir : directionList)  //Iterate through the direction list
                {
                    tempAction.setDirection(tempDir);   //Set the direction
                    if (tempAction.getTarget() != _lastAction.getTarget() || //If the new target isn't the the one in the last action, no need to check for action repetition
                        //Be sure the current action does not undo the last one
                        (tempAction.getDirection() == Direction::UP && _lastAction.getDirection() != Direction::DOWN) || (tempAction.getDirection() == Direction::DOWN && _lastAction.getDirection() != Direction::UP) &&
                        (tempAction.getDirection() == Direction::LEFT && _lastAction.getDirection() != Direction::RIGHT) || (tempAction.getDirection() == Direction::RIGHT && _lastAction.getDirection() != Direction::LEFT))
                        if (rand()%5 == 1 && moveBlock(tempAction)) //The action has a chance of 20% to be used
                        {
                            _lastAction.setAction(tempAction.getDirection(), tempAction.getTarget());   //Update the last action
                            return;
                        }
                }
            }
            
            _lastAction = Action(); //reset the last action to prevent being stuck in some situations
        }
    }

    bool Klotski::checkAction(Action action) const
    {
        //Each step in he following has it's purpose commented above it

        Block target;

        for (int row = 0; row < 5; ++row)
            for (int column = 0; column < 4; ++column)
            {
                //Find the target with the given label
                if (getBlock(row, column).getLabel() == action.getTarget())
                {
                    //Store the target Block
                    target = getBlock(row, column);

                    //Check if the action is in the bounds
                    if (row + action.getDirection2D()[0] < 5 && row + action.getDirection2D()[0] >= 0 &&
                        column + action.getDirection2D()[1] < 4 && column + action.getDirection2D()[1] >= 0 &&
                        row + action.getDirection2D()[0] + target.getExtraBlocks()[0] < 5 && row + action.getDirection2D()[0] + target.getExtraBlocks()[0] >= 0 &&
                        column + action.getDirection2D()[1] + target.getExtraBlocks()[1] < 4 && column + action.getDirection2D()[1] + target.getExtraBlocks()[1] >= 0)
                    {
                        // 1x1
                        if (target.getType() == Block_t::Block_1x1)
                            if (getBlock(row + action.getDirection2D()[0], column + action.getDirection2D()[1]).getType() == Block_t::empty)
                                return true;

                            else return false;

                        // 2x1 and 1x2
                        else if (target.getType() == Block_t::Block_2x1 || target.getType() == Block_t::Block_1x2)
                            if ((// 2x1 horizontal and 1x2 vertical
                                 getBlock(row + action.getDirection2D()[0], column + action.getDirection2D()[1]).getType() == Block_t::empty &&
                                 getBlock(row + target.getExtraBlocks()[0] + action.getDirection2D()[0], column + target.getExtraBlocks()[1] + action.getDirection2D()[1]).getType() == Block_t::empty) ||

                                (// 2x1 Down and 1x2 Right
                                 action.getDirection2D()[0] == target.getExtraBlocks()[0] * -1 && action.getDirection2D()[1] == target.getExtraBlocks()[1] * -1 &&
                                 getBlock(row + action.getDirection2D()[0], column + action.getDirection2D()[1]).getType() == Block_t::empty))
                                return true;

                            else if (// 2x1 Up and 1x2 Left
                                     action.getDirection2D()[0] == target.getExtraBlocks()[0] && action.getDirection2D()[1] == target.getExtraBlocks()[1] &&
                                     getBlock(row + target.getExtraBlocks()[0] + action.getDirection2D()[0], column + target.getExtraBlocks()[1] + action.getDirection2D()[1]).getType() == Block_t::empty)
                                return true;

                            else return false;

                        // 2x2
                        else if (target.getType() == Block_t::Block_2x2)
                            //Up
                            if (action.getDirection() == Direction::UP && 
                                getBlock(row + target.getExtraBlocks(2)[0] + action.getDirection2D()[0], column + target.getExtraBlocks(2)[1] + action.getDirection2D()[1]).getType() == Block_t::empty &&
                                getBlock(row + target.getExtraBlocks(0)[0] + action.getDirection2D()[0], column + target.getExtraBlocks(0)[1] + action.getDirection2D()[1]).getType() == Block_t::empty)
                                return true;

                            //Down
                            else if(action.getDirection() == Direction::DOWN &&
                                    getBlock(row + action.getDirection2D()[0], column + action.getDirection2D()[1]).getType() == Block_t::empty &&
                                    getBlock(row + target.getExtraBlocks(1)[0] + action.getDirection2D()[0], column + target.getExtraBlocks(1)[1] + action.getDirection2D()[1]).getType() == Block_t::empty)
                                return true;

                            //Left
                            else if(action.getDirection() == Direction::LEFT &&
                                    getBlock(row + target.getExtraBlocks(2)[0] + action.getDirection2D()[0], column + target.getExtraBlocks(2)[1] + action.getDirection2D()[1]).getType() == Block_t::empty &&
                                    getBlock(row + target.getExtraBlocks(1)[0] + action.getDirection2D()[0], column + target.getExtraBlocks(1)[1] + action.getDirection2D()[1]).getType() == Block_t::empty)
                                return true;

                            //Right
                            else if(action.getDirection() == Direction::RIGHT &&
                                    getBlock(row + action.getDirection2D()[0], column + action.getDirection2D()[1]).getType() == Block_t::empty &&
                                    getBlock(row + target.getExtraBlocks(0)[0] + action.getDirection2D()[0], column + target.getExtraBlocks(0)[1] + action.getDirection2D()[1]).getType() == Block_t::empty)
                                return true;
                        
                            else return false;


                        else return false;
                    }
                
                    else if(target.getType() == Block_t::Block_2x2 && 
                            action.getDirection() == Direction::DOWN &&
                            getBlock(row + action.getDirection2D()[0], column + action.getDirection2D()[1]).getType() == Block_t::Goal &&
                            getBlock(row + target.getExtraBlocks(1)[0] + action.getDirection2D()[0], column + target.getExtraBlocks(1)[1] + action.getDirection2D()[1]).getType() == Block_t::Goal)
                            return true;
                }

                else return false;
            }

        return false;
    }

    bool Klotski::moveBlock(const Action action)
    {
        //Each step in he following has it's purpose commented above it

        Block target;

        for (int row = 0; row < 5; ++row)
            for (int column = 0; column < 4; ++column)
            {
                //Find the target with the given label
                if (getBlock(row, column).getLabel() == action.getTarget())
                {
                    //Store the target Block
                    target = getBlock(row, column);

                    //Check if the action is in the bounds
                    if (row + action.getDirection2D()[0] < 5 && row + action.getDirection2D()[0] >= 0 &&
                        column + action.getDirection2D()[1] < 4 && column + action.getDirection2D()[1] >= 0 &&
                        row + action.getDirection2D()[0] + target.getExtraBlocks()[0] < 5 && row + action.getDirection2D()[0] + target.getExtraBlocks()[0] >= 0 &&
                        column + action.getDirection2D()[1] + target.getExtraBlocks()[1] < 4 && column + action.getDirection2D()[1] + target.getExtraBlocks()[1] >= 0)
                    {
                        // 1x1 Blocks
                        if (target.getType() == Block_t::Block_1x1)
                            if (getBlock(row + action.getDirection2D()[0], column + action.getDirection2D()[1]).getType() == Block_t::empty)
                            {
                                setBlock(row + action.getDirection2D()[0], column + action.getDirection2D()[1], target);
                                setBlock(row, column, Block(Block_t::empty, -1));
                                return true;
                            }

                            else return false;

                        // 2x1 and 1x2 Blocks
                        else if (target.getType() == Block_t::Block_2x1 || target.getType() == Block_t::Block_1x2)
                            if ((// 2x1 horizontal and 1x2 vertical
                                 getBlock(row + action.getDirection2D()[0], column + action.getDirection2D()[1]).getType() == Block_t::empty &&
                                 getBlock(row + target.getExtraBlocks()[0] + action.getDirection2D()[0], column + target.getExtraBlocks()[1] + action.getDirection2D()[1]).getType() == Block_t::empty) ||

                                (// 2x1 Down and 1x2 Right
                                 action.getDirection2D()[0] == target.getExtraBlocks()[0] * -1 && action.getDirection2D()[1] == target.getExtraBlocks()[1] * -1 &&
                                 getBlock(row + action.getDirection2D()[0], column + action.getDirection2D()[1]).getType() == Block_t::empty))
                            {
                                setBlock(row + action.getDirection2D()[0], column + action.getDirection2D()[1], target);
                                setBlock(row, column, Block(Block_t::empty, -1));
                                setBlock(row + action.getDirection2D()[0] + target.getExtraBlocks()[0], column + action.getDirection2D()[1] + target.getExtraBlocks()[1], Block(target.getType(), 0));
                                setBlock(row + target.getExtraBlocks()[0], column + target.getExtraBlocks()[1], Block(Block_t::empty, -1));
                                return true;
                            }

                            else if (// 2x1 Up and 1x2 Left
                                     action.getDirection2D()[0] == target.getExtraBlocks()[0] && action.getDirection2D()[1] == target.getExtraBlocks()[1] &&
                                     getBlock(row + target.getExtraBlocks()[0] + action.getDirection2D()[0], column + target.getExtraBlocks()[1] + action.getDirection2D()[1]).getType() == Block_t::empty)
                            {
                                setBlock(row + action.getDirection2D()[0] + target.getExtraBlocks()[0], column + action.getDirection2D()[1] + target.getExtraBlocks()[1], Block(target.getType(), 0));
                                setBlock(row + action.getDirection2D()[0], column + action.getDirection2D()[1], target);
                                setBlock(row, column, Block(Block_t::empty, -1));
                                return true;
                            }

                            else return false;

                        // 2x2 Blocks
                        else if (target.getType() == Block_t::Block_2x2)
                                //Up
                                if (action.getDirection() == Direction::UP && 
                                    getBlock(row + target.getExtraBlocks(2)[0] + action.getDirection2D()[0], column + target.getExtraBlocks(2)[1] + action.getDirection2D()[1]).getType() == Block_t::empty &&
                                    getBlock(row + target.getExtraBlocks(0)[0] + action.getDirection2D()[0], column + target.getExtraBlocks(0)[1] + action.getDirection2D()[1]).getType() == Block_t::empty)
                                {
                                    setBlock(row + target.getExtraBlocks(0)[0] + action.getDirection2D()[0], column + target.getExtraBlocks(0)[1] + action.getDirection2D()[1], Block(target.getType(), 0));
                                    setBlock(row + target.getExtraBlocks(2)[0] + action.getDirection2D()[0], column + target.getExtraBlocks(2)[1] + action.getDirection2D()[1], Block(target.getType(), 0));
                                    setBlock(row + target.getExtraBlocks(1)[0] + action.getDirection2D()[0], column + target.getExtraBlocks(1)[1] + action.getDirection2D()[1], Block(target.getType(), 0));
                                    setBlock(row + target.getExtraBlocks(1)[0], column + target.getExtraBlocks(1)[1], Block(Block_t::empty, -1));
                                    setBlock(row + action.getDirection2D()[0], column + action.getDirection2D()[1], target);
                                    setBlock(row, column, Block(Block_t::empty, -1));
                                    return true;
                                }

                                //Down
                                else if(action.getDirection() == Direction::DOWN &&
                                        getBlock(row + action.getDirection2D()[0], column + action.getDirection2D()[1]).getType() == Block_t::empty &&
                                        getBlock(row + target.getExtraBlocks(1)[0] + action.getDirection2D()[0], column + target.getExtraBlocks(1)[1] + action.getDirection2D()[1]).getType() == Block_t::empty)
                                {
                                    setBlock(row + target.getExtraBlocks(1)[0] + action.getDirection2D()[0], column + target.getExtraBlocks(1)[1] + action.getDirection2D()[1], Block(target.getType(), 0));
                                    setBlock(row + action.getDirection2D()[0], column + action.getDirection2D()[1], target);
                                    setBlock(row + target.getExtraBlocks(2)[0] + action.getDirection2D()[0], column + target.getExtraBlocks(2)[1] + action.getDirection2D()[1], Block(target.getType(), 0));
                                    setBlock(row + target.getExtraBlocks(2)[0], column + target.getExtraBlocks(2)[1], Block(Block_t::empty, -1));
                                    setBlock(row + target.getExtraBlocks(0)[0] + action.getDirection2D()[0], column + target.getExtraBlocks(0)[1] + action.getDirection2D()[1], Block(target.getType(), 0));
                                    setBlock(row + target.getExtraBlocks(0)[0], column + target.getExtraBlocks(0)[1], Block(Block_t::empty, -1));
                                    return true;
                                }

                                //Left
                                else if(action.getDirection() == Direction::LEFT &&
                                        getBlock(row + target.getExtraBlocks(2)[0] + action.getDirection2D()[0], column + target.getExtraBlocks(2)[1] + action.getDirection2D()[1]).getType() == Block_t::empty &&
                                        getBlock(row + target.getExtraBlocks(1)[0] + action.getDirection2D()[0], column + target.getExtraBlocks(1)[1] + action.getDirection2D()[1]).getType() == Block_t::empty)
                                {
                                    setBlock(row + target.getExtraBlocks(2)[0] + action.getDirection2D()[0], column + target.getExtraBlocks(2)[1] + action.getDirection2D()[1], Block(target.getType(), 0));
                                    setBlock(row + target.getExtraBlocks(1)[0] + action.getDirection2D()[0], column + target.getExtraBlocks(1)[1] + action.getDirection2D()[1], Block(target.getType(), 0));
                                    setBlock(row + target.getExtraBlocks(0)[0] + action.getDirection2D()[0], column + target.getExtraBlocks(0)[1] + action.getDirection2D()[1], Block(target.getType(), 0));
                                    setBlock(row + target.getExtraBlocks(0)[0], column + target.getExtraBlocks(0)[1], Block(Block_t::empty, -1));
                                    setBlock(row + action.getDirection2D()[0], column + action.getDirection2D()[1], target);
                                    setBlock(row, column, Block(Block_t::empty, -1));
                                    return true;
                                }

                                //Right
                                else if(action.getDirection() == Direction::RIGHT &&
                                        getBlock(row + action.getDirection2D()[0], column + action.getDirection2D()[1]).getType() == Block_t::empty &&
                                        getBlock(row + target.getExtraBlocks(0)[0] + action.getDirection2D()[0], column + target.getExtraBlocks(0)[1] + action.getDirection2D()[1]).getType() == Block_t::empty)
                                {
                                    setBlock(row + action.getDirection2D()[0], column + action.getDirection2D()[1], target);
                                    setBlock(row + target.getExtraBlocks(0)[0] + action.getDirection2D()[0], column + target.getExtraBlocks(0)[1] + action.getDirection2D()[1], Block(target.getType(), 0));
                                    setBlock(row + target.getExtraBlocks(1)[0] + action.getDirection2D()[0], column + target.getExtraBlocks(1)[1] + action.getDirection2D()[1], Block(target.getType(), 0));
                                    setBlock(row + target.getExtraBlocks(1)[0], column + target.getExtraBlocks(1)[1], Block(Block_t::empty, -1));
                                    setBlock(row + target.getExtraBlocks(2)[0] + action.getDirection2D()[0], column + target.getExtraBlocks(2)[1] + action.getDirection2D()[1], Block(target.getType(), 0));
                                    setBlock(row + target.getExtraBlocks(2)[0], column + target.getExtraBlocks(2)[1], Block(Block_t::empty, -1));
                                    return true;
                                }
                            
                                else return false;


                        else return false;
                    }

                    // 2x2 Block reaching the Goal
                    else if(target.getType() == Block_t::Block_2x2 && 
                            action.getDirection() == Direction::DOWN &&
                            getBlock(row + action.getDirection2D()[0], column + action.getDirection2D()[1]).getType() == Block_t::Goal &&
                            getBlock(row + target.getExtraBlocks(1)[0] + action.getDirection2D()[0], column + target.getExtraBlocks(1)[1] + action.getDirection2D()[1]).getType() == Block_t::Goal)
                            {
                                setBlock(row + target.getExtraBlocks(1)[0] + action.getDirection2D()[0], column + target.getExtraBlocks(1)[1] + action.getDirection2D()[1], Block(target.getType(), 0));
                                setBlock(row + action.getDirection2D()[0], column + action.getDirection2D()[1], target);
                                setBlock(row + target.getExtraBlocks(2)[0] + action.getDirection2D()[0], column + target.getExtraBlocks(2)[1] + action.getDirection2D()[1], Block(target.getType(), 0));
                                setBlock(row + target.getExtraBlocks(2)[0], column + target.getExtraBlocks(2)[1], Block(Block_t::empty, -1));
                                setBlock(row + target.getExtraBlocks(0)[0] + action.getDirection2D()[0], column + target.getExtraBlocks(0)[1] + action.getDirection2D()[1], Block(target.getType(), 0));
                                setBlock(row + target.getExtraBlocks(0)[0], column + target.getExtraBlocks(0)[1], Block(Block_t::empty, -1));
                                
                                return true;
                            }

                    else return false;
                }
            }

        return false;
    }

    bool Klotski::endGame() const
    {
        if(getBlock(5, 2).getLabel() == 10)   return true;  //If the block with the label 10 reached the Goal, then the game ended
        else return false;
    }

    int Klotski::boardScore() const
    {
        for (int row = 0; row < 5; ++row)
            for (int column = 0; column < 4; ++column)
                if (getBlock(row, column).getLabel() == 10) //Find the distance between the block with the label 10 and the Goal
                    return 5 - row;

        return 0;
    }

    void Klotski::print() const
    {
        //Each step means the row of characters for a Block (a 1x1 Block is 3x3 characters)

        int column;
        /*then print the row numbers with the cells iteratively*/
        cout << "_________________________";

        for (int row = 0; row < 5; ++row)
        {
            cout << "\n|";

            //Step 1
            if (getBlock(row, 0).getType() == Block_t::Block_1x1)
            {
                cout << "-----";
                column = 1;
            }

            else if (getBlock(row, 0).getType() == Block_t::Block_2x1)
            {
                if(getBlock(row, 0).getLabel() == 0)
                    cout << "-----";

                else cout << "|   |";

                column = 1;
            }

            else if (getBlock(row, 0).getType() == Block_t::Block_1x2)
            {
                cout << "-----------";
                column = 2;
            }

            else if (getBlock(row, 0).getType() == Block_t::Block_2x2)
            {
                if(getBlock(row + 1, 0).getType() == Block_t::Block_2x2)
                    cout << "-----------";

                else cout << "|         |";

                column = 2;
            }

            else if (getBlock(row, 0).getType() == Block_t::empty)
            {
                cout << "     ";
                column = 1;
            }
            
            for (; column < 4; ++column)
            {
                if (getBlock(row, column).getType() == Block_t::Block_1x1)
                    cout << " -----";

                else if (getBlock(row, column).getType() == Block_t::Block_2x1)
                {
                    if (getBlock(row, column).getLabel() == 0)
                        cout << " -----";

                    else cout << " |   |";
                }

                else if (getBlock(row, column).getType() == Block_t::Block_1x2)
                {
                    cout << " -----------";
                    ++column;
                }

                else if (getBlock(row, column).getType() == Block_t::Block_2x2)
                {
                    if(getBlock(row + 1, column).getType() == Block_t::Block_2x2)
                        cout << " -----------";

                    else cout << " |         |";

                    ++column;
                }

                else if (getBlock(row, column).getType() == Block_t::empty)  cout << "      ";
            }


            cout << "|\n|";

            //Step 2
            if (getBlock(row, 0).getType() == Block_t::Block_1x1)
            {
                cout << "| " << getBlock(row, 0).getLabel() << " |";
                column = 1;
            }

            else if (getBlock(row, 0).getType() == Block_t::Block_2x1)
            {
                cout << "|   |";
                column = 1;
            }

            else if (getBlock(row, 0).getType() == Block_t::Block_1x2)
            {
                cout << "|    " << getBlock(row, 1).getLabel() << "    |";
                column = 2;
            }

            else if (getBlock(row, 0).getType() == Block_t::Block_2x2)
            {
                cout << "|         |";
                column = 2;
            }

            else if (getBlock(row, 0).getType() == Block_t::empty)
            {
                cout << "     ";
                column = 1;
            }
            
            for (; column < 4; ++column)
            {
                if (getBlock(row, column).getType() == Block_t::Block_1x1)
                    cout << " | " << getBlock(row, column).getLabel() << " |";

                else if (getBlock(row, column).getType() == Block_t::Block_2x1) cout << " |   |";

                else if (getBlock(row, column).getType() == Block_t::Block_1x2)
                {
                    cout << " |    " << getBlock(row, column + 1).getLabel() << "    |";
                    ++column;
                }

                else if (getBlock(row, column).getType() == Block_t::Block_2x2)
                {
                    cout << " |         |";
                    ++column;
                }

                else if (getBlock(row, column).getType() == Block_t::empty)  cout << "      ";
            }


            cout << "|\n|";

            //Step 3
            if (getBlock(row, 0).getType() == Block_t::Block_1x1)
            {
                cout << "-----";
                column = 1;
            }

            else if (getBlock(row, 0).getType() == Block_t::Block_2x1)
            {
                if(getBlock(row, 0).getLabel() == 0)
                    cout << "| " << getBlock(row + 1, 0).getLabel() << " |";

                else cout << "-----";

                column = 1;
            }

            else if (getBlock(row, 0).getType() == Block_t::Block_1x2)
            {
                cout << "-----------";
                column = 2;
            }

            else if (getBlock(row, 0).getType() == Block_t::Block_2x2)
            {
                if(getBlock(row + 1, 0).getType() == Block_t::Block_2x2)
                    cout << "|    " << 10 << "   |";

                else cout << "-----------";

                column = 2;
            }

            else if (getBlock(row, 0).getType() == Block_t::empty)
            {
                cout << "     ";
                column = 1;
            }
            
            for (; column < 4; ++column)
            {
                if (getBlock(row, column).getType() == Block_t::Block_1x1)
                    cout << " -----";

                else if (getBlock(row, column).getType() == Block_t::Block_2x1)
                {
                    if(getBlock(row, column).getLabel() == 0)
                    cout << " | " << getBlock(row + 1, column).getLabel() << " |";

                    else cout << " -----";
                }

                else if (getBlock(row, column).getType() == Block_t::Block_1x2)
                {
                    cout << " -----------";
                    ++column;
                }

                else if (getBlock(row, column).getType() == Block_t::Block_2x2)
                {
                    if(getBlock(row + 1, column).getType() == Block_t::Block_2x2)
                        cout << " |    " << 10 << "   |";

                    else cout << " -----------";

                    ++column;
                }

                else if (getBlock(row, column).getType() == Block_t::empty)  cout << "      ";
            }
        
            cout << "|";
        }


        //Prints the bottom of the board
        cout << "\n______";

            if (getBlock(5, 1).getType() == Block_t::Block_2x2)
                cout << "||         ||";

            else if (getBlock(5, 1).getType() == Block_t::Goal)
                cout << "|           |";
            
            cout << "______\n      ";

            if (getBlock(5, 1).getType() == Block_t::Block_2x2)
                cout << "||         ||";

            else if (getBlock(5, 1).getType() == Block_t::Goal)
                cout << "|           |";
            
            cout << "      \n      "; 
            
            if (getBlock(5, 1).getType() == Block_t::Block_2x2)
                cout << "|-----------|";

            else if (getBlock(5, 1).getType() == Block_t::Goal)
                cout << "|           |";
    }

    void Klotski::initialize()
    {
        //Initial board
        Block baseBoard[6][4] ={{Block(Block_t::Block_2x1, 0), Block(Block_t::Block_2x2, 0), Block(Block_t::Block_2x2, 0), Block(Block_t::Block_2x1, 0)},
                                {Block(Block_t::Block_2x1, 1), Block(Block_t::Block_2x2, 0), Block(Block_t::Block_2x2,10), Block(Block_t::Block_2x1, 2)},
                                {Block(Block_t::Block_2x1, 0), Block(Block_t::Block_1x2, 0), Block(Block_t::Block_1x2, 4), Block(Block_t::Block_2x1, 0)},
                                {Block(Block_t::Block_2x1, 3), Block(Block_t::Block_1x1, 6), Block(Block_t::Block_1x1, 7), Block(Block_t::Block_2x1, 5)},
                                {Block(Block_t::Block_1x1, 8), Block(Block_t::empty    ,-1), Block(Block_t::empty    ,-1), Block(Block_t::Block_1x1, 9)},
                                {Block(Block_t::Wall     , 0), Block(Block_t::Goal     ,-2), Block(Block_t::Goal     ,-2), Block(Block_t::Wall     , 0)}};

         //copy the Initial board to the current one
        for (int row = 0; row < 6; ++row)
            for (int column = 0; column < 4; ++column)
                setBlock(row, column, baseBoard[row][column]);

        return;
    }
}