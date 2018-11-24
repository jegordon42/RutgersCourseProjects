
public class Player {
	//declare your fields here
	private double theBalance;
	private Hand theHand;
	
	//initialize your fields in the constructor
	public Player(double balance){
		theBalance = balance;
		theHand = new Hand();
	}

	public void deal(Card c){
		theHand.addCard(c);
	}

	   public int getRank(Hand h) 
	   {
		   	int rank;
	        if(h.hasStraight() && h.hasFlush())
	        		rank = 9;
	        else if(h.hasFourOfAKind())
	        		rank = 8;
	        else if(h.hasFullHouse())
	        		rank = 7;  
	        else if(h.hasFlush())
	        		rank = 6;
	        else if(h.hasStraight())
	        		rank = 5;
	        else if(h.hasTriplet())
	        		rank = 4;
	        else if(h.numPairs() == 2)
	        		rank = 3;
	        else if(h.numPairs() == 1)
	        		rank = 2;
	        else
	        		rank = 1;
	        
	        	return rank;
	    }
	   
	 public int getTriplet()
	 {
	        int numCardsSame = 1;
	        for(int i = 0; i < theHand.getCardCount(); i++)
	        {
	            for(int j = 0; j < theHand.getCardCount(); j++)
	            {
	                if(j != i)
	                {
	                    if(theHand.getCard(i).getValue() == theHand.getCard(j).getValue())
	                       numCardsSame++;
	                }
	            }
	            if(numCardsSame >= 3)
	                return theHand.getCard(i).getValue();
	            numCardsSame = 1;
	        }
	        return 0;
	    }
	   
	public int getPair(boolean getSecond)
	{
		int[] pairs = new int[2];
        int pairIndex = 0;
        for(int i = 0; i < 5; i++)
        {
            for(int j = 1; j < 5; j++)
            {
                if(j != i)
                {
                    if(theHand.getCard(i).getValue() == theHand.getCard(j).getValue())
                    {
                        if(!(theHand.getCard(i).getValue() == pairs[0] || theHand.getCard(i).getValue() == pairs[1]))
                        {
                            pairs[pairIndex] = theHand.getCard(i).getValue();
                            pairIndex++;
                        }
                    } 
                }
            }
        }
        
        if(getSecond)
        		return pairs[1];
        else
        		return pairs[0];
	}
	
	public Card[] discard(){
		int rank = getRank(this.theHand);
		Card[] cardsToDiscard = new Card[0];
		int indexOfDiscard = 0;
		
		//if rank is greater than 4, don't discard you got a good hand :) .
		
		if(rank == 4) //if triplet, discard other two cards in hope of a full house or four of a kind
		{
			cardsToDiscard = new Card[2];
			int triplet = getTriplet();
			
			for(int i = 0; i < 5; i++)
			{
				if(theHand.getCard(i).getValue() != triplet)
				{
					cardsToDiscard[indexOfDiscard] = theHand.getCard(i);
					indexOfDiscard++;
					theHand.removeCard(i);
				}
			}
		}
		else if(rank == 3) //if two pair, discard the card that isnt paired.
		{
			cardsToDiscard = new Card[1];
			int firstPair = getPair(false);
			int secondPair = getPair(true);
			
			for(int i = 0; i < 5; i++)
			{
				if(theHand.getCard(i).getValue() != firstPair && theHand.getCard(i).getValue() != secondPair)
				{
					cardsToDiscard[indexOfDiscard] = theHand.getCard(i);
					indexOfDiscard++;
					theHand.removeCard(i);
				}
			}
		}
		else if(rank == 2) //if pair, discard the three unpaired cards
		{
			cardsToDiscard = new Card[3];
			int pair = getPair(false);
			
			for(int i = 0; i < 5; i++)
			{
				if(theHand.getCard(i).getValue() != pair)
				{
					cardsToDiscard[indexOfDiscard] = theHand.getCard(i);
					indexOfDiscard++;
					theHand.removeCard(i);
				}
			}
		}
		else if(rank == 1) //if nothing, keep highest card but discard rest
		{
			cardsToDiscard = new Card[4];
			int highest = theHand.highestValue().getValue();
			
			for(int i = 0; i < 5; i++)
			{
				if(theHand.getCard(i).getValue() != highest)
				{
					cardsToDiscard[indexOfDiscard] = theHand.getCard(i);
					indexOfDiscard++;
					theHand.removeCard(i);
				}
			}
		}
		
		return cardsToDiscard;
	}

	public double wager(double min){
		int rank = getRank(theHand);
		double wager = 0;
		
		switch(rank)
		{
			case 1: wager = 0;
			case 2: wager = theBalance * 0.01;break;
			case 3: wager = theBalance * 0.05;break;
			case 4: wager = theBalance * 0.1;break;
			case 5: wager = theBalance * 0.2;break;
			case 6: wager = theBalance * 0.3;break;
			case 7: wager = theBalance * 0.4;break;
			case 8: wager = theBalance * 0.5;break;
			case 9: wager = theBalance * 0.6;break;
		}
		
		if(wager < min)
		{
			if(rank > 6 && theBalance >= min)
			{
				theBalance -= min;
				return min;
			}
			return -1;
		}
		else 
		{
			theBalance -= wager;
			return wager;
		}
	}

	//Returns this player's hand
	public Hand showHand(){
		return theHand;
	}
	
	public void displayHand() {
		System.out.println("This is your hand:");
		for(int i = 0; i < 5; i++)
		{
			if(theHand.getCard(i) == null)
				System.out.println(i + ": Empty");
			else
				System.out.println(i + ": " + theHand.getCard(i).toString());
		}
	}
	//Returns this player's current balance
	public double getBalance(){
		return theBalance;
	}

	//Increase player's balance by the amount specified in the parameter,
	//then reset player's hand in preparation for next round. Amount will
	//be 0 if player has lost hand
	public void winnings(double amount){
		theBalance += amount;
	}

}