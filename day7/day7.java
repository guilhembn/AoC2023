import java.io.BufferedReader;
import java.io.FileReader;
import java.util.ArrayList;
import java.util.Collections;

public class day7 {
    public static void main(String args[]) {
        ArrayList<Hand> hands = new ArrayList<>();
        ArrayList<Hand> handsJoker = new ArrayList<>();
        try (BufferedReader br = new BufferedReader(new FileReader("/home/guilhem/Documents/AoC2023/day7/input.txt"))) {
            String line;
            while ((line = br.readLine()) != null) {
                hands.add(Hand.fromString(line));
                handsJoker.add(HandJoker.fromString(line));
            }
        }catch (Exception e){//Catch exception if any
            System.err.println("Error: " + e.getMessage());
        }
        System.out.println("Puzzle 1 solution: " + puzzle1(hands));
        System.out.println("Puzzle 2 solution: " + puzzle1(handsJoker));
        
    }
    public static int puzzle1(ArrayList<Hand> hands){
        Collections.sort(hands);
        System.out.println(hands);
        int sum = 0;
        for (int i =0; i < hands.size(); i++){
            sum += hands.get(i).getBid() * (i+1);
        }
        return sum;
    }
    
}