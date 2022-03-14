#include <iostream>
#include <fstream>
#include <string>

typedef unsigned int uint;

float logistic(const float r, const float x)
{
    const float next = r * x * (1 - x);
    return next;
}

void get_points(const float r, const float x0, const uint amount, const std::string& all)
{
    float x = x0;
    std::ofstream out_all;
    std::ofstream out_last;
    out_all.open(all);
    out_all << 0 << ' ' << r << ' ' << x << '\n';
    for (uint i = 1; i < amount; ++ i){
        x = logistic(r, x);
        out_all << i << ' ' << r << ' ' << x << '\n';
    }
    out_all.close();
    out_last.close();
}

void investigate_r(const float x0, const uint amount, const std::string& all, const float r_max, const float step)
{
    uint i = 0;
    float r = 2.8;
    while(r < r_max){
        const std::string r_str = std::to_string(i);
        const std::string all_path = all + r_str + ".txt";
        get_points(r, x0, amount, all_path);
        ++ i;
        r += step;
    }
    std::cout << "Files: " << i << '\n';
}

int main()
{
    const uint amount = 1000;
    const float r_max = 4;
    const float step = 0.001;
    const float x0 = 0.1;
    const std::string all{"all"};
    investigate_r(x0, amount, all, r_max, step);
    return 0;
}
