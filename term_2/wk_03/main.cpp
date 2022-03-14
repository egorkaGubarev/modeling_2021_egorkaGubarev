#include <array>
#include <cmath>
#include <iostream>
#include <fstream>
#include <random>
#include <string>
#include <vector>

typedef unsigned int uint;

class UniformRandomGenerator
{
public:
    explicit UniformRandomGenerator(long long seed): seed_{seed}
    {
        if (seed == -1){
            seed_ = time(nullptr);
        }
        engine_.seed(seed_);
    }

    std::vector<int> create_sequence(const unsigned int n, const int min, const int max)
    {
        std::vector<int> sequence(n);
        for (unsigned int i = 0; i < n; ++ i){
            sequence[i] = get_number(min, max);
        }
        return sequence;
    }

    std::vector<float> create_float_sequence(const unsigned int n, const float min, const float max)
    {
        std::vector<float> sequence(n);
        for (unsigned int i = 0; i < n; ++ i){
            sequence[i] = get_float_number(min, max);
        }
        return sequence;
    }

    int get_number(const int min, const int max)
    {
        std::uniform_int_distribution<int> distr(min, max);
        const int number = distr(engine_);
        return number;
    }

    float get_float_number(const float min, const float max)
    {
        std::uniform_real_distribution<float> distr(min, max);
        const float number = distr(engine_);
        return number;
    }

private:
    long long seed_;
    std::mt19937_64 engine_;
};

std::vector<float> get_random_wandering(const uint steps, UniformRandomGenerator& generator)
{
    const int min = -1;
    const int max = 1;
    const uint delta = max - min;
    std::vector<float> sequence = generator.create_float_sequence(steps, 0, delta);
    for(float& number: sequence){
        number += min;
    }
    return sequence;
}

std::array<float, 3> count_average_wandering(const uint steps, const uint tests,
                                                   UniformRandomGenerator& generator)
{
    float sum = 0;
    float positive = 0;
    float square = 0;
    std::array<float, 3> result{};
    for (uint test = 0; test < tests; ++ test){
        float final_point = 0;
        const std::vector<float> wandering = get_random_wandering(steps, generator);
        for(const float delta: wandering) {
            final_point += delta;
        }
        sum += final_point;
        positive += std::abs(final_point);
        square += (float)std::pow(final_point, 2);
    }
    const float average = (float)sum / (float)tests;
    const float average_abs = (float)positive / (float)tests;
    const float average_square = (float)square / (float)tests;

    result[0] = average;
    result[1] = average_abs;
    result[2] = average_square;
    return result;
}

void collect_data(const uint steps_max, const uint tests, UniformRandomGenerator& generator,
                  const std::string& path)
{
    std::ofstream out;
    out.open(path);
    for(uint steps = 1; steps < steps_max; ++ steps){
        std::array<float, 3> data = count_average_wandering(steps, tests, generator);
        const float average = data[0];
        const float average_abs = data[1];
        const float average_square = data[2];
        out << steps << ' ' << average << ' ' << average_abs << ' ' << average_square << '\n';
    }
    out.close();
}

int main() {
    const uint steps_max = 100;
    const uint tests_1 = 10000;
    const uint tests_2 = 1000;
    const int seed = -1;
    const std::string path_1 = "result_1.txt";
    const std::string path_2 = "result_2.txt";
    UniformRandomGenerator generator(seed);
    collect_data(steps_max, tests_1, generator, path_1);
    collect_data(steps_max, tests_2, generator, path_2);
    return 0;
}
