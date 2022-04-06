#include "utils.h"

int main()
{
    const type length = 1;
    const type radius = 0.01;
    const type time = 10;

    std::string path{"result.txt"};
    std::string config{"config.txt"};

    std::vector<type> masses{1, 3, 1, 3, 1, 3, 1, 3, 1, 3};
    std::vector<type> speeds{10, 0, -1, 1, 0, -1, 1, 0, -1, 1};

    std::ofstream out;


    std::vector<type> positions{0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 0.15};
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
