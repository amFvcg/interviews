//#!/bin/env python3
//import random
//import sys
//
//assert len(sys.argv) == 3, 'Invalid number of parameters, expected 2'
//size = int(sys.argv[1])
//key_count = int(sys.argv[2])
//
//with open('accumulate.test_data_{}_{}'.format(size, key_count), 'w') as f:
//    f.write('{}\n'.format(size))
//    for i in range(size):
//        z = f.write('{},{},{}\n'.format(
//            random.randint(0, 1),
//            random.randint(0, key_count),
//            1
//            )
//        )

#include <random>
#include <fstream>

int main(int argc, char** argv)
{
    int size = std::stoi(argv[1]);
    int key_count = std::stoi(argv[2]);
    std::ofstream file("accumulate.test_data_"
                       + std::to_string(size)
                       + "_"
                       + std::to_string(key_count));

    std::random_device r;
    std::default_random_engine e1(r());
    std::uniform_int_distribution<int> bool_gen(0, 1);
    std::uniform_int_distribution<int> int_gen(0, key_count);

    file << size << '\n';
    for (int i = 0; i < size; ++i)
    {
        file << bool_gen(e1) << ',' << int_gen(e1) << ',' << 1 << '\n';
    }
    return 0;
}
