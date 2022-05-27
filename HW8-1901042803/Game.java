import PegSolitaireGame.*;

public class Game {
    public static void main(String[] arg){
        PegSolitaireGUI game = new PegSolitaireGUI();
        game.setVisible(true);

        // PegSolitaireEngine game = new PegSolitaireEngine();

        // while(!game.endGame()){
        //     game.playAuto();
        //     System.out.printf("%s\n", game);
        // }
    }
}
