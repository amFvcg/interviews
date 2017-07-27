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
#include <iostream>
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

int main()
{
    Entries input {{false, 1, 2}, {true, 2, 3}, {true, 1, 3}, {false, 4, 1}, {false, 1, 1}, {true, 1, 2}};
    for (const auto& item : input)
        std::cout << item << " | ";
    std::cout << std::endl;
    for (auto&& item : accumulate(input))
        std::cout << item << " | ";
    std::cout << std::endl;
    accumulate({});
    return 0;
}
