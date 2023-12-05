#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

struct PairHasher
{
    inline std::size_t operator()(const std::pair<int, int> &p) const
    {
        return p.first << sizeof(p.second) | p.second;
    }
};

class PartNumberParser
{
public:
    PartNumberParser(const char *inputFileName)
    {
        std::ifstream inputFile("./input.txt");
        if (inputFile.is_open())
        {
            std::string line;
            while (std::getline(inputFile, line))
            {
                std::vector<unsigned char> l;
                l.reserve(line.length());
                for (const char c : line)
                {
                    l.push_back(c);
                }
                grid_.push_back(l);
            }
        }
    }

    void displayGrid() const
    {
        for (const auto l : grid_)
        {
            for (const auto c : l)
            {
                std::cout << c;
            }
            std::cout << std::endl;
        }
    }

    int parse()
    {
        sum_ = 0;
        for (size_t i = 0; i < grid_.size(); i++)
        {
            const auto line = grid_[i];
            isNumberToAdd_ = false;
            for (size_t j = 0; j < line.size(); j++)
            {
                const char c = line[j];
                if (isdigit(c))
                {
                    currentNumber_.push_back(c);
                    for (int di = -1; di <= 1; di++)
                    {
                        if (di == -1 && i == 0)
                        {
                            continue;
                        }
                        if (di == 1 && i == grid_.size() - 1)
                        {
                            continue;
                        }
                        for (int dj = -1; dj <= 1; dj++)
                        {
                            if (di == 0 && dj == 0)
                            {
                                continue;
                            }
                            if (dj == -1 && j == 0)
                            {
                                continue;
                            }
                            if (dj == 1 && j == line.size() - 1)
                            {
                                continue;
                            }
                            const char neighbour = grid_[(int)i + di][(int)j + dj];
                            if (!isdigit(neighbour) && neighbour != '.')
                            {
                                isNumberToAdd_ = true;
                            }
                        }
                    }
                }
                else
                {
                    endNumber();
                }
            }
            endNumber();
        }
        return sum_;
    }

    int computeGearRatioSum()
    {
        for (size_t i = 0; i < grid_.size(); i++)
        {
            const auto line = grid_[i];
            isNumberToAdd_ = false;
            for (size_t j = 0; j < line.size(); j++)
            {
                const char c = line[j];
                if (isdigit(c))
                {
                    currentNumber_.push_back(c);
                    for (int di = -1; di <= 1; di++)
                    {
                        if (di == -1 && i == 0)
                        {
                            continue;
                        }
                        if (di == 1 && i == grid_.size() - 1)
                        {
                            continue;
                        }
                        for (int dj = -1; dj <= 1; dj++)
                        {
                            if (di == 0 && dj == 0)
                            {
                                continue;
                            }
                            if (dj == -1 && j == 0)
                            {
                                continue;
                            }
                            if (dj == 1 && j == line.size() - 1)
                            {
                                continue;
                            }
                            int neighbourRow = (int)i + di;
                            int neighbourCol = (int)j + dj;
                            const char neighbour = grid_[neighbourRow][neighbourCol];
                            if (neighbour == '*')
                            {
                                isNumberToAdd_ = true;
                                currentGear_ = std::make_pair(neighbourRow, neighbourCol);
                            }
                        }
                    }
                }
                else
                {
                    endNumberGear();
                }
            }
            endNumberGear();
        }
        int gearSum = 0;
        for (const auto kvGear : gears_)
        {
            if (kvGear.second.size() == 2)
            {
                gearSum += kvGear.second[0] * kvGear.second[1];
            }
        }
        return gearSum;
    }

protected:
    std::vector<std::vector<unsigned char>> grid_;
    std::vector<unsigned char> currentNumber_;
    std::pair<int, int> currentGear_;
    std::unordered_map<std::pair<int, int>, std::vector<int>, PairHasher> gears_;
    bool isNumberToAdd_;
    int sum_;

    void endNumber()
    {
        if (!currentNumber_.empty())
        {
            if (isNumberToAdd_)
            {
                std::string numberString(currentNumber_.begin(), currentNumber_.end());
                int number = std::stoi(numberString);
                sum_ += number;
            }
            isNumberToAdd_ = false;
            currentNumber_.clear();
        }
    }

    void endNumberGear()
    {
        if (!currentNumber_.empty())
        {
            if (isNumberToAdd_)
            {
                std::string numberString(currentNumber_.begin(), currentNumber_.end());
                int number = std::stoi(numberString);
                gears_[currentGear_].push_back(number);
            }
            isNumberToAdd_ = false;
            currentNumber_.clear();
        }
    }
};

int main()
{
    PartNumberParser p("./input.txt");
    int puzzle1_solution = p.parse();
    std::cout
        << "Puzzle 1 solution: " << puzzle1_solution << std::endl;
    int puzzle2_solution = p.computeGearRatioSum();
    std::cout << "Puzzle 2 solution: " << puzzle2_solution << std::endl;
    return 0;
}