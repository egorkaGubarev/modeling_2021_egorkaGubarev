#include <algorithm>
#include <chrono>
#include <iostream>
#include <fstream>
#include <cmath>
#include <random>
#include <string>
#include <vector>

typedef float my_type;
typedef unsigned int uint;

class UniformRandomGenerator
{
    public:
        static std::vector<my_type> create_sequence(const size_t n, const my_type min, const my_type max, int seed)
        {
            if (seed == -1){
                seed = time(0);
            }
            std::mt19937_64 engine(seed);
            std::uniform_real_distribution<my_type> distr(min, max);
            std::vector<my_type> sequence(n);
            for (size_t i = 0; i < n; ++ i){
                sequence[i] = distr(engine);
            }
            return sequence;
        }
};

class NormalRandomGenerator
{
    public:
        static std::vector<float> create_sequence(const size_t n, const float mean, const float sigma, int seed)
        {
            if (seed == -1){
                seed = time(0);
            }
            std::mt19937_64 engine(seed);
            std::normal_distribution distr(mean, sigma);
            std::vector<float> sequence(n);
            for (size_t i = 0; i < n; ++ i){
                sequence[i] = distr(engine);
            }
            return sequence;
        }
};

template <typename t>
std::ostream& print_vector(const std::vector<t>& to_print, std::ostream& out)
{
    for(t number: to_print){
        out << number << ' ';
    }
    out << '\n';
    return out;
}

template <typename t_data>
std::vector<uint> get_histogram_data(const std::vector<t_data>& data, const t_data min, const t_data max, const t_data size)
{
    const t_data range = max - min;
    const uint amount = std::floor(range / size);
    std::vector<uint> buckets(amount, 0);
    for(const t_data value: data){
        const t_data delta = value - min;
        const uint number = std::floor(delta / size);
        ++ buckets[number];
    }
    return buckets;
}

template <typename t_data>
t_data make_parabolic_distribution(const t_data number)
{
    const t_data result = std::pow(3 * number, 1.0f / 3);
    return result;
}

int main(const int argc, const char** const argv)
{
    const size_t amount = 10000;
    const my_type min = 0;
    const my_type max = 100;
    const my_type size = 1;
    const float mean = 0;
    const float sigma = 1;
    const int seed = -1;
    const std::string path{"result.txt"};
    std::ofstream out;
    out.open(path);
    UniformRandomGenerator uniform_generator;
    NormalRandomGenerator normal_generator;
    std::vector<my_type> uniform_sequence = uniform_generator.create_sequence(amount, min, max, seed);
    std::vector<float> normal_sequence = normal_generator.create_sequence(amount, mean, sigma, seed);
    std::vector<uint> uniform_buckets = get_histogram_data(uniform_sequence, min, max, size);
    std::vector<uint> normal_buckets = get_histogram_data(normal_sequence, -5.0f, 5.0f, 0.1f);
    std::cout << "Uniform: " << '\n';
    print_vector(uniform_sequence, out);
    print_vector(uniform_buckets, std::cout);
    std::cout << "Normal: " << '\n';
    print_vector(normal_sequence, out);
    print_vector(normal_buckets, std::cout);
    std::vector<my_type> parabolic_distribution(amount);
    std::vector<my_type>::iterator begin = std::begin(uniform_sequence);
    std::vector<my_type>::iterator end = std::end(uniform_sequence);
    std::vector<my_type>::iterator begin_parabolic = std::begin(parabolic_distribution);
    std::transform(begin, end, begin_parabolic, make_parabolic_distribution<my_type>);
    print_vector(parabolic_distribution, out);
    out.close();
    return 0;
}
