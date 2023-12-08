import java.util.Arrays;
import java.util.Collections;
import java.util.HashSet;
import java.util.Set;

public class Hand implements Comparable<Hand>{
    
    public Hand(Card[] cards, int bid) {
        super();
        this.cards = cards;
        this.bid = bid;
        this.computeType();
    }

    public Type getType(){
        return type;
    }
    public String getCardsStr(){
        String out = new String();
        for (Card c: cards){
            out += c.toString();
        }
        return out;
    }
    public int getBid(){
        return bid;
    }
    public static Hand fromString(String str){
        String[] split = str.split("\\s+");
        Card[] cards = new Card[5];
        for (int i =0; i< split[0].length(); i++){
            cards[i] = Card.New(split[0].charAt(i));
        }
        return new Hand(cards, Integer.parseInt(split[1]));
    }
    @Override
    public String toString() {
        return this.getCardsStr() + " " + this.bid;
    }
    static public enum Type{
        HIGH, ONE_PAIR, TWO_PAIRS, THREEOAK, FULL_HOUSE, FOUROAK, FIVEOAK
    }
    protected Card[] cards;
    protected Type type;
    protected int bid;

    protected void computeType(){
        Set<Card> cardsSet = new HashSet<>(Arrays.asList(cards));
        if (cardsSet.size() == 1){
            type = Type.FIVEOAK;
        }else if (cardsSet.size() == 2){
            int aCardFreq = Collections.frequency(Arrays.asList(cards), cardsSet.iterator().next());
            if (aCardFreq == 1 || aCardFreq == 4){
                type = Type.FOUROAK;
            }else{
                type = Type.FULL_HOUSE;
            }
        }else if (cardsSet.size() == 3){
            int maxCardFreq = 0;
            for (Card cardT: cardsSet){
                maxCardFreq = Math.max(maxCardFreq, Collections.frequency(Arrays.asList(cards), cardT));
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
            int cardCompare = this.cards[i].compareTo(arg0.cards[i]);
            if (cardCompare != 0){
                return cardCompare;
            }
        }
        return 0;
    }
}