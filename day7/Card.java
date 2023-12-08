import java.util.HashMap;
import java.util.Map;

public enum Card{
    TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, T, J, Q, K, A;

    public static Card New(char value){
        return strToCard.get(String.valueOf(value));
    }
    @Override
    public String toString() {
        for (String s: strToCard.keySet()){
            if (strToCard.get(s) == this){
                return s;
            }
        }
        return "";
    }

    private static final Map<String, Card> strToCard = new HashMap<>();
    static{
        strToCard.put("2", TWO);
        strToCard.put("3", THREE);
        strToCard.put("4", FOUR);
        strToCard.put("5", FIVE);
        strToCard.put("6", SIX);
        strToCard.put("7", SEVEN);
        strToCard.put("8", EIGHT);
        strToCard.put("9", NINE);
        strToCard.put("T", T);
        strToCard.put("J", J);
        strToCard.put("Q", Q);
        strToCard.put("K", K);
        strToCard.put("A", A);
    }
}