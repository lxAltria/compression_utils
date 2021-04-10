#ifndef _AUXILIARY_HPP
#define _AUXILIARY_HPP

#include <vector>
#include <cmath>

namespace COMP_UTILS{

// compute and return the average of data in block_size^3 
template <class T>
std::vector<T> compute_average(T const * data, uint32_t n1, uint32_t n2, uint32_t n3, int block_size){
    uint32_t dim0_offset = n2 * n3;
    uint32_t dim1_offset = n3;
    uint32_t num_block_1 = (n1 - 1) / block_size + 1;
    uint32_t num_block_2 = (n2 - 1) / block_size + 1;
    uint32_t num_block_3 = (n3 - 1) / block_size + 1;
    std::vector<T> aggregated = std::vector<T>();
    uint32_t index = 0;
    T const * data_x_pos = data;
    for(int i=0; i<num_block_1; i++){
        int size_1 = (i == num_block_1 - 1) ? n1 - i * block_size : block_size;
        T const * data_y_pos = data_x_pos;
        for(int j=0; j<num_block_2; j++){
            int size_2 = (j == num_block_2 - 1) ? n2 - j * block_size : block_size;
            T const * data_z_pos = data_y_pos;
            for(int k=0; k<num_block_3; k++){
                int size_3 = (k == num_block_3 - 1) ? n3 - k * block_size : block_size;
                T const * cur_data_pos = data_z_pos;
                int n_block_elements = size_1 * size_2 * size_3;
                double sum = 0;
                for(int ii=0; ii<size_1; ii++){
                    for(int jj=0; jj<size_2; jj++){
                        for(int kk=0; kk<size_3; kk++){
                            sum += *cur_data_pos;
                            cur_data_pos ++;
                        }
                        cur_data_pos += dim1_offset - size_3;
                    }
                    cur_data_pos += dim0_offset - size_2 * dim1_offset;
                }
                aggregated.push_back(sum / n_block_elements);
                data_z_pos += size_3;
            }
            data_y_pos += dim1_offset * size_2;
        }
        data_x_pos += dim0_offset * size_1;
    }    
    return aggregated;
}

// get index of 3D data
inline uint32_t _index(int i, int j, int k, uint32_t dim0_offset, uint32_t dim1_offset){
    return i * dim0_offset + j * dim1_offset + k;
}

// compute and return the gradient of data using central difference
template <class T>
std::vector<std::vector<T>> compute_gradients(T const * data, uint32_t n1, uint32_t n2, uint32_t n3){
    std::vector<std::vector<T>> gradients;
    for(int i=0; i<3; i++){
        gradients.push_back(std::vector<T>());
    }
    uint32_t dim0_offset = n2 * n3;
    uint32_t dim1_offset = n3;    
    for(int i=1; i<n1 - 1; i++){
        for(int j=1; j<n2 - 1; j++){
            for(int k=1; k<n3 - 1; k++){
                gradients[0].push_back((data[_index(i + 1, j, k, dim0_offset, dim1_offset)] - data[_index(i - 1, j, k, dim0_offset, dim1_offset)]) / 2);
                gradients[1].push_back((data[_index(i, j + 1, k, dim0_offset, dim1_offset)] - data[_index(i, j - 1, k, dim0_offset, dim1_offset)]) / 2);
                gradients[2].push_back((data[_index(i, j, k + 1, dim0_offset, dim1_offset)] - data[_index(i, j, k - 1, dim0_offset, dim1_offset)]) / 2);
            }
        }
    }
    return gradients;
}

}
#endif
