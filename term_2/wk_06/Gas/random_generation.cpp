#include "utils.h"

int main()
{
    const uint amount = 10;

    const type length = 1;
    const type mass_1 = 1;
    const type mass_2 = 10;
    const type max_speed = 0.1;
    const type radius = 0.01;
    const type time = 10;

    std::string path{"result.txt"};
    std::string config{"config.txt"};

    std::ofstream out;


    Gas<type> my_gas(amount, length, mass_1, mass_2, max_speed, radius, std::move(path));
    my_gas.process(time);
    my_gas.invert_speeds();
    my_gas.process(time);
    const type time_total = my_gas.get_time();
    const uint collisions = my_gas.get_collisions();
    out.open(config);
    out << amount << ' ' << mass_1 << ' ' << mass_2 << ' '  << radius << ' ' << time_total << ' ' << length << ' '
        << collisions << '\n';
    out.close();
    return 0;
}
