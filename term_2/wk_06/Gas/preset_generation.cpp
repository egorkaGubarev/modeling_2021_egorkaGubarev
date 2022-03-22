#include "utils.h"

int main()
{
    const type length = 1;
    const type radius = 0.01;
    const type time = 0.01;

    std::string path{"result.txt"};
    std::string config{"config.txt"};

    std::vector<type> masses{1, 1, 1, 1};
    std::vector<type> speeds{1, 0, 0, -1};

    std::ofstream out;


    const type center = length / 2;
    const type second_position = center - radius;
    const type third_position = center + radius;
    const type distance = 3 * radius / 2;
    const type left_position = second_position - distance;
    const type right_position = third_position + distance;
    std::vector<type> positions{left_position, second_position, third_position, right_position};
    Gas<type> my_gas(masses, positions, speeds, length, radius, std::move(path));
    my_gas.process(time);
    const uint collisions = my_gas.get_collisions();
    const uint amount = my_gas.get_amount();
    const type mass_1 = my_gas.get_mass_1();
    const type mass_2 = my_gas.get_mass_2();
    out.open(config);
    out << amount << ' ' << mass_1 << ' ' << mass_2 << ' '  << radius << ' ' << time << ' ' << length << ' '
    << collisions << '\n';
    out.close();
    return 0;
}
