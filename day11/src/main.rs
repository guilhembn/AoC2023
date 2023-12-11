use std::collections::HashMap;
use std::fs::File;
use std::io::{self, prelude::*, BufReader};
use std::sync::Arc;

#[derive(PartialEq, Eq, Debug, Hash)]
enum CellType {
    None,
    Galaxy,
}

#[derive(Debug, PartialEq, Eq, Hash)]
struct Cell {
    pub row: u32,
    pub col: u32,
    pub t: CellType,
}

#[derive(Debug)]
struct Map {
    pub map: Vec<Vec<Arc<Cell>>>,
    pub galaxies: Vec<Arc<Cell>>,
    pub empty_rows: Vec<u32>,
    pub empty_cols: Vec<u32>,
}

impl Map {
    pub fn from_file(filename: &str) -> io::Result<Map> {
        let mut map = Vec::new();
        let mut galaxies = Vec::new();
        let file = File::open(filename)?;
        let reader = BufReader::new(file);

        for (row, line) in reader.lines().enumerate() {
            let mut line_vec = Vec::new();
            for (col, c) in line?.chars().enumerate() {
                line_vec.push(Arc::new(Cell {
                    row: row as u32,
                    col: col as u32,
                    t: if c == '#' {
                        CellType::Galaxy
                    } else {
                        CellType::None
                    },
                }));
                if c == '#' {
                    galaxies.push(line_vec.last().unwrap().clone());
                }
            }
            map.push(line_vec);
        }
        let mut to_ret = Map {
            map: map,
            galaxies: galaxies,
            empty_rows: Vec::new(),
            empty_cols: Vec::new(),
        };
        to_ret.fill_empty_rows_cols();
        Ok(to_ret)
    }
    fn fill_empty_rows_cols(&mut self) {
        for col in 0..self.map[0].len() {
            let mut is_empty = true;
            for row in 0..self.map.len() {
                if self.map[row][col].t == CellType::Galaxy {
                    is_empty = false;
                    break;
                }
            }
            if is_empty {
                self.empty_cols.push(col as u32);
            }
        }

        for row in 0..self.map.len() {
            let mut is_empty = true;
            for col in 0..self.map[0].len() {
                if self.map[row][col].t == CellType::Galaxy {
                    is_empty = false;
                    break;
                }
            }
            if is_empty {
                self.empty_rows.push(row as u32);
            }
        }
    }
    pub fn compute_distance_pairs(
        &self,
        expension_factor: u64,
    ) -> HashMap<(Arc<Cell>, Arc<Cell>), u64> {
        let mut distances: HashMap<(Arc<Cell>, Arc<Cell>), u64> = HashMap::new();
        for i in 0..self.galaxies.len() - 1 {
            for j in (i + 1)..self.galaxies.len() {
                let g1 = self.galaxies[i].clone();
                let g2 = self.galaxies[j].clone();
                let mut d: u64 = g1.row.abs_diff(g2.row) as u64 + g1.col.abs_diff(g2.col) as u64;
                let row_max = g1.row.max(g2.row);
                let row_min = g1.row.min(g2.row);
                let col_max = g1.col.max(g2.col);
                let col_min = g1.col.min(g2.col);
                for r in &self.empty_rows {
                    if row_min <= *r && *r <= row_max {
                        d += expension_factor;
                    }
                }
                for c in &self.empty_cols {
                    if col_min <= *c && *c <= col_max {
                        d += expension_factor;
                    }
                }
                distances.insert((g1, g2), d);
            }
        }
        distances
    }
}

fn main() {
    let map = Map::from_file("./input.txt").unwrap();
    let puzzle_1_answer: u64 = map.compute_distance_pairs(1).values().sum();
    println!("Puzzle 1 solution: {}", &puzzle_1_answer);
    let puzzle_2_answer: u64 = map.compute_distance_pairs(1_000_000 - 1).values().sum();
    println!("Puzzle 2 solution: {}", &puzzle_2_answer);
}
