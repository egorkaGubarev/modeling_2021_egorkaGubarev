#include <iostream>
#include <math.h>

typedef unsigned int uint;

float get_sum_inverted_squares_1(uint index_max)
{
    float number = 1;
    float sum = 0;
    for (uint index = 1; index < index_max; ++ index){
        number /= 2;
        sum += number;
    }
    return sum;
}

float get_sum_inverted_squares_2(uint index_max)
{
    float number;
    float sum = 0;
    for (uint index = 1; index < index_max; ++ index){
        uint divider = std::pow(index, 2);
        number = static_cast<float>(1) / divider;
        sum += number;
    }
    return sum;
}

int main(int argc, char** argv)
{
    uint index_max = 4000;
    float sum_1 = get_sum_inverted_squares_1(index_max);
    std::cout << "Sum to " << index_max << ": " << sum_1 << '\n';
    float sum_2 = get_sum_inverted_squares_2(index_max);
    std::cout << "Sum to " << index_max << ": " << sum_2 << '\n';
    return 0;
}
