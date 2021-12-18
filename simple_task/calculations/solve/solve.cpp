#include <array>
#include <fstream>
#include "../headers/input.h"
#include "../headers/utils.h"

int main(const int argc, const char** const argv)
{
    const data delta = time / points;
    std::array<data, 4> conditions{coordinate1, coordinate2, speed1, speed2};
    const DoubleEquation<data> equation(g, m1, m2, l1, l2);
    const std::string path{"result/result.txt"};
    const std::string path_parameters{"parameters/parameters.txt"};
    std::ofstream out;
    out.open(path_parameters);
    out << m1 << ' ' << m2 << ' ' << l1 << ' ' << l2 << ' ' << delta;
    out.close();
    solver.solve(conditions, equation, method, time, step, path, points);
    return 0;
}
