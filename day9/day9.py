import numpy as np
from scipy.interpolate import lagrange

def parse_inputs(input_filename):
    measurements = []
    with open(input_filename, "r") as f:
        for l in f:
            measurements.append(np.array(list(map(int, l.split()))))
    return measurements

def guess_degrees(inputs):
    degrees = []
    for inp in inputs:
        diff = inp
        deg = 0
        while len(diff) > 1 and diff.any():
            diff = np.diff(diff, axis=0)
            deg += 1
        degrees.append(deg - 1)
    return degrees

def find_polynomials(inputs, poly_degs):
    polys = [lagrange(np.arange(poly_deg + 1), inp[:poly_deg+1]) for inp, poly_deg in zip(inputs, poly_degs)]
    return polys

def naive(inputs):
    nexts = []
    for inp in inputs:
        lasts = []
        diff = inp
        deg = 0
        while len(diff) > 1 and diff.any():
            lasts.append(diff[-1])
            diff = np.diff(diff, axis=0)
            deg += 1
        nexts.append(sum(lasts))
        print(nexts[-1])
    return sum(nexts)

def naive2(inputs):
    previous = []
    for inp in inputs:
        firsts = []
        diff = inp
        deg = 0
        while len(diff) > 1 and diff.any():
            firsts.append(diff[0])
            diff = np.diff(diff, axis=0)
            deg += 1
        sub = 0
        for i, first in enumerate(firsts):
            sub += (-1)**(i%2) * first
        previous.append(sub)
        print(previous[-1])
    return sum(previous)

def puzzle1(inputs, polys):
    print([poly(len(inp)) for inp, poly in zip(inputs, polys)])
    return np.sum([np.round(poly(len(inp))) for inp, poly in zip(inputs, polys)])

def main(input_filename):
    inputs = parse_inputs(input_filename)
    answer_1 = naive(inputs)
    print("Puzzle 1 answer: ", answer_1)
    answer_2 = naive2(inputs)
    print("Puzzle 2 answer: ", answer_2)



if __name__ == "__main__":
    main("./input.txt")