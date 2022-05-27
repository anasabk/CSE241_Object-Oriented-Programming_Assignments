package PegSolitaireGame;

import PegSolitaireGame.PegSolitaireEngine.*;
import PegSolitaireGame.PegSolitaireExc.*;

import java.awt.*;
import java.awt.event.*;
import java.io.IOException;
import java.util.concurrent.TimeUnit;

import javax.swing.*;
import javax.swing.GroupLayout.*;


/**
 * This class handles the Graphical User interface of the game
 */
public class PegSolitaireGUI extends JFrame{
    /**
     * The engine of the game
     */
    private PegSolitaireEngine _game;

    /**
     * The target of an action
     */
    private int[] target;

    private JButton newGameButton = new JButton("New Game"),
                    loadGameButton = new JButton("Load Game"),
                    saveGameButton = new JButton("Save Game"),
                    exitButton = new JButton("Exit"),
                    resetButton = new JButton("Reset"),
                    nextButton = new JButton("Next Action"),
                    undoButton = new JButton("Undo");

    private JToggleButton[][] boardButtons,
                              defaultBoard = new JToggleButton[7][7];

    private JPanel currentPanel, boardPanel, defaultPanel;
    private GroupLayout layout = new GroupLayout(getContentPane());
    private MainActionHandler mainActionListener = new MainActionHandler();
    private CellActionListener cellActionListener = new CellActionListener();
    private NewGameSettingsFrame newGameFrame = new NewGameSettingsFrame();


    /**
     * Constructs the main interface
     */
    public PegSolitaireGUI(){
        /*grid of the default board*/
        GridLayout defaultBoardGrid = new GridLayout(7, 7, 5, 5);

        /*The board panel used when no game is active*/
        defaultPanel = new JPanel(defaultBoardGrid);

        /*Add the action listeners*/
        newGameButton.addActionListener( mainActionListener );
        loadGameButton.addActionListener( mainActionListener );
        saveGameButton.addActionListener( mainActionListener );
        exitButton.addActionListener( mainActionListener );
        resetButton.addActionListener( mainActionListener );
        undoButton.addActionListener( mainActionListener );
        nextButton.addActionListener( mainActionListener );

        /*Initially disable unnecessary buttons*/
        saveGameButton.setEnabled(false);
        resetButton.setEnabled(false);
        undoButton.setEnabled(false);
        nextButton.setEnabled(false);

        /*Construct the default board buttons*/
        for(int i = 0; i < 7 ; ++i)
            for(int j = 0; j < 7 ; ++j){
                defaultBoard[i][j] = new JToggleButton(" ");
                defaultBoard[i][j].setEnabled(false);
                defaultBoard[i][j].setMinimumSize(new Dimension(35, 35));
                defaultBoard[i][j].setFont(new Font("Tahoma", Font.BOLD, 18));
                defaultBoard[i][j].setMargin(new Insets(0,0,0,0));
                defaultPanel.add(defaultBoard[i][j], Alignment.CENTER);
            }

        currentPanel = defaultPanel;
        createLayout(currentPanel);
        setTitle("PegSolitaire");
        pack();
        setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
    }

    /**
     * Create the panel of the board from the game
     */
    public void setBoardPanel(){
        GridLayout buttonGrid = 
            new GridLayout (_game.Dimensions()[0], 
                            _game.Dimensions()[1], 
                            5, 
                            5);

        boardButtons = new JToggleButton[_game.Dimensions()[0]]
                                     [_game.Dimensions()[1]];

        boardPanel = new JPanel(buttonGrid);

        for(int row = 0; row < _game.Dimensions()[0]; ++row)
            for(int column = 0; column < _game.Dimensions()[1]; ++column){
                try {
                    if(_game.getCellState(row, column).toString().equals("."))
                        boardButtons[row][column] = new JToggleButton(" ");

                    else{
                        boardButtons[row][column] = 
                            new JToggleButton(_game.getCellState(row, column).toString());

                        if(_game.getCellState(row, column).toString().equals(" ")) 
                            boardButtons[row][column].setEnabled(false);
                    }

                } catch (Exception e) {}

                boardButtons[row][column].addActionListener( cellActionListener );

                boardButtons[row][column].setMinimumSize(
                            defaultBoard[0][0].getMinimumSize());

                boardButtons[row][column].setPreferredSize(
                            defaultBoard[0][0].getPreferredSize());

                boardButtons[row][column].setFont(
                            new Font("Tahoma", Font.BOLD, 12));

                boardButtons[row][column].setMargin(new Insets(0,0,0,0));
                boardPanel.add(boardButtons[row][column], Alignment.CENTER);
            }
    }

    /**
     * build the layout of the main screen using Group Layout
     * @param
     * boardPanel : The panel that will contain the game board
     */
    public void createLayout(JPanel boardPanel){
        getContentPane().setLayout(layout);
        layout.setAutoCreateGaps(true);
        layout.setAutoCreateContainerGaps(true);

        layout.setHorizontalGroup(layout.createSequentialGroup()
            .addGroup(layout.createParallelGroup(Alignment.LEADING)
                .addComponent(newGameButton)
                .addComponent(loadGameButton)
                .addComponent(saveGameButton)
                .addComponent(exitButton)
                .addGap(21)
                .addComponent(resetButton)
                .addComponent(undoButton)
                .addComponent(nextButton))
            .addGap(20)
            .addComponent(boardPanel)
        );

        layout.linkSize(SwingConstants.HORIZONTAL, 
                        newGameButton, 
                        loadGameButton,
                        saveGameButton, 
                        exitButton, 
                        resetButton,
                        undoButton,
                        nextButton);

        layout.setVerticalGroup(layout.createParallelGroup(Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addComponent(newGameButton)
                .addComponent(loadGameButton)
                .addComponent(saveGameButton)
                .addComponent(exitButton)
                .addGap(21)
                .addComponent(resetButton)
                .addComponent(undoButton)
                .addComponent(nextButton))
            .addGap(20)
            .addComponent(boardPanel)
        );
    }

    /**
     * Finish the current session.<P>
     * Show a message dialog and disable the board
     */
    public void endGame(){
        JOptionPane.showMessageDialog( 
                null, 
                "Congratulations!!! You Won\nScore: " + _game.score(),
                "Game Ended", 
                JOptionPane.PLAIN_MESSAGE );

        for(int row = 0; row < boardButtons.length; ++row)
            for(int column = 0; column < boardButtons[row].length; ++column)
                boardButtons[row][column].setEnabled(false);

        undoButton.setEnabled(false);
        nextButton.setEnabled(false);
    }

    /**
     * Action handler of the main interface
     */
    private class MainActionHandler implements ActionListener{
        private FileNamePane fileNamePane;

        public void actionPerformed(ActionEvent event) {
            if(event.getSource() == newGameButton){
                newGameFrame.setVisible(true);
                newGameFrame.pack();
                target = null;
            }

            else if(event.getSource() == loadGameButton) {
                /*Build the custom input pane to get the file name*/
                fileNamePane = new FileNamePane(new ActionListener(){
                    @Override
                    public void actionPerformed(ActionEvent event) {
                        if(event.getSource().equals(fileNamePane.okButton)){
                            String fileName = null;

                            try {
                                fileName = fileNamePane.fileNameField.getText() + ".PSG";
                                _game = PegSolitaireEngine.loadGame(fileName);
                                
                                setBoardPanel();
                                layout.replace(currentPanel, boardPanel);
                                currentPanel = boardPanel;

                                PegSolitaireGUI.this.pack();
                                saveGameButton.setEnabled(true);
                                resetButton.setEnabled(true);
                                undoButton.setEnabled(true);
                                nextButton.setEnabled(true);

                                fileNamePane.setVisible(false);
                            }
                            
                            catch (NullPointerException noTextException) {
                                return;
                            }

                            catch (InvalidSaveFileNameException fileNameException) {
                                JOptionPane.showMessageDialog( 
                                    null, 
                                    fileNameException.getMessage(),
                                    fileNameException.getTitle(),
                                    JOptionPane.ERROR_MESSAGE );
                            }
                            
                            catch (IOException ioException){
                                JOptionPane.showMessageDialog( 
                                    null, 
                                    "Could not get data from the file: " + fileName,
                                    "IO Error",
                                    JOptionPane.ERROR_MESSAGE );
                            }
                        }

                        else if(event.getSource().equals(fileNamePane.cancelButton)){
                            fileNamePane.setVisible(false);
                        }
                    }
                });

                fileNamePane.setVisible(true);
            }

            else if(event.getSource() == saveGameButton) {
                /*Build the custom input pane to get the file name*/
                fileNamePane = new FileNamePane(new ActionListener(){
                    @Override
                    public void actionPerformed(ActionEvent event) {
                        if(event.getSource().equals(fileNamePane.okButton)){
                            String fileName = null;

                            try {
                                fileName = fileNamePane.fileNameField.getText() + ".PSG";
                                _game .saveGame(fileName);
                                fileNamePane.setVisible(false);
                            }
                            
                            catch (NullPointerException noTextException) {
                                return;
                            }

                            catch (IOException ioException){
                                JOptionPane.showMessageDialog( 
                                    null, 
                                    "Could not get data from the file: " + fileName,
                                    "IO Error",
                                    JOptionPane.ERROR_MESSAGE );
                            }
                        }

                        else if(event.getSource().equals(fileNamePane.cancelButton)){
                            fileNamePane.setVisible(false);
                        }
                    }
                });

                fileNamePane.setVisible(true);
            }

            else if(event.getSource() == resetButton) {
                /*Reset the interface*/
                _game = null;
                layout.replace(currentPanel, defaultPanel);
                currentPanel = defaultPanel;
                target = null;
                
                saveGameButton.setEnabled(false);
                resetButton.setEnabled(false);
                undoButton.setEnabled(false);
                nextButton.setEnabled(false);
            }

            else if(event.getSource() == undoButton) {
                try {
                    _game.Undo();

                    setBoardPanel();
                    layout.replace(currentPanel, boardPanel);
                    currentPanel = boardPanel;
                    target = null;

                } catch (NoActionsToUndoException UndoException) {
                    JOptionPane.showMessageDialog( 
                        null, 
                        UndoException.getMessage(),
                        UndoException.getTitle(),
                        JOptionPane.ERROR_MESSAGE );
                }
            }

            else if(event.getSource() == exitButton) {
                dispose();
                System.exit(0);
            }

            else if(event.getSource() == nextButton){
                _game.playAuto();
                setBoardPanel();
                layout.replace(currentPanel, boardPanel);
                currentPanel = boardPanel;

                if(_game.endGame())
                    endGame();
            }
        }
    }

    /**
     * The custom input pane that takes file name for file IO
     */
    private class FileNamePane extends JFrame{
        private GroupLayout localLayout = new GroupLayout(getContentPane());
        private JTextField fileNameField = new JTextField();
        private JLabel text1 = new JLabel("Please enter the name of the file:");
        private JLabel text2 = new JLabel("(With no extensions)");
        private JButton okButton = new JButton("OK");
        private JButton cancelButton = new JButton("Cancel");

        /*Build the pane Using GroupLayout*/
        public FileNamePane(ActionListener actionListener){
            localLayout.setHorizontalGroup(localLayout.createParallelGroup(Alignment.LEADING)
                .addGroup(localLayout.createSequentialGroup()
                    .addGap(10)
                    .addGroup(localLayout.createParallelGroup(Alignment.LEADING)
                        .addComponent(text1)    
                        .addComponent(text2))
                    .addGap(10))
                .addGroup(localLayout.createSequentialGroup()
                    .addGap(10)
                    .addComponent(fileNameField)
                    .addGap(10))
                .addGroup(Alignment.CENTER, localLayout.createSequentialGroup()
                    .addComponent(cancelButton)
                    .addGap(10)
                    .addComponent(okButton))
            );

            localLayout.setVerticalGroup(localLayout.createSequentialGroup()
                .addGap(10)
                .addComponent(text1)
                .addComponent(text2)
                .addComponent(fileNameField)
                .addGap(10)
                .addGroup(localLayout.createParallelGroup(Alignment.CENTER)
                    .addComponent(cancelButton)
                    .addComponent(okButton))
                .addGap(10)
            );

            okButton.addActionListener(actionListener);
            cancelButton.addActionListener(actionListener);

            setLayout(localLayout);
            pack();
            setResizable(false);
            setVisible(false);
        }
    }

    /**
     * Action listener for cell buttons in the board panel
     */
    private class CellActionListener implements ActionListener{
        @Override
        public void actionPerformed(ActionEvent event) {
            if(event.getSource().getClass() == JToggleButton.class) 
                /**
                 * Find the target first
                 */
                for(int row = 0; row < _game.Dimensions()[0]; ++row)
                    for(int column = 0; column < _game.Dimensions()[1]; ++column)
                        if ( event.getSource() == boardButtons[row][column] ){
                            if(target == null) target = new int[]{row, column};

                            else if(target[0] == row && target[1] == column)
                                target = null;

                            else try {
                                /*create the action*/
                                Direction_Peg direction =  
                                    Direction_Peg.getDirection(
                                        new int[] {row - target[0], column - target[1]});

                                Action_Peg action = 
                                    _game.new Action_Peg(target[0], 
                                                         target[1], 
                                                         direction);

                                /*Perform it*/
                                _game.movePeg(action);
        
                            } catch (InvalidActionException actionException) {
                                JOptionPane.showMessageDialog( 
                                    null, 
                                    actionException.getMessage(),
                                    actionException.getTitle(), 
                                    JOptionPane.ERROR_MESSAGE );
                            }
                            
                            catch (OutofBoundsException OOBException) { 
                                JOptionPane.showMessageDialog( 
                                    null, 
                                    OOBException.getMessage(),
                                    OOBException.getTitle(), 
                                    JOptionPane.ERROR_MESSAGE );
                            }

                            catch (InvalidDirectionException directionException) {
                                JOptionPane.showMessageDialog( 
                                    null, 
                                    directionException.getMessage(),
                                    directionException.getTitle(), 
                                    JOptionPane.ERROR_MESSAGE );
                            }
                            
                            finally {
                                /*Refresh the panel after doing the action */
                                setBoardPanel();
                                layout.replace(currentPanel, boardPanel);
                                currentPanel = boardPanel;
                                target = null;

                                if (_game.endGame()){
                                    endGame();
                                }
                            }
                        }
        }
    }

    /**
     * The panel that handles the new game settings
     */
    private class NewGameSettingsFrame extends JFrame{
        private JRadioButton french = new JRadioButton("French Style");
        private JRadioButton Wiegleb = new JRadioButton("J. C. Wiegleb Style");
        private JRadioButton Asymmetrical = new JRadioButton("Asymmetrical Style");
        private JRadioButton English = new JRadioButton("English Style");
        private JRadioButton Diamond = new JRadioButton("Diamond Style");
        private JRadioButton human = new JRadioButton("Human Player");
        private JRadioButton computer = new JRadioButton("Computer");
        private JButton createButton = new JButton("Create game");
        private JButton cancelButton = new JButton("Cancel");
        private gameStyleItemHandler ItemHandler = new gameStyleItemHandler();
        private newGameActionListener ActionListener = new newGameActionListener();
        private int boardStyle = 0;
        private int playerType = 0;


        /**
         * Build the panel
         */
        public NewGameSettingsFrame(){
            GroupLayout newGameLayout = new GroupLayout(getContentPane());
            JTabbedPane Cards = new JTabbedPane();

            JPanel BoardTypePanel = new JPanel();
            JPanel PlayerTypePanel = new JPanel();

            BoxLayout BoardTypeLayout = new BoxLayout(BoardTypePanel, BoxLayout.Y_AXIS);
            BoxLayout PlayerTypeLayout = new BoxLayout(PlayerTypePanel, BoxLayout.Y_AXIS);

            ButtonGroup PlayerTypeGroup = new ButtonGroup();
            ButtonGroup BoardTypeGroup = new ButtonGroup();

            BoardTypePanel.setLayout(BoardTypeLayout);
            BoardTypePanel.add(french);
            BoardTypePanel.add(Wiegleb);
            BoardTypePanel.add(Asymmetrical);
            BoardTypePanel.add(English);
            BoardTypePanel.add(Diamond);

            PlayerTypePanel.setLayout(PlayerTypeLayout);
            PlayerTypePanel.add(human);
            PlayerTypePanel.add(computer);

            Cards.addTab("Board Type", BoardTypePanel);
            Cards.addTab("Player Type", PlayerTypePanel);

            Cards.setMinimumSize(new Dimension(220, 150));

            newGameLayout.setHorizontalGroup(newGameLayout.createParallelGroup(Alignment.CENTER)
                .addComponent(Cards)
                .addGap(10)
                .addGroup(newGameLayout.createSequentialGroup()
                    .addGap(5)
                    .addComponent(cancelButton)
                    .addGap(5)
                    .addComponent(createButton)
                    .addGap(5))
                .addGap(5)
            );

            newGameLayout.setVerticalGroup(newGameLayout.createSequentialGroup()
                .addComponent(Cards)
                .addGap(10)
                .addGroup(newGameLayout.createParallelGroup(Alignment.CENTER)
                    .addGap(5)
                    .addComponent(cancelButton)
                    .addGap(5)
                    .addComponent(createButton)
                    .addGap(5))
                .addGap(5)
            );

            BoardTypeGroup.add(french);
            BoardTypeGroup.add(Wiegleb);
            BoardTypeGroup.add(Asymmetrical);
            BoardTypeGroup.add(English);
            BoardTypeGroup.add(Diamond);

            PlayerTypeGroup.add(human);
            PlayerTypeGroup.add(computer);
    
            french.addItemListener(ItemHandler);
            Wiegleb.addItemListener(ItemHandler);
            Asymmetrical.addItemListener(ItemHandler);
            English.addItemListener(ItemHandler);
            Diamond.addItemListener(ItemHandler);

            human.addItemListener(ItemHandler);
            computer.addItemListener(ItemHandler);

            createButton.addActionListener(ActionListener);
            cancelButton.addActionListener(ActionListener);

            setTitle("New Game");
            setLayout(newGameLayout);
            setVisible(false);
            pack();
        }

        /**
         * Item handler for JRadioButtons
         */
        private class gameStyleItemHandler implements ItemListener{
            @Override
            public void itemStateChanged(ItemEvent event) {
                if(event.getSource().equals(french)) 
                    boardStyle = 0;
                
                else if(event.getSource().equals(Wiegleb))
                    boardStyle = 1;
    
                else if(event.getSource().equals(Asymmetrical))
                    boardStyle = 2;
    
                else if(event.getSource().equals(English))
                    boardStyle = 3;
    
                else if(event.getSource().equals(Diamond))
                    boardStyle = 4;

                else if(event.getSource().equals(human))
                    playerType = 0;

                else if(event.getSource().equals(computer))
                    playerType = 1;
            }
        }

        /**
         * Action Hnadler for creating the game
         */
        private class newGameActionListener implements ActionListener{
            javax.swing.Timer timer;

            @Override
            public void actionPerformed(ActionEvent event) {
                if(event.getSource().equals(createButton))
                {
                    try {
                        /*Create the game*/
                        _game = new PegSolitaireEngine(boardStyle);

                        /*Refresh the board panel*/
                        setBoardPanel();
                        layout.replace(currentPanel, boardPanel);
                        currentPanel = boardPanel;

                        PegSolitaireGUI.this.pack();
                        setVisible(false);

                        saveGameButton.setEnabled(true);
                        resetButton.setEnabled(true);
                        undoButton.setEnabled(true);
                        nextButton.setEnabled(true);

                        /*Player is computer */
                        if(playerType == 1) {
                            timer = new Timer(0, new ActionListener(){
                                @Override
                                public void actionPerformed(ActionEvent e) {
                                    _game.playAuto();
                                    setBoardPanel();
                                    layout.replace(currentPanel, boardPanel);
                                    currentPanel = boardPanel;

                                    try { TimeUnit.MILLISECONDS.sleep(500); } 
                                    catch (Exception exc) {}

                                    if(!_game.endGame()) timer.restart();
                                    else{
                                        timer.stop();
                                        endGame();
                                    }
                                }
                            });

                            timer.setRepeats(true);
                            timer.setInitialDelay(0);
                            timer.start();
                        }
                    } 
                    catch (InvalidBoardTypeException BoardTypeException) {
                        JOptionPane.showMessageDialog( 
                            null, 
                            BoardTypeException.getMessage(),
                            BoardTypeException.getTitle(), 
                            JOptionPane.ERROR_MESSAGE );
                    }
                }

                else if(event.getSource().equals(cancelButton))
                    setVisible(false);
            }
        }
    }
}
