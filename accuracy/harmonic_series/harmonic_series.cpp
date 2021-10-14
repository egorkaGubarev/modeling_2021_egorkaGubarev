#include <iostream>

typedef unsigned int uint;

float get_sum_harmonic(uint index_max)
{
    float number;
    float sum = 0;
    for (uint index = 1; index <= index_max; ++ index){
        number = static_cast<float>(1) / index;
        sum += number;
    }
    return sum;
}

int main(int argc, char** argv)
{
    uint index_max_1 = 1000000;
    uint index_max_2 = 10000000;
    uint index_max_3 = 100000000;
    float sum_1 = get_sum_harmonic(index_max_1);
    std::cout << "Sum to 1000000: " << sum_1 << '\n';
    float sum_2 = get_sum_harmonic(index_max_2);
    std::cout << "Sum to 10000000: " << sum_2 << '\n';
    float sum_3 = get_sum_harmonic(index_max_3);
    std::cout << "Sum to 100000000: " << sum_3 << '\n';
    return 0;
}
