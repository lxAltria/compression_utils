#ifndef _EVALUATE_HPP
#define _EVALUATE_HPP

#include <iostream>
#include <cassert>
#include <vector>
#include <cmath>
#include <ctime>
#include "auxiliary.hpp"

namespace COMP_UTILS{

// evaluate L^infinity error of data and dec_data
template<class T>
T evaluate_L_inf(T const * data, T const * dec_data, uint32_t num_elements, bool normalized=true, bool verbose=false){
    T L_inf_error = 0;
    T L_inf_data = 0;
    for(int i=0; i<num_elements; i++){
        if(L_inf_data < fabs(data[i])) L_inf_data = fabs(data[i]);
        T error = data[i] - dec_data[i];
        if(L_inf_error < fabs(error)) L_inf_error = fabs(error);
    }
    if(verbose){
        std::cout << "Max absolute value = " << L_inf_data << std::endl;
        std::cout << "L_inf error = " << L_inf_error << std::endl;
        std::cout << "L_inf error (normalized) = " << L_inf_error / L_inf_data << std::endl;
    }
    return normalized ? L_inf_error / L_inf_data : L_inf_error;
}

// evaluate L^2 error of data and dec_data (note: not normalized to data size)
template<class T>
double evaluate_L2(T const * data, T const * dec_data, uint32_t num_elements, bool normalized=true, bool verbose=false){
    double L2_error = 0;
    T L_inf_data = 0;
    for(int i=0; i<num_elements; i++){
        if(L_inf_data < fabs(data[i])) L_inf_data = fabs(data[i]);
        T error = data[i] - dec_data[i];
        L2_error += error * error;
    }
    if(verbose){
        std::cout << "L_2 error = " << L2_error << std::endl;
        std::cout << "L_2 error (normalized) = " << L2_error / (L_inf_data * L_inf_data) << std::endl;
    }
    return normalized ? sqrt(L2_error) / L_inf_data : sqrt(L2_error);
}

// return L^2 error of gradients
template<class T>
void evaluate_gradients(T const * data, T const * dec_data, uint32_t n1, uint32_t n2, uint32_t n3){
    auto gradients = compute_gradients(data, n1, n2, n3);
    auto gradients_dec = compute_gradients(dec_data, n1, n2, n3);
    double gradients_L2 = 0;
    for(int i=0; i<gradients.size(); i++){
        auto L2_error = evaluate_L2(gradients[i].data(), gradients_dec[i].data(), gradients[i].size(), false, false);
        gradients_L2 += L2_error * L2_error;
        std::cout << "L^2 error of Gradient " << i << " = " << L2_error << std::endl;
    }
    std::cout << "L^2 error of all gradients = " << sqrt(gradients_L2) << std::endl; 
}

template<class T>
void evaluate_average(T const * data, T const * dec_data, uint32_t n1, uint32_t n2, uint32_t n3, int block_size = 0){
    if(block_size == 0){
        double average = 0;
        double average_dec = 0;
        for(int i=0; i<n1 * n2 * n3; i++){
            average += data[i];
            average_dec += dec_data[i]; 
        }
        std::cout << "Error in global average = " << fabs(average - average_dec)/(n1*n2*n3) << std::endl;
    }
    else{
        auto average = compute_average(data, n1, n2, n3, block_size);
        auto average_dec = compute_average(dec_data, n1, n2, n3, block_size);        
        std::cout << "L^infinity error of average with block size " << block_size << " = " << evaluate_L_inf(average.data(), average_dec.data(), average.size(), true, false) << std::endl;
        std::cout << "L^2 error of average with block size " << block_size << " = " << evaluate_L2(average.data(), average_dec.data(), average.size(), true, false) << std::endl;
    }
}

}
#endif