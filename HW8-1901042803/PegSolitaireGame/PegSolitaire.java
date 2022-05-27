package PegSolitaireGame;

import PegSolitaireGame.PegSolitaireExc.*;
import java.io.*;

/**
 * The main interface of the PegSolitaire game, has 
 * the Subclass PegSolitaireEngine.
 * 
 * @see
 * PegSolitaireExc
 * @see
 * PegSolitaireEngine
 * @see
 * PegSolitaireGUI
 */
public interface PegSolitaire {
    /**
     * The interface of the enum type that describes 
     * what states can a cell be in.
     */
    public interface CellState {
        public String toString();
    }

    /**
     * The interface of the enum type that describes in
     * what direction is an action targeted to.
     */
    public interface Direction {
        /**
         * The four possible directions in the board in 2D vector form. <P>
         * The directions are in the following order starting 
         * from index 0 to 3: Up, Down, Left and Right
         */
        public static final int[][] _directionList = 
                            {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

        /**
         * Return the vector form of the direction.
         * @return 
         * An array as the following {vertically, horizontally}
         */
        public int[] vectorForm();
    }

    /**
     * The interface of the class that holds the 
     * information about an action.
     */
    public interface Action {
        /**
         * Return the target coordinates of the action
         * @return 
         * An array as the following: {row, column}
         */
        public int[] target();

        /**
         * Return the direction of the action
         * @return 
         * An object of the enum type Direction
         */
        public Direction direction();

        /**
         * Set the target coordinates of the action
         * @param row
         * @param column
         * @throws OutofBoundsException
         */
        public void setTarget(int row, int column)
            throws OutofBoundsException;

        /**
         * Set the direction of the action
         * @param direction
         * @throws InvalidDirectionException
         * : If the given direction was not of a matching type
         */
        public void setDirection(Direction direction)
            throws InvalidDirectionException;

        public Object clone();

        public String toString();
    }

    /**
     * Set the state of the cell in the given coordinates
     * @param row
     * @param column
     * @param newState
     * @throws OutofBoundsException
     */
    public void setCellState(
        int row, int column, CellState newState)
        throws OutofBoundsException;

    /**
     * Get the state of the cell in the given coordinates
     * @param row
     * @param column
     * @return The cell state in the enum type CellState
     * @throws OutofBoundsException
     */
    public CellState getCellState(int row, int column)
        throws OutofBoundsException;

    /**
     * Perform a single random computer generated action 
     * on the board. It still records the last action.
     */
    public void playAuto();

    /**
     * Check if the game has ended
     * @return true if the game ended, false if not.
     */
    public boolean endGame();

    /**
     * Initialize the board with the given board type
     * @param boardType : The index of the board type
     * @throws InvalidBoardTypeException
     */
    public void initialize(int boardType)
        throws InvalidBoardTypeException;

    /**
     * Perform the given action on the board. Uses checkAction() 
     * to validate the action. Records the last performed action.
     * @param action
     * @throws InvalidActionException
     * If the given action was not valid
     */
    public void movePeg(Action action)
        throws InvalidActionException;

    /**
     * Check if the given action is valid
     * @param action
     * @return true if the action is valid, false if not.
     */
    public boolean checkAction(Action action);

    /**
     * Undo the last performed action, and make it 
     * null after that.
     * @throws NoActionsToUndoException
     */
    public void Undo() throws NoActionsToUndoException;

    /**
     * Save the current game to a file with the given name
     * @param fileName
     * @throws IOException
     */
    public void saveGame(String fileName)
        throws IOException;

    /**
     * Calculate and return the current score of the board
     * @return the score in integers
     */
    public int score();

    public Object clone();

    public String toString();

    /**
     * Get the dimensions of the board
     * @return An array as the following {rows, columns}
     */
    public int[] Dimensions();
}
