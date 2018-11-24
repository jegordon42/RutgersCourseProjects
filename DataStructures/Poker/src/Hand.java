/**
 * An object of type Hand represents a hand of cards.  The
 * cards belong to the class Card.  A hand is empty when it
 * is created, and any number of cards can be added to it.
 */

public class Hand {

   private Card[] hand;   // The cards in the hand.
   private int count; 
   
   /**
    * Create a hand that is initially empty.
    */
   public Hand() {
      hand = new Card[5];
      count = 0;
   }
   
   /**
    * Remove all cards from the hand, leaving it empty.
    */
   public void clear() {
      for(int i=0 ; i<hand.length; i++){ hand[i] = null; }
      count = 0;
   }
   
   /**
    * Add a card to the hand.  It is added at the end of the current hand.
    * @param c the non-null card to be added.
    * @throws NullPointerException if the parameter c is null.
    */
   public void addCard(Card c) {
      
      for(int i=0 ; i<hand.length; i++){ 
        if (hand[i] == null){
            hand[i] = c;
            count = count + 1;
            break;
        }
     }

      
   }
   
   /**
    * Remove a card from the hand, if present.
    * @param c the card to be removed.  If c is null or if the card is not in 
    * the hand, then nothing is done.
    */
   public void removeCard(Card c) {

    for(int i=0 ; i<hand.length; i++){ 
        if (hand[i].equals(c)){
            hand[i] = null;
            count = count-1;
        }
    }

   }
   
   /**
    * Remove the card in a specified position from the hand.
    * @param position the position of the card that is to be removed, where
    * positions are starting from zero.
    * @throws IllegalArgumentException if the position does not exist in
    * the hand, that is if the position is less than 0 or greater than
    * or equal to the number of cards in the hand.
    */
   public void removeCard(int position) {
      if (position < 0 || position >= hand.length)
         throw new IllegalArgumentException("Position does not exist in hand: "
               + position);
      hand[position] = null;
      count--;
   }
   /**
    * Returns the number of cards in the hand.
    */
   public int getCardCount() {
      return count;
   }
   
   /**
    * Gets the card in a specified position in the hand.  (Note that this card
    * is not removed from the hand!)
    * @param position the position of the card that is to be returned
    * @throws IllegalArgumentException if position does not exist in the hand
    */
   public Card getCard(int position) {
      if (position < 0 || position >= hand.length)
         throw new IllegalArgumentException("Position does not exist in hand: "
               + position);
       return hand[position];
   }
   
   /**
    * Sorts the cards in the hand so that cards of the same suit are
    * grouped together, and within a suit the cards are sorted by value.
    * Note that aces are considered to have the lowest value, 1.
    */
   public void sortBySuit() {
      int size = count;
      int nonnull = 0;
      int index = 0;
      
      Card[] newHand = new Card[5];
      while (size > 0) {
         if (hand[nonnull] == null) { nonnull = nonnull+1; continue;}
         int pos = nonnull;  // Position of minimal card.
         Card c = hand[nonnull];  // Minimal card.
         
         for (int i = nonnull+1; i < hand.length; i++) {
            Card c1 = hand[i];
            if (c1 != null){
                if ( c1.getSuit() < c.getSuit() ||
                        (c1.getSuit() == c.getSuit() && c1.getValue() < c.getValue()) ) {
                    pos = i;
                    c = c1;
                }
            }
         }
         hand[pos] = null;
         size = size - 1;
         newHand[index++] = c;
         nonnull = 0;
      }
      hand = newHand;
   }
   
   /**
    * Sorts the cards in the hand so that cards of the same value are
    * grouped together.  Cards with the same value are sorted by suit.
    * Note that aces are considered to have the lowest value, 1.
    */
   public void sortByValue() {
      int size = count;
      int nonnull = 0;
      int index = 0;
      
      Card[] newHand = new Card[5];
      while (size > 0) {
         if (hand[nonnull] == null) { nonnull = nonnull+1; continue;}
         int pos = nonnull;  // Position of minimal card.
         Card c = hand[nonnull];  // Minimal card.
         
         for (int i = nonnull+1; i < hand.length; i++) {
            
            Card c1 = hand[i];
            if (c1 != null){
                if ( c1.getValue() < c.getValue() ||
                        (c1.getValue() == c.getValue() && c1.getSuit() < c.getSuit()) ) {
                    pos = i;
                    c = c1;
                }
            }
         }
         hand[pos] = null;
         size = size - 1;
         newHand[index++] = c;
         nonnull = 0;
      }
      hand = newHand;
   }
   
   public void printHand(){
       
       for(int i=0; i<hand.length; i++){
           if (hand[i] != null){
               System.out.println(hand[i]);
           }
       }
       System.out.println();
   }
   
   private int getIndex(Card value) 
   {
       for(int i=0;i<5;i++) 
       {
           if(hand[i] != null && hand[i] == value)
           return i;
       }
       return 0;
   }
   
    //Returns the number of pairs this hand contains
    public int numPairs()
    {
        int[] pairs = new int[2];
        int pairIndex = 0;
        for(int i = 0; i < 5; i++)
        {
            for(int j = 1; j < 5; j++)
            {
                if(j != i && hand[i] != null && hand[j] != null)
                {
                    
                    if( hand[i].getValue() == hand[j].getValue())
                    {
                        if(!(hand[i].getValue() == pairs[0] || hand[i].getValue() == pairs[1]))
                        {
                            pairs[pairIndex] = hand[i].getValue();
                            pairIndex++;
                        }
                    } 
                }
            }
        }
        return pairIndex;
    }

    //Returns true if this hand has 3 cards that are of the same value
    public boolean hasTriplet()
    {
        int numCardsSame = 1;
        for(int i = 0; i < count; i++)
        {
            for(int j = 0; j < count; j++)
            {
                if(j != i)
                {
                    if(hand[i].getValue() == hand[j].getValue())
                       numCardsSame++;
                }
            }
            if(numCardsSame >= 3)
                return true;
            numCardsSame = 1;
        }
        return false;
    }

    public Card getTriplet()
    {
        int numCardsSame = 1;
        for(int i = 0; i < count; i++)
        {
            for(int j = 0; j < count; j++)
            {
                if(j != i)
                {
                    if(hand[i].getValue() == hand[j].getValue())
                       numCardsSame++;
                }
            }
            if(numCardsSame >= 3)
                return hand[i];
            numCardsSame = 1;
        }
        return null;
    }
    
    //Returns true if this hand has all cards that are of the same suit
    public boolean hasFlush()
    {
        
        for(int i = 1; i < count; i++)
        {
            if(hand[i - 1].getSuit() != hand[i].getSuit())
                return false;
        }
        return true;
    }

    //Returns true if this hand has 5 consecutive cards of any suit
    public boolean hasStraight()

    {

        this.sortByValue();
        int first = hand[0].getValue();
        int next = first+1;
        boolean hasStraight = true;
        if(first < 10) 
        {

            for(int i = 1; i<hand.length; i++) 
            {

                if (hand[i] != null && hand[i].getValue() != next) 
                {
                    hasStraight = false;
                }
                    next++;
            }

        }

        else 
        {
            hasStraight = false;
        }

        if(first == 1 && hand[1].getValue() == 10 && hand[2].getValue() == 11 && hand[3].getValue()==12 && hand[4].getValue() == 13) 
        {
            hasStraight = true;
        }

        return hasStraight;

    }

    
    //Returns true if this hand has a triplet and a pair of different //values
    public boolean hasFullHouse()
    {
        if(hasTriplet() && numPairs() == 2)
            return true;
        else
            return false;
    }
    
    //Returns true if this hand has 4 cards that are of the same value
    public boolean hasFourOfAKind()
    {
        int numCardsSame = 1;
        for(int i = 0; i < count; i++)
        {
            for(int j = 0; j < count; j++)
            {
                if(j != i)
                {
                    if(hand[i].getValue() == hand[j].getValue())
                       numCardsSame++;
                }
            }
            if(numCardsSame == 4)
                return true;
            numCardsSame = 1;
        }
        return false;
    }
    
    //Returns the card with the highest value in the hand. When there is
    //more than one highest value card, you may return any one of them
    public Card highestValue() 
    {
        if(count>0)
        {
            //System.out.println("count: " + count);
            sortByValue();
            if(count>0) 
            {
                if (hand[0].getValue() == 1)
                    return hand[0];
                return hand[count-1];
            }
        }
        return null;
    }
    
    public Card highestNotPairValue() 
    {
        int highestDuplicate = 0;
        int[] pairs = new int[2];
        Card[] cpairs = new Card[2];
        int pairIndex = 0;
        for(int i = 0; i < count; i++)
        {
            for(int j = 1; j < count; j++)
            {
                if(hand[i] != null && hand[j] != null && j != i)
                {
                    if(hand[i].getValue() == hand[j].getValue())
                    {
                        if(!(hand[i].getValue() == pairs[0] || hand[i].getValue() == pairs[1]))
                        {
                            pairs[pairIndex] = hand[i].getValue();
                            cpairs[pairIndex] = hand[i];
                            pairIndex++;
                        }
                    }
                }
            }
        }
        if(pairIndex == 0)
            highestDuplicate = 0;
        else if(pairIndex == 1)
            highestDuplicate = cpairs[0].getValue();
        else
        {
            
            if(pairs[0] > pairs[1] || pairs[0]==1)
                highestDuplicate =  cpairs[0].getValue();
            else
                highestDuplicate = cpairs[1].getValue();
        }

        if(count>0)
        {
            //System.out.println("count: " + count);
            sortByValue();
            for(int i = 4; i >=0; i--) 
            {
                if (hand[0].getValue() == 1 && highestDuplicate != 1)
                    return hand[0];
                if (hand[i].getValue()!= highestDuplicate)
                    return hand[i];
            }
        }
        return null;
    }
    //Returns the highest valued Card of any pair or triplet found, null if
    // none. When values are equal, you may return either
    public Card highestDuplicate()
    {
        if(numPairs() == 0)
            return null;
        int[] pairs = new int[2];
        Card[] cpairs = new Card[2];
        int pairIndex = 0;
        for(int i = 0; i < count; i++)
        {
            for(int j = 1; j < count; j++)
            {
                if(hand[i] != null && hand[j] != null && j != i)
                {
                    if(hand[i].getValue() == hand[j].getValue())
                    {
                        if(!(hand[i].getValue() == pairs[0] || hand[i].getValue() == pairs[1]))
                        {
                            pairs[pairIndex] = hand[i].getValue();
                            cpairs[pairIndex] = hand[i];
                            pairIndex++;
                        }
                    }
                }
            }
        }
        if(pairIndex == 1)
            return cpairs[0];
        else
        {
            
            if(pairs[0] > pairs[1] || pairs[0]==1)
                return cpairs[0];
            else
                return cpairs[1];
        }

    }

    //Returns -1 if the instance hand loses to the parameter hand, 0 if //the hands are equal, and +1 if the instance hand wins over the //parameter hand. Hint: you might want to use some of the methods //above
    public int compareTo(Hand h) 
    {
        Card[] instanceCards= new Card[5];
        Card[] parameterCards = new Card[5];
        Hand instanceHand = new Hand();
        Hand parameterHand = new Hand();
        
        for(int i = 0;i<5; i++)
        {
            Card instanceCard = new Card(this.hand[i].getValue(), this.hand[i].getSuit());
            instanceCards[i] = instanceCard;
            Card parameterCard = new Card(h.hand[i].getValue(), h.hand[i].getSuit());
            parameterCards[i] = parameterCard;
        }
        
        instanceHand.hand = instanceCards;
        instanceHand.count=5;
        parameterHand.hand = parameterCards;
        parameterHand.count = 5;
        
        int instanceRank = 0;
        int parameterRank = 0;

        if(hasStraight() && hasFlush())
            instanceRank = 9;
        else if(hasFourOfAKind())
            instanceRank = 8;
        else if(hasFullHouse())
            instanceRank = 7;  
        else if(hasFlush())
            instanceRank = 6;
        else if(hasStraight())
            instanceRank = 5;
        else if(hasTriplet())
            instanceRank = 4;
        else if(numPairs() == 2)
            instanceRank = 3;
        else if(numPairs() == 1)
            instanceRank = 2;
        else
            instanceRank = 1;

        if(h.hasStraight() && h.hasFlush())
            parameterRank = 9;
        else if(h.hasFourOfAKind())
            parameterRank = 8;
        else if(h.hasFullHouse())
            parameterRank = 7;  
        else if(h.hasFlush())
            parameterRank = 6;
        else if(h.hasStraight())
            parameterRank = 5;
        else if(h.hasTriplet())
            parameterRank = 4;
        else if(h.numPairs() == 2)
            parameterRank = 3;
        else if(h.numPairs() == 1)
            parameterRank = 2;
        else
            parameterRank = 1;

        if(instanceRank > parameterRank)
            return 1;
        else if(instanceRank < parameterRank)
            return -1;
        else
        {
            if(instanceRank != 7)
            {
                while(numPairs() > 0 && highestDuplicate().getValue() == h.highestDuplicate().getValue())
                {
                    removeCard(getIndex(highestDuplicate()));
                    h.removeCard(h.getIndex(h.highestDuplicate()));
                } 
                if(numPairs() > 0)
                {
                    if(highestDuplicate().getValue() == 1)
                        return 1;
                    if(h.highestDuplicate().getValue() == 1)
                        return -1;
                    if(highestDuplicate().getValue() > h.highestDuplicate().getValue() )
                        return 1;
                    else
                        return -1;
                }
                else
                {
                    instanceHand.sortByValue();
                    parameterHand.sortByValue();
                    int same = 0;
                    for(int i = 0; i < count; i++)
                    {
                        if(instanceHand.hand[i].getValue() == parameterHand.hand[i].getValue())
                        {
                            same++;
                        }
                    }
                    
                    if(same == 5) 
                        return 0;
                    Card zeroCard = new Card(2,1);
                    int counter = 5;

                    while(instanceHand.highestNotPairValue().getValue() == parameterHand.highestNotPairValue().getValue() && counter != 0)
                    {
                        int instanceIndex = instanceHand.getIndex(instanceHand.highestNotPairValue());
                        instanceHand.hand[instanceIndex] = zeroCard;
                        int parameterIndex = parameterHand.getIndex(parameterHand.highestNotPairValue());
                        parameterHand.hand[parameterIndex] = zeroCard;
                        counter--;
                    }
                    if(instanceRank == 6)
                    {
                        if(count == 0)
                            return 0;
                        if(instanceHand.highestValue().getValue() == 1)
                            return 1;
                        if(parameterHand.highestValue().getValue() == 1)
                            return -1;
                        if(instanceHand.highestValue().getValue() > parameterHand.highestValue().getValue() )
                            return 1;
                        if(instanceHand.highestValue().getValue() < parameterHand.highestValue().getValue())
                            return -1;
                    }
                    else
                    {
                        if(count == 0)
                            return 0;
                        if(instanceHand.highestNotPairValue().getValue() == 1)
                            return 1;
                        if(parameterHand.highestNotPairValue().getValue() == 1)
                            return -1;
                        if(instanceHand.highestNotPairValue().getValue() > parameterHand.highestNotPairValue().getValue() )
                            return 1;
                        if(instanceHand.highestNotPairValue().getValue() < parameterHand.highestNotPairValue().getValue())
                            return -1;
                    }
                }
            }
            else
            {
                if(getTriplet().getValue() > h.getTriplet().getValue())
                    return 1;
                else if(getTriplet().getValue() < h.getTriplet().getValue())
                    return -1;
            }
        }
        return 0;
        
    }
}