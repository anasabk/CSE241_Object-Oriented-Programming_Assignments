#ifndef HEADER_H
#define HEADER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unistd.h>

using std::vector;
using std::ostream;
using std::ifstream;
using std::ofstream;
using std::string;
using std::cout;
            
namespace Gaming
{
    class BoardGame2D
    {
        public:
        virtual bool playUser(const string) = 0;  //Take actions from user and perform them
        virtual void playUser() final;   //Keep taking actions in a loop until the game ends

        virtual void playAuto() = 0;    //play a computer generated action
        void playAutoAll();    //finish the game using computer generated actions (print the board on each action and leave a delay between actions)

        friend ostream & operator<<(ostream &, const BoardGame2D &);    //print the board to the terminal starting from the top-left corner using ANSI escape sequence
        friend ofstream & operator<<(ofstream &, const BoardGame2D &);  
        virtual void print() const = 0;

        virtual bool endGame() const = 0; //Return true if the game is finished
        virtual int boardScore() const = 0; //return the score of the game, the lower the number the better the score is. (best score is 0)

        virtual void initialize() = 0; //initialize the board. It might result in the same board, or a randomly generated one

        //Play all the games in the vector
        static void playVector(vector<BoardGame2D> listofGames)
        { 
            for (int counter = 0; counter < listofGames.size(); ++counter)
                listofGames[counter].playAutoAll();
        }
    };

    //Class for Peg Solitaire Game
    class PegSolitaire : public BoardGame2D
    {
        public:
        //Enum for representing the cells of a board in terms of characters
        enum class CellState : char {
            null = ' ',
            peg = 'P',
            empty = '.'
        };

        //Class for storing an action in the table
        class Action {
            public:
            //Enum to represent posible directions of a peg in the board
            enum class Direction : int {
                UP = 0,
                DOWN = 1,
                LEFT = 2,
                RIGHT = 3
            };
            
            //Constructors
            Action (const Direction newDir, const unsigned int row, const unsigned int column) : _row(row), _column(column), _direction(newDir) {}
            Action () : _row(0), _column(0),_direction(Direction::UP) {}
            Action (const Action & newAction) : _row(newAction.getRow()), _column(newAction.getColumn()), _direction(newAction.getDirection()) {}

            //Setters
            bool setAction (const Direction, const unsigned int row, const unsigned int column);
            bool setAction (const string &);
            bool setDirection (const Direction);
            bool setDirection (const char);
            bool setTarget (const unsigned int row, const unsigned int column);

            //Getters
            const int * getDirection2D () const { return _directionList[(int)_direction]; } //Get direction as a vector in 2D
            Direction getDirection () const { return _direction; }
            unsigned int getRow () const { return _row; }
            unsigned int getColumn () const { return _column; }


            private:
            unsigned int _row, _column; //Coordinates of the target
            Direction _direction;
            static int _directionList[4][2];    //List of the possible directions in terms of 2D vectors
        };

        //Constructors
        PegSolitaire() { initialize(); }

        //Setters
        inline void setCellState(const unsigned int row, const unsigned int column, const CellState newState) { _board[row][column] = newState; }   //Replace the Cell state in the given location with the given new state

        //Getters
        inline CellState getCellState(unsigned int row, unsigned int column) const { return _board[row][column]; } //Return the Cell state of the cell in the given location

        //Other functions
        bool playUser(const string) override;  //Take actions from user and perform them
        void playAuto();    //play a computer generated action

        void print() const;    //Print the board to an output stream

        bool endGame() const; //Return true if the game is finished
        int boardScore() const; //return the score of the game, the lower the number the better the score is. (best score is 0)

        void initialize(); //initialize the board. It might result in the same board, or a randomly generated one

        bool movePeg(const Action);   //Perform the given action on the board
        bool checkAction(Action) const;   //Check if the given action is valid

        private:
        CellState _board[9][9]; //The board of the game
    };

    //Class for Eight Puzzle Game
    class EightPuzzle : public BoardGame2D
    {
        public:
        //Class to represent a cell
        class Cell{
            public:
            //Constructors
            Cell(int row = 0, int column = 0) : _row(row), _column(column) {}

            //Setters
            void setRow(int row) { _row = row; } 
            void setColumn(int column) { _column = column; } 

            //Getters
            int getRow() const { return _row; } 
            int getColumn() const { return _column; } 

            private:
            int _row, _column;  //The coordinates of the cell
        };

        //Class for storing an action in the table
        class Action{
            public:

            //Enum to represent posible directions of a peg in the board
            enum class Direction : int {
                UP = 0,
                DOWN = 1,
                LEFT = 2,
                RIGHT = 3 
            };


            //Constructors

            Action (const Direction newDir, const unsigned int Target) : _Target(Target), _direction(newDir) {}
            Action () : _Target(0),_direction(Direction::UP) {}  
            Action (const Action & newAction) : _Target(newAction.getTarget()), _direction(newAction.getDirection()) {} 


            //Setters

            bool setAction (const Direction, const unsigned int Target);   
            bool setAction(const string &);
            bool setTarget(const int);
            bool setDirection (const Direction);  
            bool setDirection (const char);   


            //Getters

            int getTarget() const { return _Target; }
            Direction getDirection () const { return _direction; }            
            const int * getDirection2D () const { return _directionList[(int)_direction]; } //Get direction as a vector in 2D


            private:
            int _Target;    //Label of the target from 1 to 8
            Direction _direction;
            static int _directionList[4][2];    //List of the possible directions in terms of 2D vectors
        };


        //Constructors

        EightPuzzle () { initialize(); shuffle(); }


        //Setters

        void setCellState(const unsigned int row, const unsigned int column, const unsigned int newCellState) { _board[row][column] = newCellState; } 


        //Getters

        int getCellState(unsigned int row, unsigned int column) const { return _board[row][column]; } 


        //Other functions

        bool playUser(const string) override;  //Take actions from user and perform them 
        void playAuto();    //play a computer generated action  

        void print() const;    //Print the board to an output stream

        bool endGame() const; //Return true if the game is finished   
        int boardScore() const; //return the score of the game, the lower the number the better the score is. (best score is 0)   

        void initialize(); //initialize the board. It might result in the same board, or a randomly generated one   

        bool checkAction(Action) const; //Check if the given action is valid
        bool moveCell(const Action); //Perform the given action on the board

        void shuffle(); //Shuffle the board

        private:
        int _board[3][3];
    };

    //Class for Klotski Game
    class Klotski : public BoardGame2D
    {
        public:
        //Enum to represent the types of blocks in the game
        enum class Block_t : int{
            Block_2x1,
            Block_1x2,
            Block_2x2,
            Block_1x1,
            empty = -1,
            Goal = -2,
            Wall = -3
        };

        //Enum to represent posible directions of a peg in the board
        enum class Direction : int {
            UP = 0,
            DOWN = 1,
            LEFT = 2,
            RIGHT = 3
        };

        //Class to represent a block
        class Block{
            public:
            //Constructors

            Block() : _type(Block_t::Block_1x1) {}
            Block(const Block_t type, const int label) : _type(type), _label(label) {}
            Block(const Block & newBlock) : _type(newBlock.getType()), _label(newBlock.getLabel()) {}


            //Setters

            void setBlock(const Block_t type, const int label) { _type = type; _label = label;}
            void setBlock(const Block target) { _type = target.getType(); _label = target.getLabel(); }
            bool setType(const Block_t);
            bool setLabel(const int);
            

            //Getters

            Block_t getType() const { return _type; }
            int getLabel() const { return _label; }
            int * getExtraBlocks() const { return _extraBlockList[(int)_type]; }    //Get the direction where the other part of the block is
            int * getExtraBlocks(int index) const { return _extraBlockList[index]; }    //Get another part of the block in the specified direction

    
            private:
            Block_t _type;  //The type of the block
            int _label; //Label of the block
            static int _extraBlockList[4][2];   //The directions where multiple parts of a block are relative to the base part
        };

        //Class for storing an action
        class Action {
            public:

            //Constructors

            Action (const Direction newDir, const int Target) : _Target(Target), _direction(newDir) {}  
            Action () : _Target(0),_direction(Direction::UP) {}  
            Action (const Action & newAction) : _Target(newAction.getTarget()), _direction(newAction.getDirection()) {} 


            //Setters

            bool setAction (const Direction, const int);   
            bool setAction (const string &);  
            bool setDirection (const Direction);  
            bool setDirection (const char);   //Set the direction with a character
            bool setTarget (const int);   


            //Getters

            const int * getDirection2D () const { return _directionList[(int)_direction]; }    //Get direction as a vector in 2D
            Direction getDirection () const { return _direction; }    
            int getTarget () const { return _Target; }    


            private:
            int _Target; //Label of the target from 1 to 10
            Direction _direction;
            static int _directionList[4][2]; //List of the possible directions in terms of 2D vectors
        };

        //Constructors

        Klotski() { initialize(); }


        //Setters and Getters

        Block const & getBlock(unsigned int row, unsigned int column) const { return _board[row][column]; } 
        void setBlock(unsigned int row, unsigned int column, const Block target) { _board[row][column].setBlock(target); }

        //Other functions
        bool playUser(const string) override;  //Take actions from user and perform them 
        void playAuto();    //play a computer generated action  

        void print() const;    //Print the board to an output stream 

        bool endGame() const; //Return true if the game is finished   
        int boardScore() const; //return the score of the game, the lower the number the better the score is. (best score is 0)   

        void initialize(); //initialize the board. It might result in the same board, or a randomly generated one   

        bool checkAction(Action) const; //Check if the given action is valid
        bool moveBlock(const Action); //Perform the given action on the board

        private:
        Block _board[6][4];
        Action _lastAction; //Stores the last action performed to prevent undoing an action too many times
    };
}


#endif