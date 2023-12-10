#include <deque>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

struct Coords
{
    size_t row;
    size_t col;

    bool operator==(const Coords other) const
    {
        return (row == other.row && col == other.col);
    }
};

std::ostream &operator<<(std::ostream &os, const Coords &c)
{
    return os << "[" << c.row << ";" << c.col << "]";
}

struct CoordsHash
{
public:
    size_t operator()(const Coords c) const
    {
        return std::hash<size_t>()(c.col) ^ std::hash<size_t>()(c.row); // Not ideal...
    }
};

class Tile
{
public:
    Tile(const char type, const size_t row, const size_t col) : type_(type), coords_{row, col}
    {
    }

    std::unordered_set<Coords, CoordsHash> connectedCoordinates() const
    {
        std::unordered_set<Coords, CoordsHash> connected;
        switch (type_)
        {
        case '.':
            break;
        case '|':
            connected.insert({coords_.row - 1, coords_.col});
            connected.insert({coords_.row + 1, coords_.col});
            break;
        case '-':
            connected.insert({coords_.row, coords_.col - 1});
            connected.insert({coords_.row, coords_.col + 1});
            break;
        case 'L':
            connected.insert({coords_.row - 1, coords_.col});
            connected.insert({coords_.row, coords_.col + 1});
            break;
        case 'J':
            connected.insert({coords_.row - 1, coords_.col});
            connected.insert({coords_.row, coords_.col - 1});
            break;
        case '7':
            connected.insert({coords_.row + 1, coords_.col});
            connected.insert({coords_.row, coords_.col - 1});
            break;
        case 'F':
            connected.insert({coords_.row + 1, coords_.col});
            connected.insert({coords_.row, coords_.col + 1});
            break;
        case 'S':
            break;
        default:
            break;
        }
        return connected;
    }

    Coords coords_;
    char type_;
};

class Grid
{
public:
    static Grid fromFile(const char *filename)
    {
        Grid g;
        std::ifstream inputFile("./input.txt");
        if (inputFile.is_open())
        {
            std::string line;
            while (std::getline(inputFile, line))
            {
                g.addLine(line);
            }
        }
        g.computeStartType();
        g.distancesFromStart_ = g.computeDistancesFromStart();
        g.insideLoop_ = g.computeInsideLoop();
        return g;
    }

    void addLine(std::string &str)
    {
        if (size_[1] == 0)
        {
            size_[1] = str.length();
        }
        else
        {
            // Assert size_[1] == str.length()
        }
        std::vector<Tile> newLine;
        newLine.reserve(size_[1]);
        for (size_t i = 0; i < str.length(); i++)
        {
            if (str[i] == 'S')
            {
                start_ = {size_[0], i};
            }
            newLine.push_back(Tile(str[i], size_[0], i));
        }
        grid_.push_back(newLine);
        size_[0]++;
    }

    void computeStartType()
    {
        Tile *start = (*this)[start_];
        std::unordered_set<char> conDir;
        for (const auto &n : neighbours(*start))
        {
            if (n.second != nullptr && n.second->connectedCoordinates().count(start_) == 1)
            {
                conDir.insert(n.first);
            }
        }
        if (conDir.count('N') && conDir.count('S'))
        {
            start->type_ = '|';
        }
        else if (conDir.count('E') && conDir.count('W'))
        {
            start->type_ = '-';
        }
        else if (conDir.count('N') && conDir.count('E'))
        {
            start->type_ = 'L';
        }
        else if (conDir.count('N') && conDir.count('W'))
        {
            start->type_ = 'J';
        }
        else if (conDir.count('S') && conDir.count('W'))
        {
            start->type_ = '7';
        }
        else if (conDir.count('S') && conDir.count('E'))
        {
            start->type_ = 'F';
        }
        else
        {
            std::cerr << "ERROR Cannot retrieve start type" << std::endl;
        }
    }

    std::unordered_map<char, Tile *> neighbours(const Tile &tile)
    {
        std::unordered_map<char, Tile *> neighbours;
        neighbours['N'] = (*this)[{tile.coords_.row - 1, tile.coords_.col}];
        neighbours['S'] = (*this)[{tile.coords_.row + 1, tile.coords_.col}];
        neighbours['E'] = (*this)[{tile.coords_.row, tile.coords_.col + 1}];
        neighbours['W'] = (*this)[{tile.coords_.row, tile.coords_.col - 1}];
        return neighbours;
    }

    std::vector<Tile *>
    connectedTo(const Tile &tile)
    {
        std::vector<Tile *> con;
        for (const auto &connected : tile.connectedCoordinates())
        {
            if ((*this)[connected] == nullptr)
            {
                continue;
            }
            con.push_back(&grid_[connected.row][connected.col]);
        }
        return con;
    }

    void displayGrid()
    {
        for (const auto &row : grid_)
        {
            for (const auto tile : row)
            {
                bool toReset = false;
                if (tile.coords_ == start_)
                {
                    std::cout << "\033[1;32m";
                    toReset = true;
                }
                else if (distancesFromStart_.count(&grid_[tile.coords_.row][tile.coords_.col]))
                {
                    std::cout << "\033[1;34m";
                    toReset = true;
                }
                else if (insideLoop_.count(&grid_[tile.coords_.row][tile.coords_.col]))
                {
                    std::cout << "\033[33m";
                    toReset = true;
                }

                std::cout << tile.type_;
                if (toReset)
                {
                    std::cout << "\033[0m";
                }
            }
            std::cout << std::endl;
        }
    }

    Tile *operator[](const Coords &coords)
    {
        if (coords.row < 0 || coords.row >= size_[0] || coords.col < 0 || coords.col >= size_[1])
        {
            return nullptr;
        }
        return &grid_[coords.row][coords.col];
    }

    std::unordered_map<Tile *, int> computeDistancesFromStart()
    {
        std::deque<Tile *> toVisit;
        std::unordered_map<Tile *, int> distances;
        toVisit.push_back((*this)[start_]);
        distances[(*this)[start_]] = 0;
        Tile *current = (*this)[start_];
        while (!toVisit.empty())
        {

            current = toVisit.front();
            toVisit.pop_front();
            for (const auto &n : connectedTo(*current))
            {
                if (distances.count(n) == 0)
                {
                    distances[n] = distances[current] + 1;
                    toVisit.push_back(n);
                }
            }
        }
        return distances;
    }

    std::pair<Tile *, int> getMaxDistanceFromStart()
    {
        std::pair<Tile *, int> maxPair({nullptr, 0});
        for (const auto &p : distancesFromStart_)
        {
            if (p.second > maxPair.second)
            {
                maxPair = p;
            }
        }
        return maxPair;
    }

    std::unordered_set<Tile *> computeInsideLoop()
    {
        std::unordered_set<Tile *> inside;
        for (auto &row : grid_)
        {
            for (auto &tile : row)
            {
                if (distancesFromStart_.count(&tile))
                {
                    continue;
                }
                int crossing = 0;
                for (size_t i = 1; i <= tile.coords_.row; i++)
                {
                    Tile *ray = &grid_[tile.coords_.row - i][tile.coords_.col];
                    if (distancesFromStart_.count(ray) && ray->type_ != '|' && ray->type_ != 'J' && ray->type_ != '7')
                    {
                        crossing++;
                    }
                }
                std::cout << crossing << std::endl;
                if (crossing % 2 == 1)
                {
                    inside.insert(&tile);
                }
            }
        }
        return inside;
    }

    std::unordered_set<Tile *> getInsideLoop()
    {
        return insideLoop_;
    }

protected:
    std::vector<std::vector<Tile>> grid_;
    size_t size_[2];
    Coords start_;
    std::unordered_map<Tile *, int> distancesFromStart_;
    std::unordered_set<Tile *> insideLoop_;

    Grid() : size_{0, 0}, start_{0, 0} {}
};

int main()
{
    Grid g = Grid::fromFile("./input.txt");
    g.displayGrid();
    std::cout << "Puzzle 1 solution: " << g.getMaxDistanceFromStart().second << std::endl;
    std::cout << "Puzzle 2 solution: " << g.getInsideLoop().size() << std::endl;
}