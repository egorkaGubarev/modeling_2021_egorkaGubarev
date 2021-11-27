#include <array>
#include <iostream>
#include "../headers/input.h"
#include "../headers/utils.h"

typedef unsigned int uint;
typedef float data;

int main(const int argc, const char** const argv)
{
    uint i = 0;
    const data frequency_start = frequency_pendulum / 2;
    const data frequency_end = frequency_pendulum * 2;
    const data step_frequency = 0.01;
    data frecuency_force_current = frequency_start;
    const std::string path_pattern{"result/result_"};
    const std::string path_frequency{"frequency/frequency.txt"};
    std::string i_str, path;
    std::ofstream out;
    out.open(path_frequency);
    while(frecuency_force_current < frequency_end){
        std::array<data, 2> conditions{coordinate, speed};
        i_str  = std::to_string(i);
        path = path_pattern + i_str + ".txt";
        const PositiveForceEquation<data> equation(frequency_pendulum, k_friction_linear, k_friction_cube, force, frecuency_force_current);
        solver.solve(conditions, equation, method, time, step, path, points);
        out << frecuency_force_current << '\n';
        ++ i;
        frecuency_force_current += step_frequency;
    }
    return 0;
}
