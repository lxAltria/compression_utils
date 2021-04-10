#ifndef _FILE_HPP
#define _FILE_HPP

#include <vector>
#include <fstream>

namespace COMP_UTILS{

template<typename T>
std::vector<T> readfile(char const * file) {
    std::ifstream fin(file, std::ios::binary);
    if (!fin) {
        std::cout << " Error, Couldn't find the file" << "\n";
        return std::vector<T>();
    }
    fin.seekg(0, std::ios::end);
    const uint32_t num_elements = fin.tellg() / sizeof(T);
    fin.seekg(0, std::ios::beg);
    auto data = std::vector<T>(num_elements);
    fin.read(reinterpret_cast<char *>(&data[0]), num_elements * sizeof(T));
    fin.close();
    return data;
}

template<typename T>
T * readfile(char const * file, uint32_t &num) {
    std::ifstream fin(file, std::ios::binary);
    if (!fin) {
        std::cout << " Error, Couldn't find the file" << "\n";
        return NULL;
    }
    fin.seekg(0, std::ios::end);
    const uint32_t num_elements = fin.tellg() / sizeof(T);
    fin.seekg(0, std::ios::beg);
    T * data = (T *) malloc(num_elements * sizeof(T));
    fin.read(reinterpret_cast<char *>(data), num_elements * sizeof(T));
    fin.close();
    num = num_elements;
    return data;
}

template<typename T>
void writefile(const char * file, T * data, uint32_t num_elements) {
    std::ofstream fout(file, std::ios::binary);
    fout.write(reinterpret_cast<const char *>(&data[0]), num_elements * sizeof(T));
    fout.close();
}

}
#endif