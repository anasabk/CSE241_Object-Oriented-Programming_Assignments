#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <fstream>

#define MAX_boardNum 6

using namespace std;


class PegSolitaire
{
    public:
        enum CellShape {
            square,
            hexagon
        };

        enum class CellState {
                null = ' ',
                peg = 'P',
                empty = '.'
            };


        class Cell
        {
            public:
                Cell(int newRow, int newColumn, CellState newState = CellState::null) : state(newState) { setPostion(newRow, newColumn); }
                Cell() : row(0), column(0), state(CellState::null) {}

                bool setPostion(int, int);
                void setState(CellState newState) { state = newState; }
                
                int getRow() const { return row; }
                int getColumn() const { return column; }
                CellState getState() const { return state; }

            private:
                CellState state;
                int row;
                char column;
        };

        class Action {
            enum class Direction{
                UP = 0,
                DOWN = 1,
                LEFT = 2,
                RIGHT = 3,
                Default = 4
            };

            public:
                Action(Direction newDir, Cell newTarget) : target(newTarget) { if (setDirection(newDir) == false)  exit(1); }
                Action() : direction(directionList[4]), target(0, 0) {}

                bool setAction(string &);
                bool setAction(Direction newDir, Cell target);
                void resetAction();

                bool setDirection(Direction);
                bool setTarget(Cell);

                int* getDirection();
                Cell getTarget();
                
            private:
                Cell target;
                const int * direction;
                static int directionList[5][2];
        };

        class Board {
            public:
                Board() : board(), type(CellShape::square), pegNum(0){}
                Board(Board & newBoard) : board(newBoard.getBoard()), type(newBoard.getType()), pegNum(newBoard.getPegNum()){}
                Board(vector<vector<Cell>> newBoard, CellShape newType) : board(newBoard), type(newType), pegNum(countPegs()){}

                bool setBoard(Board &);
                bool setBoard(vector<vector<Cell>> &, CellShape);

                vector<vector<Cell>> getBoard() { return board; }
                CellShape getType() { return type; }
                int getPegNum() { return pegNum; }
                int countPegs();
                void decrementPegNum() { --pegNum;};
                
                bool printBoard();
                bool printBoard(ofstream &);

            private:
                vector<vector<Cell>> board;
                CellShape type;
                int pegNum;
        };

    
        PegSolitaire(PegSolitaire &newGame) { *this = newGame; }
        PegSolitaire(int);
        PegSolitaire() : movesDone(0), currentAction(), board() {}

        int getPegNum() { return board.getPegNum(); }
        int getEmptyNum() { return board.getPegNum() - getPegsTaken(); }
        int getPegsTaken() { return movesDone*2; }
        int getMovesDone() { return getMovesDone(); }

        bool saveGame(ofstream &);
        bool loadGame(ifstream &);

        bool play();
        bool play(string);

        bool hasEnded();

        void playGame();

        static int totalPegs();

        bool isLargerThan(PegSolitaire);


    private:
        Board board;
        Action currentAction;

        int movesDone;

        static int totalPegNum;

        static Board boardList[1];

        bool setBoard(Board &);
        bool setMovesDone(int);
};


int PegSolitaire::Action::directionList[5][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}, {0, 0}};

PegSolitaire::Board PegSolitaire::boardList[1] = {{{{{0, 0, CellState::null}, {0, 0, CellState::null}, {0, 0, CellState::peg}, {0, 0, CellState::peg}, {0, 0, CellState::peg}, {0, 0, CellState::null}, {0, 0, CellState::null}},
                                                    {{0, 0, CellState::null},  {0, 0, CellState::peg}, {0, 0, CellState::peg}, {0, 0, CellState::peg}, {0, 0, CellState::peg},  {0, 0, CellState::peg}, {0, 0, CellState::null}},
                                                    { {0, 0, CellState::peg},  {0, 0, CellState::peg}, {0, 0, CellState::peg}, {0, 0, CellState::peg}, {0, 0, CellState::peg},  {0, 0, CellState::peg},  {0, 0, CellState::peg}},
                                                    { {0, 0, CellState::peg},  {0, 0, CellState::peg}, {0, 0, CellState::peg}, {0, 0, CellState::peg}, {0, 0, CellState::peg},  {0, 0, CellState::peg},  {0, 0, CellState::peg}},
                                                    { {0, 0, CellState::peg},  {0, 0, CellState::peg}, {0, 0, CellState::peg}, {0, 0, CellState::peg}, {0, 0, CellState::peg},  {0, 0, CellState::peg},  {0, 0, CellState::peg}},
                                                    {{0, 0, CellState::null},  {0, 0, CellState::peg}, {0, 0, CellState::peg}, {0, 0, CellState::peg}, {0, 0, CellState::peg},  {0, 0, CellState::peg}, {0, 0, CellState::null}},
                                                    {{0, 0, CellState::null}, {0, 0, CellState::null}, {0, 0, CellState::peg}, {0, 0, CellState::peg}, {0, 0, CellState::peg}, {0, 0, CellState::null}, {0, 0, CellState::null}}},

                                                    CellShape::square}};


bool PegSolitaire::Cell::setPostion(int rowNum, int columnNum)
{
    if(rowNum < 0 || columnNum < 0)   return false;
    else
    {
        row = rowNum;
        column = columnNum + 'a';
        return true;
    }
}


bool PegSolitaire::Action::setDirection(Direction newDir)
{
    if (newDir == Direction::UP || newDir == Direction::DOWN ||
        newDir == Direction::LEFT || newDir == Direction::RIGHT)
    {
        direction = directionList[(int)newDir];
        return true;
    }

    else return false;
}


bool PegSolitaire::Action::setTarget(Cell newTarget)
{
    if (newTarget.getColumn() > 0 && newTarget.getRow() > 0)
    {
        target = newTarget;
        return true;
    }

    else return false;
}

/*Translate the string command of the action to an action_t struct*/
bool PegSolitaire::Action::setAction (string &movement)
{
    int counter = 0,
        column,
        row;

    /*transform the column letters to integers, weither they are capital or small letters and store them as the column*/
    if (movement[counter] >= 'A' && movement[counter] <= 'Z')   column = movement[counter] - 'A';
    else if (movement[counter] >= 'a' || movement[counter] <= 'z')    column = movement[counter] - 'a';
    else return false;

    ++counter;

    /*store the integer value of the row*/
    if (movement[counter] > '0' && movement[counter] <= '9') row = movement[counter] - '1';
    else return false;

    ++counter;

    if (movement[counter] > '0' && movement[counter] <= '9')    row = (row)*10 + movement[counter] - '1';

    else if (movement[counter] != '-')  return false;

    else ++counter;

    /*store the direction of the action based on the letter of the given direction*/
    if (movement[counter] == 'U' || movement[counter] == 'u')   return setAction(Direction::UP, {row, column});
    else if (movement[counter] == 'D' || movement[counter] == 'd')   return setAction(Direction::DOWN, {row, column});
    else if (movement[counter] == 'L' || movement[counter] == 'l')   return setAction(Direction::LEFT, {row, column});
    else if (movement[counter] == 'R' || movement[counter] == 'r')   return setAction(Direction::RIGHT, {row, column});
    else return false;
}


bool PegSolitaire::Action::setAction (Direction newDir, Cell newTarget)
{
    if (setTarget(newTarget) && setDirection(newDir)) return true;
    else
    {
        resetAction();
        return false;
    }
}


void PegSolitaire::Action::resetAction()
{
    setTarget({0, 0});
    setDirection(Direction::Default);
}


bool PegSolitaire::Board::setBoard(Board & newBoard)
{
    if(newBoard.getBoard().size() != newBoard.getBoard()[0].size() ||
       newBoard.getBoard().size() < 5)  return false;

    else
    {
        *this = newBoard;
        return true;
    }
}


bool PegSolitaire::Board::setBoard(vector<vector<Cell>> & newBoard, CellShape newType)
{
    if(newBoard.size() != newBoard[0].size() ||
       newBoard.size() < 5)  return false;

    else
    {
        board = newBoard;
        type = newType;
        return true;
    }
}


int PegSolitaire::Board::countPegs()
{
    int counter = 0;

    for (int row = 0; row < board.size(); ++row)
        for (auto cell : board[row])
            if (cell.getState() == CellState::peg)  ++counter;

    return counter;
}


bool PegSolitaire::Board::printBoard(ofstream &outStream)
{
    int column, row;
    char column_c;

    if ( !outStream.is_open() ) return false;
    
    /*Printing mode for square shaped cells*/
    if (getType() == CellShape::square)
    {
        outStream << "      ";

        /*print the column letters first*/
        for ((column = 0) , (column_c = 'a') ; column < getBoard()[0].size() ; (++column) && (++column_c))
            outStream << column_c << ' ';

        outStream << "\n\n";

        /*then print the row numbers with the cells iteratively*/
        for (int row = 0; row < getBoard().size(); ++row)
        {
            outStream << "  " << row + 1 << "  ";

            if ( row < 9 )  outStream << ' ';

            for (auto cell : getBoard()[row])
                outStream << (char)cell.getState() << ' ';

            outStream << endl;
        }
    }
    
    return true;
}


bool PegSolitaire::Board::printBoard()
{
    int column, row;
    char column_c;

    /*Printing mode for square shaped cells*/
    if (getType() == CellShape::square)
    {
        cout << "      ";

        /*print the column letters first*/
        for ((column = 0) , (column_c = 'a') ; column < getBoard()[0].size() ; (++column) && (++column_c))
            cout << column_c << ' ';

        cout << "\n\n";

        /*then print the row numbers with the cells iteratively*/
        for (int row = 0; row < getBoard().size(); ++row)
        {
            cout << "  " << row + 1 << "  ";

            if ( row < 9 )  cout << ' ';

            for (auto cell : getBoard()[row])
                cout << (char)cell.getState() << ' ';

            cout << endl;
        }
    }
    
    return true;
}


PegSolitaire::PegSolitaire(int boardNum) : movesDone(0), currentAction()
{
    if (boardNum < 0 || boardNum > MAX_boardNum) exit(1);
    else    board = boardList[boardNum];
}




// ostream &operator<<(ostream &stream, PegSolitaire game);