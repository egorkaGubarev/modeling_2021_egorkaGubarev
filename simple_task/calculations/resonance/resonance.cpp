#include <array>
#include <iostream>
#include "../headers/input.h"
#include "../headers/utils.h"

typedef unsigned int uint;
typedef float data;

int main(const int argc, const char** const argv)
{
    uint j = 0;
    const data friction_start = 0.01;
    const data friction_end = 2;
    const data friction_step = 1.1;
    const data frequency_start = 0.7;
    const data frequency_end = 1.07;
    const data step_frequency = 1.01;
    data friction_current = friction_start;
    const std::string path_pattern{"result/result_"};
    const std::string path_frequency{"frequency/frequency.txt"};
    const std::string path_friction{"friction/friction.txt"};
    std::string i_str, j_str, path;
    std::ofstream out_frecuency, out_friction;
    out_frecuency.open(path_frequency);
    out_friction.open(path_friction);
    while(friction_current < friction_end){
        j_str = std::to_string(j);
        uint i = 0;
        data frecuency_force_current = frequency_start;
        while(frecuency_force_current < frequency_end){
            std::array<data, 2> conditions{coordinate, speed};
            i_str  = std::to_string(i);
            path = path_pattern + j_str + '_' + i_str +  ".txt";
            const LinearForceEquation<data> equation(frequency_pendulum, friction_current, k_friction_cube, force, frecuency_force_current);
            solver.solve(conditions, equation, method, time, step, path, points);
            if(j == 0){
                out_frecuency << frecuency_force_current << '\n';
            }
            ++ i;
            frecuency_force_current *= step_frequency;
        }
        out_friction << friction_current << '\n';
        ++ j;
        std::cout << "Friction: " << friction_current << " rad / c" << '\n';
        friction_current *= friction_step;
    }

    return 0;
}
