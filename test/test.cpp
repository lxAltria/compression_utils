#include <iostream>
#include <cassert>
#include <cstring>
#include "evaluate.hpp"
#include "file.hpp"

using namespace std;

int main(int argc, char ** argv){
    string data_file = argv[1];
    string dec_data_file = argv[2];
    const int n1 = atoi(argv[3]);
    const int n2 = atoi(argv[4]);
    const int n3 = atoi(argv[5]);
    const int block_size = atoi(argv[6]);

    auto data = COMP_UTILS::readfile<float>(data_file.c_str());
    auto dec_data = COMP_UTILS::readfile<float>(dec_data_file.c_str());
    assert(data.size() == n1 * n2 * n3);
    assert(data.size() == dec_data.size());
    COMP_UTILS::evaluate_L_inf(data.data(), dec_data.data(), data.size(), true, true);
    COMP_UTILS::evaluate_L2(data.data(), dec_data.data(), data.size(), true, true);
    COMP_UTILS::evaluate_gradients(data.data(), dec_data.data(), n1, n2, n3);
    COMP_UTILS::evaluate_average(data.data(), dec_data.data(), n1, n2, n3, block_size);
    return 0;
}

