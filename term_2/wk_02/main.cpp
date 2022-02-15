#include <iostream>
#include <fstream>
#include <random>
#include <set>
#include <string>
#include <vector>

typedef unsigned int uint;

unsigned int count_factorial(const unsigned int arg)
{
    unsigned int factorial = 1;
    for(unsigned int i = 2; i <= arg; ++ i){
        factorial *= i;
    }
    return factorial;
}

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
        std::uniform_int_distribution<int> distr(min, max);
        std::vector<int> sequence(n);
        for (unsigned int i = 0; i < n; ++ i){
            sequence[i] = distr(engine_);
        }
        return sequence;
    }

private:
    long long seed_;
    std::mt19937_64 engine_;
};

std::set<int> create_set(const unsigned int tests, const int range, UniformRandomGenerator& my_generator)
{
    const int result_max = range - 1;
    std::set<int> result;
    for(unsigned int test = 0; test < tests; ++ test){
        std::vector<int> sequence = my_generator.create_sequence(1, 0, result_max);
        const int random = sequence[0];
        result.insert(random);
    }
    return result;
}

float count_probability(const unsigned int tests, const int range)
{
    unsigned int full = 0;
    const unsigned int global_tests = 10000;
    UniformRandomGenerator my_generator(-1);
    for(unsigned int global_test = 0; global_test < global_tests; ++ global_test){
        std::set<int> result = create_set(tests, range, my_generator);
        const unsigned int size = result.size();
        if (size == range){
            ++ full;
        }
    }
    const float probability = (float)full / global_tests;
    return probability;
}

double solve_task_1(const unsigned tests, const unsigned range)
{
    double sum = 0;
    for(unsigned int i = 1; i <= range; ++ i){
        sum += pow(-1, i + range) * pow(i, tests) / count_factorial(i) / count_factorial(range - i);

    }
    double probability = sum * count_factorial(range) / pow(range, tests);
    return probability;
}

void get_data_for_task_1(const int range)
{
    const unsigned int tests_max = range * 10;
    const std::string path = "result.txt";
    std::ofstream out;
    out.open(path);
    for (unsigned int tests = 1; tests <= tests_max; ++ tests){
        const float probability = count_probability(tests, range);
        const double correct = solve_task_1(tests, range);
        out << tests << ' ' << probability << ' ' << correct << '\n';
    }
    out.close();
}

bool is_equal(const std::vector<unsigned int>& vector1, const std::vector<unsigned int>& vector2)
{
    const unsigned int size_1 = vector1.size();
    const unsigned int size_2 = vector2.size();
    if(size_1 != size_2){
        return false;
    }
    else{
        for(unsigned int i = 0; i < size_1; ++ i){
            const unsigned int value_1 = vector1[i];
            const unsigned  int value_2 = vector2[i];
            if (value_1 != value_2){
                return false;
            }
        }
    }
    return true;
}

void shift(std::vector<unsigned int>& my_vector)
{
    const unsigned int size = my_vector.size();
    const unsigned int shift_last = size - 1;
    for(unsigned int i = 0; i < shift_last; ++ i){
        my_vector[i] = my_vector[i + 1];
    }
}

uint count_attempts(const std::vector<uint>& phrase_need)
{
    unsigned int attempts = 0;
    std::vector<unsigned int> phrase{0, 0, 0};
    UniformRandomGenerator my_generator(-1);
    while(!is_equal(phrase_need, phrase)){
        shift(phrase);
        const std::vector<int> random = my_generator.create_sequence(1, 1, 3);
        const int number = random[0];
        phrase[2] = number;
        ++ attempts;
    }
    return attempts;
}

void get_data_for_task_2(const std::vector<unsigned int>& phrase_need)
{
    uint tests = 100000;
    uint symbols = 0;
    for (uint i = 0; i < tests; ++ i){
        symbols += count_attempts(phrase_need);
    }
    const float average = (float)symbols / (float)tests;
    std::cout << "Average: " << average;
}

int main() {
    std::vector<unsigned int> phrase_need{1, 2, 3};
    const unsigned int range = 5;
    get_data_for_task_2(phrase_need);
    return 0;
}
