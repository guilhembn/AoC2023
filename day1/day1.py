
def solve(filename: str) -> None:
    sum = 0
    with open(filename, 'r') as f:
        for line in f:
            first_digit = next(c for c in line if c.isdigit())
            second_digit = next(c for c in reversed(line) if c.isdigit())
            sum += int(str(first_digit) + str(second_digit))
    print("First puzzle solution:", sum)

def solve2(filename: str) -> None:
    digits = {"eight": "8", "two": "2", "one": "1", "three": "3", "four": "4", "five": "5", "six": "6", "seven": "7", "nine": "9",}
    sum = 0
    with open(filename, 'r') as f:
        for line in f:
            for i, c in enumerate(line):
                to_break = False
                if c.isdigit():
                    first_digit = c
                    break
                for letters, number in digits.items():
                    if line[i:].startswith(letters):
                        first_digit = number
                        to_break = True
                        break
                if to_break:
                    break
            for i, c in enumerate(line[::-1]):
                to_break = False
                if c.isdigit():
                    second_digit = c
                    break
                for letters, number in digits.items():
                    if line[::-1][i:].startswith(letters[::-1]):
                        second_digit = number
                        to_break = True
                        break
                if to_break:
                    break
            if first_digit is None or second_digit is None:
                raise RuntimeError(f"Either first or second digit has not been found. line: {line}, first digit: {first_digit}, second digit: {second_digit}")
            sum += int(str(first_digit) + str(second_digit))
    print("Second puzzle solution:", sum)


if __name__ == "__main__":
    solve("./input.txt")
    solve2("./input.txt")