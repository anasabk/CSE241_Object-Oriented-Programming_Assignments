package PegSolitaireGame;

import java.io.*;
import java.util.Random;

import javax.swing.JOptionPane;

import PegSolitaireGame.PegSolitaireExc.*;

/**
 * The class that makes the engine of the PegSolitaire game.
 * It handles all the functions that directly 
 * affect the board.
 * 
 * @see
 * PegSolitaire
 * @see
 * PegSolitaireExc
 * @see
 * PegSolitaireGUI
 */
public class PegSolitaireEngine implements PegSolitaire {
    /**
     * The board of the PegSolitaire game
     */
    private CellState[][] _board;

    /**
     * The last performed action in the game. Initialized to null in all constructors.
     */
    private Action_Peg lastAction;

    /**
     * The list of the available built in board styles in the game.
     */
    public static final CellState[][][] boardStyles = 
          {{{CellState_Peg.Null, CellState_Peg.Null, CellState_Peg.peg,  CellState_Peg.peg, CellState_Peg.peg,CellState_Peg.Null, CellState_Peg.Null},
            {CellState_Peg.Null,  CellState_Peg.peg, CellState_Peg.peg,  CellState_Peg.peg, CellState_Peg.peg, CellState_Peg.peg, CellState_Peg.Null},
            { CellState_Peg.peg,  CellState_Peg.peg, CellState_Peg.peg,  CellState_Peg.peg, CellState_Peg.peg, CellState_Peg.peg,  CellState_Peg.peg},
            { CellState_Peg.peg,  CellState_Peg.peg, CellState_Peg.peg,CellState_Peg.empty, CellState_Peg.peg, CellState_Peg.peg,  CellState_Peg.peg},
            { CellState_Peg.peg,  CellState_Peg.peg, CellState_Peg.peg,  CellState_Peg.peg, CellState_Peg.peg, CellState_Peg.peg,  CellState_Peg.peg},
            {CellState_Peg.Null,  CellState_Peg.peg, CellState_Peg.peg,  CellState_Peg.peg, CellState_Peg.peg, CellState_Peg.peg, CellState_Peg.Null},
            {CellState_Peg.Null, CellState_Peg.Null, CellState_Peg.peg,  CellState_Peg.peg, CellState_Peg.peg,CellState_Peg.Null, CellState_Peg.Null}},

           {{CellState_Peg.Null, CellState_Peg.Null, CellState_Peg.Null, CellState_Peg.peg,   CellState_Peg.peg, CellState_Peg.peg, CellState_Peg.Null, CellState_Peg.Null, CellState_Peg.Null},
            {CellState_Peg.Null, CellState_Peg.Null, CellState_Peg.Null, CellState_Peg.peg,   CellState_Peg.peg, CellState_Peg.peg, CellState_Peg.Null, CellState_Peg.Null, CellState_Peg.Null},
            {CellState_Peg.Null, CellState_Peg.Null, CellState_Peg.Null, CellState_Peg.peg,   CellState_Peg.peg, CellState_Peg.peg, CellState_Peg.Null, CellState_Peg.Null, CellState_Peg.Null},
            { CellState_Peg.peg,  CellState_Peg.peg,  CellState_Peg.peg, CellState_Peg.peg,   CellState_Peg.peg, CellState_Peg.peg,  CellState_Peg.peg,  CellState_Peg.peg,  CellState_Peg.peg},
            { CellState_Peg.peg,  CellState_Peg.peg,  CellState_Peg.peg, CellState_Peg.peg, CellState_Peg.empty, CellState_Peg.peg,  CellState_Peg.peg,  CellState_Peg.peg,  CellState_Peg.peg},
            { CellState_Peg.peg,  CellState_Peg.peg,  CellState_Peg.peg, CellState_Peg.peg,   CellState_Peg.peg, CellState_Peg.peg,  CellState_Peg.peg,  CellState_Peg.peg,  CellState_Peg.peg},
            {CellState_Peg.Null, CellState_Peg.Null, CellState_Peg.Null, CellState_Peg.peg,   CellState_Peg.peg, CellState_Peg.peg, CellState_Peg.Null, CellState_Peg.Null, CellState_Peg.Null},
            {CellState_Peg.Null, CellState_Peg.Null, CellState_Peg.Null, CellState_Peg.peg,   CellState_Peg.peg, CellState_Peg.peg, CellState_Peg.Null, CellState_Peg.Null, CellState_Peg.Null},
            {CellState_Peg.Null, CellState_Peg.Null, CellState_Peg.Null, CellState_Peg.peg,   CellState_Peg.peg, CellState_Peg.peg, CellState_Peg.Null, CellState_Peg.Null, CellState_Peg.Null}},

           {{CellState_Peg.Null, CellState_Peg.Null, CellState_Peg.peg,   CellState_Peg.peg, CellState_Peg.peg, CellState_Peg.Null, CellState_Peg.Null, CellState_Peg.Null},
            {CellState_Peg.Null, CellState_Peg.Null, CellState_Peg.peg,   CellState_Peg.peg, CellState_Peg.peg, CellState_Peg.Null, CellState_Peg.Null, CellState_Peg.Null},
            {CellState_Peg.Null, CellState_Peg.Null, CellState_Peg.peg,   CellState_Peg.peg, CellState_Peg.peg, CellState_Peg.Null, CellState_Peg.Null, CellState_Peg.Null},
            { CellState_Peg.peg,  CellState_Peg.peg, CellState_Peg.peg,   CellState_Peg.peg, CellState_Peg.peg,  CellState_Peg.peg,  CellState_Peg.peg,  CellState_Peg.peg},
            { CellState_Peg.peg,  CellState_Peg.peg, CellState_Peg.peg, CellState_Peg.empty, CellState_Peg.peg,  CellState_Peg.peg,  CellState_Peg.peg,  CellState_Peg.peg},
            { CellState_Peg.peg,  CellState_Peg.peg, CellState_Peg.peg,   CellState_Peg.peg, CellState_Peg.peg,  CellState_Peg.peg,  CellState_Peg.peg,  CellState_Peg.peg},
            {CellState_Peg.Null, CellState_Peg.Null, CellState_Peg.peg,   CellState_Peg.peg, CellState_Peg.peg, CellState_Peg.Null, CellState_Peg.Null, CellState_Peg.Null},
            {CellState_Peg.Null, CellState_Peg.Null, CellState_Peg.peg,   CellState_Peg.peg, CellState_Peg.peg, CellState_Peg.Null, CellState_Peg.Null, CellState_Peg.Null}},

           {{CellState_Peg.Null, CellState_Peg.Null, CellState_Peg.peg,   CellState_Peg.peg, CellState_Peg.peg, CellState_Peg.Null, CellState_Peg.Null},
            {CellState_Peg.Null, CellState_Peg.Null, CellState_Peg.peg,   CellState_Peg.peg, CellState_Peg.peg, CellState_Peg.Null, CellState_Peg.Null},
            { CellState_Peg.peg,  CellState_Peg.peg, CellState_Peg.peg,   CellState_Peg.peg, CellState_Peg.peg,  CellState_Peg.peg,  CellState_Peg.peg},
            { CellState_Peg.peg,  CellState_Peg.peg, CellState_Peg.peg, CellState_Peg.empty, CellState_Peg.peg,  CellState_Peg.peg,  CellState_Peg.peg},
            { CellState_Peg.peg,  CellState_Peg.peg, CellState_Peg.peg,   CellState_Peg.peg, CellState_Peg.peg,  CellState_Peg.peg,  CellState_Peg.peg},
            {CellState_Peg.Null, CellState_Peg.Null, CellState_Peg.peg,   CellState_Peg.peg, CellState_Peg.peg, CellState_Peg.Null, CellState_Peg.Null},
            {CellState_Peg.Null, CellState_Peg.Null, CellState_Peg.peg,   CellState_Peg.peg, CellState_Peg.peg, CellState_Peg.Null, CellState_Peg.Null}},

           {{CellState_Peg.Null, CellState_Peg.Null, CellState_Peg.Null,CellState_Peg.Null,  CellState_Peg.peg,CellState_Peg.Null,CellState_Peg.Null, CellState_Peg.Null, CellState_Peg.Null},
            {CellState_Peg.Null, CellState_Peg.Null, CellState_Peg.Null, CellState_Peg.peg,  CellState_Peg.peg, CellState_Peg.peg,CellState_Peg.Null, CellState_Peg.Null, CellState_Peg.Null},
            {CellState_Peg.Null, CellState_Peg.Null,  CellState_Peg.peg, CellState_Peg.peg,  CellState_Peg.peg, CellState_Peg.peg, CellState_Peg.peg, CellState_Peg.Null, CellState_Peg.Null},
            {CellState_Peg.Null,  CellState_Peg.peg,  CellState_Peg.peg, CellState_Peg.peg,  CellState_Peg.peg, CellState_Peg.peg, CellState_Peg.peg,  CellState_Peg.peg, CellState_Peg.Null},
            { CellState_Peg.peg,  CellState_Peg.peg,  CellState_Peg.peg, CellState_Peg.peg,CellState_Peg.empty, CellState_Peg.peg, CellState_Peg.peg,  CellState_Peg.peg,  CellState_Peg.peg},
            {CellState_Peg.Null,  CellState_Peg.peg,  CellState_Peg.peg, CellState_Peg.peg,  CellState_Peg.peg, CellState_Peg.peg, CellState_Peg.peg,  CellState_Peg.peg, CellState_Peg.Null},
            {CellState_Peg.Null, CellState_Peg.Null,  CellState_Peg.peg, CellState_Peg.peg,  CellState_Peg.peg, CellState_Peg.peg, CellState_Peg.peg, CellState_Peg.Null, CellState_Peg.Null},
            {CellState_Peg.Null, CellState_Peg.Null, CellState_Peg.Null, CellState_Peg.peg,  CellState_Peg.peg, CellState_Peg.peg,CellState_Peg.Null, CellState_Peg.Null, CellState_Peg.Null},
            {CellState_Peg.Null, CellState_Peg.Null, CellState_Peg.Null,CellState_Peg.Null,  CellState_Peg.peg,CellState_Peg.Null,CellState_Peg.Null, CellState_Peg.Null, CellState_Peg.Null},}
        };

    /**
     * An enum type to represent the state of a cell 
     * in the board. <P>
     * Represents the following states: 
     * peg(a cell with a peg inside), 
     * empty(an empty cell), 
     * Null(an unuseable cell, used for crearting the style of the board). <P>
     * Each cell state has a unique character form.<P>
     * It's a subclass of CellState interface.
     * 
     * @see
     * CellState
     */
    public enum CellState_Peg implements CellState{
        peg('P'), empty('.'), Null(' ');

        /**
         * The character form
         */
        private char _value;

        /**
         * The constructor to assign the character values
         * @param c : character form
         */
        private CellState_Peg(char c){
            _value = c;
        }

        @Override
        public String toString() {
            return String.format("%c", _value);
        }
    }

    /**
     * An enum type to represent the direction of an action. <P>
     * Represents the following directions: Up, Down, Left and Right. <P>
     * Each direction has an integer value which is the index of it's vector
     * form in directionList from the interface.<P>
     * It's a subclass of Direction interface
     * 
     * @see
     * Direction
     */
    public static enum Direction_Peg implements Direction{
        Up(0), Down(1), Left(2), Right(3);

        /**
         * the vector form index
         */
        private final int _value;

        /**
         * The constructor to assign the index values
         * @param i : The index
         */
        private Direction_Peg(int i){
            _value = i;
        }

        @Override
        public int[] vectorForm(){
            return Direction._directionList[_value];
        }

        /**
         * A static function to give the enum form of a 
         * 2D vector direction <P>
         * Is a subclass of the interface Direction
         * @param direction : The direction in the form {vertically, horizontally}
         * @return The enum form in the Type Direction_Peg
         * @throws InvalidDirectionException
         */
        public static Direction_Peg getDirection(int[] direction)
            throws InvalidDirectionException
        {
            if (direction[0] == Direction_Peg.Up.vectorForm()[0]*2 &&
                direction[1] == Direction_Peg.Up.vectorForm()[1]*2)
                return Direction_Peg.Up;

            else if(direction[0] == Direction_Peg.Down.vectorForm()[0]*2 &&
                    direction[1] == Direction_Peg.Down.vectorForm()[1]*2)
                return Direction_Peg.Down;

            else if(direction[0] == Direction_Peg.Left.vectorForm()[0]*2 &&
                    direction[1] == Direction_Peg.Left.vectorForm()[1]*2)
                return Direction_Peg.Left;

            else if(direction[0] == Direction_Peg.Right.vectorForm()[0]*2 &&
                    direction[1] == Direction_Peg.Right.vectorForm()[1]*2)
                return Direction_Peg.Right;

            else throw new InvalidDirectionException();
        }
    
        @Override
        public String toString() {
            if(this == Up)
                return "Up";

            else if(this == Down)
                return "Down";

            else if(this == Left)
                return "Left";

            else if(this == Right)
                return "Right";

            else return super.toString();
        }
    }

    /**
     * An inner class that holds the information about an 
     * action and handles the operations on them.<P>
     * It's a subclass of Action interface.
     * 
     * @see
     * Action
     */
    public class Action_Peg implements Action{
        private int _row, _column;
        private Direction_Peg _direction;

        /**
         * Default constructor. Initializes the target 
         * to {0, 0} and the direction to Up.
         */
        public Action_Peg(){
            _row = 0;
            _column = 0;
            _direction = Direction_Peg.Up;
        }

        /**
         * A constructor that takes the coordinates of the 
         * target and the direction of the action.
         * @param row
         * @param column
         * @param direction
         * @throws OutofBoundsException
         */
        public Action_Peg(
            int row, int column, Direction_Peg direction)
            throws OutofBoundsException
        {
            /*Validate the coordinates*/
            if (row < 0 || row >= Dimensions()[0] ||
                column < 0 || column >= Dimensions()[1])
                throw new OutofBoundsException(row, column);

            else{
                _row = row;
                _column = column;
                _direction = direction;
            }
        }

        @Override
        public int[] target(){
            return new int[] {_row, _column};
        }

        @Override
        public Direction_Peg direction(){
            return _direction;
        }

        @Override
        public void setTarget(int row, int column)
            throws OutofBoundsException
        {
            /*Validate the coordinates*/
            if (row < 0 || row >= Dimensions()[0] ||
                column < 0 || column >= Dimensions()[1])
                throw new OutofBoundsException(row, column);

            else{
                _row = row;
                _column = column;
            }
        }
    
        @Override
        public void setDirection(Direction direction)
            throws InvalidDirectionException
        {
            if(direction.getClass().equals(Direction_Peg.class))
                _direction = (Direction_Peg)direction;

            else throw new InvalidDirectionException();
        }
    
        @Override
        public Object clone()
        {
            Action_Peg tempClone = null;

            try{
                tempClone = new  Action_Peg(_row, _column, _direction);
            }catch(OutofBoundsException exc) {}
            
            return tempClone;
        }
        
        @Override
        public String toString() {
            return String.format(
                "row: %d, Column: %d, Direction: %s", 
                _row, _column, _direction);
        }    
    }

    /**
     * Default constructor. Initialize the board to 
     * style 0, and the last action to null
     */
    public PegSolitaireEngine(){
        try {
            initialize(0);
        } catch (InvalidBoardTypeException exc) {}

        lastAction = null;
    }

    /**
     * A constructor that initializes the board with 
     * the given board style index.
     * @param boardType
     * @throws InvalidBoardTypeException
     */
    public PegSolitaireEngine(int boardType)
        throws InvalidBoardTypeException
    {
        try {
            initialize(boardType);
        } catch (InvalidBoardTypeException exc) {
            throw exc;
        }
        
        lastAction = null;
    }

    /**
     * A constructor that initializes the board of the 
     * game with the given board.
     * @param board : A premade board
     * @throws IndexOutOfBoundsException
     */
    public PegSolitaireEngine(CellState[][] board)
        throws IndexOutOfBoundsException
    {
        _board = new CellState[board.length][board[0].length];

        /*Copy the cells one by one*/
        for(int row = 0; row < Dimensions()[0];  ++row)
            for(int column = 0; column < Dimensions()[1]; ++column)
                _board[row][column] = board[row][column];

        lastAction = null;
    }

    /**
     * Get the game from the file with the given name and return it.
     * @param fileName : The file name with it's extension
     * @return A PegSolitaireEngine object
     * @throws InvalidSaveFileNameException
     * @throws IOException
     */
    public static PegSolitaireEngine loadGame(String fileName)
        throws InvalidSaveFileNameException, IOException
    {
        CellState_Peg[][] board = null;

        try {
            /*Open the file*/
            File target = new File(fileName);
            BufferedReader reader = 
                    new BufferedReader(new FileReader(target));
            
            String buffer = reader.readLine();

            /*Create the board */
            board = new CellState_Peg
                        [Character.getNumericValue(buffer.charAt(0))]
                        [Character.getNumericValue(buffer.charAt(3))];

            /**
             * Keep reading the lines and registering 
             * the cells till reaching file end
             */
            for(int row = 0; 
                row < board.length && (buffer = reader.readLine()) != null; 
                ++row)

                for(int column = 0; column < board[row].length; ++column){
                    if(buffer.charAt(column) == 'P')
                        board[row][column] = CellState_Peg.peg;

                    else if(buffer.charAt(column) == '.')
                        board[row][column] = CellState_Peg.empty;

                    else if(buffer.charAt(column) == ' ')
                        board[row][column] = CellState_Peg.Null;                    
                }

            /*Close the file*/
            reader.close();
        } 
        
        catch (FileNotFoundException fileNamException) {
            throw new InvalidSaveFileNameException(fileName);
        }
        catch (IOException ioException) {
            throw ioException;
        }

        /*Return the game*/
        return new PegSolitaireEngine(board);
    }

    @Override
    public void saveGame(String fileName)
        throws IOException
    {
        try {
            /*Open the file*/
            File target = new File(fileName);
            FileOutputStream output = new FileOutputStream(target);

            /*Write the data*/
            output.write (( Dimensions()[0] + ", " + 
                            Dimensions()[1] + "\n" +
                            toString())
                            .getBytes());

            /*Close the file*/
            output.close();
        } 
        catch (FileNotFoundException FileException) {}
        catch (IOException IOexception) {
            throw IOexception;
        }
    }

    @Override
    public void initialize(int boardType)
        throws InvalidBoardTypeException
    {
        /*Create the board*/
        _board = new CellState_Peg[boardStyles[boardType].length]
                                  [boardStyles[boardType][0].length];

        /*Copy each cell*/
        if (boardType < boardStyles.length &&
            boardType >= 0)
            for(int row = 0; row < Dimensions()[0];  ++row)
                for(int column = 0; column < Dimensions()[1]; ++column)
                    _board[row][column] = 
                            boardStyles[boardType][row][column];

        else throw new InvalidBoardTypeException(boardType);
    }

    @Override
    public void setCellState(
        int row, int column, CellState newState)
        throws OutofBoundsException
    {
        /*Validate the coordinates*/
        if(row < 0 || row >= Dimensions()[0] ||
           column < 0 || column >= Dimensions()[1])
           throw new OutofBoundsException(row, column);

        else _board[row][column] = (CellState_Peg)newState;
    }

    @Override
    public CellState getCellState(
        int row, int column) 
        throws OutofBoundsException
    {
        /*Validate the coordinates*/
        if(row < 0 || row >= Dimensions()[0] ||
           column < 0 || column >= Dimensions()[1])
           throw new OutofBoundsException(row, column);

        return _board[row][column];
    }

    @Override
    public boolean endGame(){
        Action_Peg tempAction = new Action_Peg();
        final Direction_Peg[] directionList =  {Direction_Peg.Up, 
                                                Direction_Peg.Down, 
                                                Direction_Peg.Left, 
                                                Direction_Peg.Right};

        try{
            /**
             * Scan each cell with a peg for valid possible actions
             */
            for (int row = 0 ; row < Dimensions()[0] ; ++row)
                for (int column = 0 ; column < Dimensions()[1] ; ++column)
                    if(_board[row][column] == CellState_Peg.peg)
                    {
                        tempAction.setTarget(row, column);

                        for (var tempDir : directionList)
                        {
                            tempAction.setDirection(tempDir);

                            if (checkAction(tempAction)) return false;
                        }
                    }

        }catch(Exception exc) {
            JOptionPane.showMessageDialog( 
                null, 
                "Something is wrong in endGame() in the engine.",
                "ERROR",
                JOptionPane.ERROR_MESSAGE );
        }

        lastAction = null;
        return true;    //No movable pegs found
    }

    @Override
    public int score(){
        int score = 0;

        /*Each cell with a peg increases the score by one*/
        for (int row = 0; row < Dimensions()[0]; ++row)
            for (int column = 0; column < Dimensions()[1]; ++column)
                if (_board[row][column] == CellState_Peg.peg) ++score;

        return score;
    }

    @Override
    public boolean checkAction(Action action) {
        int row = action.target()[0],
            column = action.target()[1];

        int[] direction = action.direction().vectorForm();

        /**
         * Check if the destination is inside the board, 
         * then see if the states of the affected cells are valid
         */
        if (row + direction[0] < 0 || row + direction[0] >= Dimensions()[0] ||
            column + direction[1] < 0 || column + direction[1] >= Dimensions()[1] ||
            row + direction[0]*2 < 0 || row + direction[0]*2 >= Dimensions()[0] ||
            column + direction[1]*2 < 0 || column + direction[1]*2 >= Dimensions()[1] ||
             
            _board[row][column] != CellState_Peg.peg ||
            _board[row + direction[0]][column + direction[1]] != CellState_Peg.peg ||
            _board[row + direction[0]*2][column + direction[1]*2] != CellState_Peg.empty)

            return false;

        else return true;
    }

    @Override
    public void movePeg(Action action)
        throws InvalidActionException
    {
        int row = action.target()[0],
            column = action.target()[1];

        int[] direction = action.direction().vectorForm();

        try{
            if (checkAction(action))
            {
                setCellState(row, column, CellState_Peg.empty);

                setCellState(row + direction[0], 
                             column + direction[1], 
                             CellState_Peg.empty);

                setCellState(row + direction[0]*2, 
                             column + direction[1]*2, 
                             CellState_Peg.peg);

                lastAction = (Action_Peg)action;
            }

            else throw new InvalidActionException();
        }catch(OutofBoundsException BoundsExc){}
        catch(InvalidActionException actionException){
            throw actionException;
        }
    }

    @Override
    public void Undo()
        throws NoActionsToUndoException
    {
        if (lastAction == null)
            throw new NoActionsToUndoException();

        else{
            try{
                int row = lastAction.target()[0],
                    column = lastAction.target()[1];

                int[] direction = lastAction.direction().vectorForm();

                setCellState(row, column, CellState_Peg.peg);

                setCellState(row + direction[0], 
                             column + direction[1], 
                             CellState_Peg.peg);

                setCellState(row + direction[0]*2, 
                             column + direction[1]*2, 
                             CellState_Peg.empty);

                lastAction = null;
            }catch (OutofBoundsException exc) {}
        }
    }

    @Override
    public Object clone(){
        CellState[][] tempBoard = 
            new CellState_Peg[Dimensions()[0]][Dimensions()[1]];

        for(int row = 0; row < Dimensions()[0];  ++row)
            for(int column = 0; column < Dimensions()[1]; ++column)
                tempBoard[row][column] = _board[row][column];

        return new PegSolitaireEngine(tempBoard);
    }

    @Override
    public String toString() {
        String result = new String();

        for (int row = 0; row < Dimensions()[0]; ++row)
        {
            for (var cell : _board[row])
                result += cell.toString();

            result += "\n";
        }

        return result;
    }

    @Override
    public int[] Dimensions(){
        return new int[] {_board.length, _board[0].length};
    }

    @Override
    public void playAuto() {
        Action_Peg tempAction = new Action_Peg();
        Direction[] directionList = {Direction_Peg.Up, 
                                     Direction_Peg.Down, 
                                     Direction_Peg.Left, 
                                     Direction_Peg.Right};

        Random randNum = new Random();

        try {
            /**
             * Go through each cell, if a cell has a peg, 
             * each valid direction has a chance of 20% to be performed.
             * if the board's end is reached and no actions where performed, 
             * rescan the board again in case the game hasn't ended
             */
            while(!(endGame()))
                for (int row = 0; row < Dimensions()[0]; ++row)
                    for (int column = 0; column < Dimensions()[1]; ++column)
                        if(_board[row][column].equals(CellState_Peg.peg))
                        {
                            tempAction.setTarget(row, column);

                            for (var tempDir : directionList){
                                tempAction.setDirection(tempDir);

                                if(checkAction(tempAction) && (randNum.nextInt(5) == 1)){
                                    movePeg(tempAction);
                                    lastAction = tempAction;
                                    return; /*Exit the function if an action was performed */
                                }
                            }
                        }

        } catch (Exception e) {
            JOptionPane.showMessageDialog( 
                null, 
                "Something is wrong in playAuto() in the engine.",
                "ERROR",
                JOptionPane.ERROR_MESSAGE );
        }
    }
}
