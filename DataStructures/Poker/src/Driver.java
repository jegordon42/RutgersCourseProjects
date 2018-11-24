import java.io.IOException;

public class Driver {
	public static void main(String[] args)	throws IOException {
		Card card1 = new Card(2,1);
		Card card2 = new Card(4,1);
		Card card3 = new Card(10,1);
		Card card4 = new Card(3,2);
		Card card5 = new Card(6,1);
		Player thePlayer = new Player(10000);
		thePlayer.deal(card1);
		thePlayer.deal(card2);
		thePlayer.deal(card3);
		thePlayer.deal(card4);
		thePlayer.deal(card5);

		
		thePlayer.displayHand();
		System.out.println("Your hand has a rank of " + thePlayer.getRank(thePlayer.showHand()));
		
		double opponentBet = 5001;
		System.out.println();
		System.out.println("You have $" + thePlayer.getBalance());
		System.out.println("The opponent bet $" + opponentBet + ", so you bet $" + thePlayer.wager(opponentBet));
		System.out.println("Now you have $" + thePlayer.getBalance());
		System.out.println();
		
		System.out.println("Discarding..");
		thePlayer.discard();
		System.out.println("Discard Complete");
		thePlayer.displayHand();
	}
}