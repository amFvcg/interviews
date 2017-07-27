/*
 * =====================================================================================
 *
 *       Filename:  accumulate.cpp
 *
 *    Description:  Exercise is about accumulating structs {bool, int, double},
 *                  where {bool, int} is a key, and {double} is a value; When
 *                  there's more than one bool == true, no aggregation should
 *                  occur;
 *
 *        Created:  27.07.2017 20:31:01
 *
 * =====================================================================================
 */
#include <algorithm>
#include <chrono>
#include <iostream>
#include <fstream>
#include <numeric>
#include <tuple>
#include <vector>

struct Entry {
    bool b;
    int i;
    float v;
};
bool operator<(const Entry& lhs, const Entry& rhs) {
    return (lhs.b < rhs.b) || (lhs.b == rhs.b && lhs.i < rhs.i);
}
std::ostream& operator<<(std::ostream& stream, const Entry& entry) {
    stream << '{'
        << entry.b 
        << ", "
        << entry.i
        << ", "
        << entry.v
        << '}'
        ;
    return stream;
}

using Entries = std::vector<Entry>;

Entries accumulate(Entries input)
{
    std::sort(input.begin(), input.end(), 
                operator<                 
    );
    auto it = std::find_if(input.begin(), input.end(), [](const auto& item) { return item.b; }); 
    auto range = std::equal_range(it, input.end(), *it, operator<);
    while (range.first != std::end(input))
    {
        if (std::distance(range.first, range.second) > 1)
            it = input.erase(range.first, range.second);
        else
            it = range.second;
        range = std::equal_range(it, std::end(input), *it, operator<);
    }
    Entries result;
    range = std::equal_range(input.begin(), input.end(), *input.begin(), operator<);
    while(range.first != std::end(input))
    {
        result.push_back(
                {
                range.first->b,
                range.first->i,
                std::accumulate(range.first, range.second, 
                    (float)0.0, [](const auto& init, const auto& elem) { return init + elem.v; })
                }
                );
        range = std::equal_range(range.second, input.end(), *range.second, operator<);
    }
    return result;
}

Entries read_entries(std::string filename)
{
    std::ifstream file(filename.c_str());
    std::string line;
    int size;
    if (!std::getline(file, line))
        return {};
    size = std::stoi(line);
    Entries input;
    input.reserve(size);
    while(std::getline(file, line))
    {
        std::string::size_type pos = 0;
        std::string::size_type n = line.find(',', pos);
        Entry e;
        if (n != std::string::npos)
            e.b = std::stoi(line.substr(pos, n));
        else
            continue;
        pos = n+1;
        n = line.find(',', pos);
        if (n != std::string::npos)
            e.i = std::stoi(line.substr(pos, n));
        else
            continue;
        pos = n+1;
        e.v = std::stof(line.substr(pos, line.size()));
        input.emplace_back(std::move(e));
    }
    return input;
}

int main(int argc, char** argv)
{
    Entries input {{false, 1, 2}, {true, 2, 3}, {true, 1, 3}, {false, 4, 1}, {false, 1, 1}, {true, 1, 2}};
    for (const auto& item : input)
        std::cout << item << " | ";
    std::cout << std::endl;
    for (auto&& item : accumulate(input))
        std::cout << item << " | ";
    std::cout << std::endl;
    auto beg = std::chrono::system_clock::now();
    auto entries = read_entries(argv[1]);
    auto end = std::chrono::system_clock::now();
    std::cout << "Read entries took: " << (end-beg).count() << std::endl;
    beg = std::chrono::system_clock::now();
    accumulate(entries);
    end = std::chrono::system_clock::now();
    std::cout << "Parse entries took: " << (end-beg).count() << std::endl;
    return 0;
}
