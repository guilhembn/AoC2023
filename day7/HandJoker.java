import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.HashSet;
import java.util.Set;

public class HandJoker extends Hand{
    
    public HandJoker(Card[] cards, int bid) {
        super(cards, bid);
    }

   
    public static HandJoker fromString(String str){
        String[] split = str.split("\\s+");
        Card[] cards = new Card[5];
        for (int i =0; i< split[0].length(); i++){
            cards[i] = Card.New(split[0].charAt(i));
        }
        return new HandJoker(cards, Integer.parseInt(split[1]));
    }

    @Override
    protected void computeType(){
        ArrayList<Card> cardsWithoutJoker = new ArrayList<>(Arrays.asList(cards));
        while(cardsWithoutJoker.contains(Card.J)){
            cardsWithoutJoker.remove(Card.J);
        }
        if (cardsWithoutJoker.size() == 0){
            type = Type.FIVEOAK;
            return;
        }
        Set<Card> cardsWithoutJSet = new HashSet<>(cardsWithoutJoker);
        Card maxCardType = Card.A;
        int max = 0;
        for (Card c: cardsWithoutJSet){
            int freq = Collections.frequency(cardsWithoutJoker, c);
            if (freq > max){
                maxCardType = c;
                max = freq;
            }
        }
        ArrayList<Card> cardsJoked = new ArrayList<>();
        for (Card c: this.cards){
            if (c == Card.J){
                cardsJoked.add(maxCardType);
            }else{
                cardsJoked.add(c);
            }
        }
        Set<Card> cardsSet = new HashSet<>(cardsJoked);
        if (cardsSet.size() == 1){
            type = Type.FIVEOAK;
        }else if (cardsSet.size() == 2){
            int aCardFreq = Collections.frequency(cardsJoked, cardsSet.iterator().next());
            if (aCardFreq == 1 || aCardFreq == 4){
                type = Type.FOUROAK;
            }else{
                type = Type.FULL_HOUSE;
            }
        }else if (cardsSet.size() == 3){
            int maxCardFreq = 0;
            for (Card cardT: cardsSet){
                maxCardFreq = Math.max(maxCardFreq, Collections.frequency(cardsJoked, cardT));
            }
            if (maxCardFreq == 2){
                type = Type.TWO_PAIRS;
            }else{
                type = Type.THREEOAK;
            }
        }else if (cardsSet.size() == 4){
            type = Type.ONE_PAIR;
        }else{
            type = Type.HIGH;
        }
    }

    @Override
    public int compareTo(Hand arg0) {
        int typeCompare = this.type.compareTo(arg0.type);
        if (typeCompare != 0){
            return typeCompare;
        }
        for (int i=0 ; i<cards.length; i++){
            if (this.cards[i] == Card.J && arg0.cards[i] != Card.J){
                return -1;
            }else if (this.cards[i] != Card.J && arg0.cards[i] == Card.J){
                return 1;
            }
            int cardCompare = this.cards[i].compareTo(arg0.cards[i]);
            if (cardCompare != 0){
                return cardCompare;
            }
        }
        return 0;
    }
}