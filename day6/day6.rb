
def parse_race_1(filename)
    file = File.open(filename)
    file_data = file.readlines.map(&:chomp)

    times = file_data[0].split
    times.delete_at(0)
    times = times.map {|t| t.to_f}
    distances = file_data[1].split
    distances.delete_at(0)
    distances = distances.map {|d| d.to_f}
    races = times.zip(distances)
    return races
end

def parse_race_2(filename)
    file = File.open(filename)
    file_data = file.readlines.map(&:chomp)
    times = file_data[0].split
    times.delete_at(0)
    time = times.join
    distances = file_data[1].split
    distances.delete_at(0)
    distance = distances.join
    return [time.to_f, distance.to_f]
end

def get_roots(time, distance)
    return [(time - Math.sqrt(time ** 2 - 4 * distance)) / 2, (time + Math.sqrt(time ** 2 - 4 * distance)) / 2]
end

def get_integers_between_floats(a, b)
    down, up = [a, b].minmax
    if down.ceil() == down
        down += 1
    end
    if up.floor() == up
        up -= 1
    end
    return up.floor - down.ceil + 1
end

def puzzle1(races)
    product = 1
    for race in races
        product *= get_integers_between_floats(*get_roots(*race))
    end
    return product
end

def puzzle2(race)
    return get_integers_between_floats(*get_roots(*race))
end

races1 = parse_race_1("./input.txt")
puts "Puzzle 1 solution: #{puzzle1(races1)}"
race2 = parse_race_2("./input.txt")
puts "Puzzle 2 solution: #{puzzle2(race2)}"
