#include <iostream>

typedef unsigned int uint;

template<typename t_data>
const uint make_sum(const t_data end, const t_data step)
{
    uint iterration = 0;
    t_data sum = 0;
    while (sum < end){
        sum += step;
        ++ iterration;
    }
    return iterration;
}

template<typename t_data>
const uint make_kahan_sum(const t_data end, const t_data step)
{
    uint iterration = 0;
    t_data sum = 0;
    t_data error = 0;
    while (sum < end){
        t_data step_corrected = step - error;
        t_data t = sum + step_corrected;
        error = t - sum - step_corrected;
        sum = t;
        ++ iterration;
    }
    return iterration;
}

int main(int argc, char** argv)
{
    const float end = 1;
    const float step = 0.00000001;
    const uint iterrations_amount = make_kahan_sum(end, step);
    std::cout << iterrations_amount << '\n';
    return 0;
}
