use regex::Regex;
use std::fs;

fn main() {
    let game_re = Regex::new(r"Game ([0-9]+)").unwrap();
    let red_re = Regex::new(r"([0-9]+) red").unwrap();
    let green_re = Regex::new(r"([0-9]+) green").unwrap();
    let blue_re = Regex::new(r"([0-9]+) blue").unwrap();
    let contents =
        fs::read_to_string("./input.txt").expect("Should have been able to read the file");
    let games = contents.trim().split("\n").collect::<Vec<&str>>();
    let mut sum = 0;
    let mut sum_power = 0;
    for g in games {
        let gn: Vec<&str> = g.split(":").collect();
        let game_number: i64 = game_re.captures(gn[0]).unwrap()[1]
            .parse()
            .expect("Cannot parse game number");
        let reveals = gn[1].split(";");
        let mut game_ok = true;
        let mut min_red = 0;
        let mut min_green = 0;
        let mut min_blue = 0;
        for reveal in reveals {
            let reds_m = red_re.captures(reveal);
            let greens_m = green_re.captures(reveal);
            let blues_m = blue_re.captures(reveal);
            let red = if reds_m.is_some() {
                reds_m.unwrap()[1].parse().expect("Cannot parse red")
            } else {
                0
            };
            let green = if greens_m.is_some() {
                greens_m.unwrap()[1].parse().expect("Cannot parse green")
            } else {
                0
            };
            let blue = if blues_m.is_some() {
                blues_m.unwrap()[1].parse().expect("Cannot parse blue")
            } else {
                0
            };
            if red > 12 || green > 13 || blue > 14 {
                game_ok = false;
            }

            min_red = min_red.max(red);
            min_green = min_green.max(green);
            min_blue = min_blue.max(blue);
        }
        if game_ok {
            sum += game_number
        }
        sum_power += min_red * min_green * min_blue;
    }
    println!("Puzzle 1 solution: {}", sum);
    println!("Puzzle 2 solution: {}", sum_power);
}
