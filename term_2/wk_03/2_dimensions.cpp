#include <array>
#include <cmath>
#include <iostream>
#include <fstream>
#include <random>
#include <string>
#include <vector>

typedef int data_type;
typedef unsigned int uint;

template <typename type>
class UniformRandomGenerator
{
public:
    explicit UniformRandomGenerator(long long seed)
    {
        if (seed == -1){
            seed = time(nullptr);
        }
        engine_ = new std::mt19937_64(seed);
    }

    std::vector<type> create_sequence(const unsigned int n, const type min, const type max)
    {
        std::vector<type> sequence(n);
        for (unsigned int i = 0; i < n; ++ i){
            sequence[i] = get_number(min, max);
        }
        return sequence;
    }

    type get_number(type min, type max)
    {
        std::uniform_real_distribution<type> distr(min, max);
        const type number = distr(*engine_);
        return number;
    }

private:
    std::mt19937_64* engine_;
};

template<>
int UniformRandomGenerator<int>::get_number(int min, int max)
{
    std::uniform_int_distribution<int> distr(min, max);
    const int number = distr(*engine_);
    return number;
}

template<typename type>
std::vector<std::pair<type, type>>& continue_wandering(std::vector<std::pair<type, type>>& wandering,
                                                      const uint size, const type min, const type max,
                                                      UniformRandomGenerator<type>& generator)
{
    const type x_delta = generator.get_number(min, max);
    const type y_delta = generator.get_number(min, max);
    const std::pair<type, type>& last = wandering[size];
    const type x_last = last.first;
    const type y_last = last.second;
    const type x_current = x_last + x_delta;
    const type y_current = y_last + y_delta;
    std::pair<type, type> current{x_current, y_current};
    wandering.push_back(current);
    return wandering;
}

/*template<typename type>
void collect_data(const uint tests, const uint steps, UniformRandomGenerator<type>& generator)
{
    std::vector<std::vector<std::pair<type, type>>>
}*/

int main()
{
    const long long seed = -1;
    const data_type min = -1;
    const data_type max = 1;
    UniformRandomGenerator<data_type> generator(seed);
    std::vector<data_type> number = generator.create_sequence(1, min, max);
    const data_type random = number[0];
    std::cout << random << '\n';
    return 0;
}
