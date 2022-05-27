package PegSolitaireGame;

import PegSolitaireGame.PegSolitaire.Action;

/**
 * This class contains all the possible exceptions that 
 * can be produced by PegSoltaire class and it's subclasses.
 * 
 * @see
 * PegSolitaire
 * @see
 * PegSolitaireEngine
 * @see
 * PegSolitaireGUI
 */
public class PegSolitaireExc {

    /**
     * Thrown when coordinates that exceed 
     * the board's bounds are used.
     */
    public static class OutofBoundsException extends Exception{
        /**
         * Stores the title of a dialog, 
         * can be used when showing one.
         */
        private String _title = "Out Of Bounds";
        
        /**
         * Default constructor
         */
        public OutofBoundsException(){
            super("Out of bounds coordinates where detected.");
        }

        /**
         * A constructor that takes the invalid coordinates 
         * to include them in the message
         * @param row : The row value of the invalid coordinates
         * @param column : The column value of the invalid coordinates
         */
        public OutofBoundsException(int row, int column){
            super("The coordinates " + 
                  row + ", " + column + 
                  " are out of bounds.");
        }

        /**
         * Return the title string for a dialog pane
         */
        public String getTitle(){
            return _title;
        }
    }

    /**
     * Thrown when the start and the destination points of 
     * the action can not be represented by one of the four 
     * valid directions (Up, Down, Left, Right), which means 
     * that the destination point is two cells away from the 
     * start point in one of the four directions.
     */
    public static class InvalidDirectionException extends Exception{
        /**
         * Stores the title of a dialog, 
         * can be used when showing one.
         */
        private String _title = "invalid direction";

        /**
         * Default constructor
         */
        public InvalidDirectionException(){
            super("An invalid direction was entered.");
        }

        /**
         * Return the title string for a dialog pane
         */
        public String getTitle(){
            return _title;
        }
    }

    /**
     * Thrown when an action that doesn't meet the requirments 
     * of a valid action is used .
     */
    public static class InvalidActionException extends Exception{
        /**
         * Stores the title of a dialog, 
         * can be used when showing one.
         */
        private String _title = "Invalid Action";

        /**
         * Default constructor
         */
        public InvalidActionException(){
            super("An invalid action was detected.");
        }

        /**
         * A constructor that takes the invalid action as 
         * a parameter to include it in the message.
         * @param action : The invalid action
         */
        public InvalidActionException(Action action){
            super("The action: " + action + " is invalid.");
        }

        /**
         * Return the title string for a dialog pane.
         */
        public String getTitle(){
            return _title;
        }
    }

    /**
     * Thrown when an undo command is given but no previous 
     * action is recorded.
     */
    public static class NoActionsToUndoException extends Exception{
        /**
         * Stores the title of a dialog, 
         * can be used when showing one.
         */
        private String _title = "Undo Action Error";

        /**
         * Default constructor
         */
        public NoActionsToUndoException(){
            super("No actions to undo");
        }

        /**
         * Return the title string for a dialog pane
         */
        public String getTitle(){
            return _title;
        }
    }

    /**
     * Thrown when a board type index is given that is out of 
     * the bounds of the board types list.
     */
    public static class InvalidBoardTypeException extends Exception{
        /**
         * Stores the title of a dialog, 
         * can be used when showing one.
         */
        private String _title = "Invalid Board Type";

        /**
         * Default constructor
         */
        public InvalidBoardTypeException(){
            super("Invalid board type is entered.");
        }

        /**
         * A constructor that takes the invalid board type index 
         * as a parameter to include it in the message.
         * @param BoardType : The invalid board type index
         */
        public InvalidBoardTypeException(int BoardType){
            super("The board type " + BoardType + " is invalid.");
        }

        /**
         * Return the title string for a dialog pane
         */
        public String getTitle(){
            return _title;
        }
    }

    /**
     * Thrown when a invalid file name is given to 
     * load a game from it.
     */
    public static class InvalidSaveFileNameException extends Exception{
        /**
         * Stores the title of a dialog, 
         * can be used when showing one.
         */
        private String _title = "Invalid Save File";

        /**
         * Default constructor
         */
        public InvalidSaveFileNameException(){
            super("Invalid save file name.");
        }

        /**
         * A constructor that takes the invalid file name as a 
         * parameter to include it in the message.
         * @param FileName : The invalid file name
         */
        public InvalidSaveFileNameException(String FileName){
            super("The save file " + FileName + " is invalid.");
        }

        /**
         * Return the title string for a dialog pane
         */
        public String getTitle(){
            return _title;
        }
    }
}
