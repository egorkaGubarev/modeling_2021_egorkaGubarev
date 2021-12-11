#include <array>
#include <fstream>
#include "../headers/input.h"
#include "../headers/utils.h"

typedef float t_data;

int main(const int argc, const char** const argv)
{
    const std::array<data, 2> conditions{coordinate, speed};
    std::array<data, 2> result;
    LinearSquareWaveEquation<data> equation(frequency_pendulum, k_friction_linear, k_friction_cube, force, frequency_force);
    const std::string path{"result/result.txt"};
    std::ofstream out;
    out.open(path);
    t_data current_time = 0;
    const t_data step_analytic = time / (points - 1);
    while(current_time < time){
        equation.get_solution(conditions, current_time, result);
        write<data, 2>(equation, current_time, result, out);
        current_time += step_analytic;
    }
    return 0;
}
