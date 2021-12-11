#include <array>
#include "../headers/input.h"
#include "../headers/utils.h"

int main(const int argc, const char** const argv)
{
    std::array<data, 2> conditions{coordinate, speed};
    const LinearForceEquation<data> equation(frequency_pendulum, k_friction_linear, k_friction_cube, force, frequency_force);
    const std::string path{"result/result.txt"};
    solver.solve(conditions, equation, method, time, step, path, points);
    return 0;
}
