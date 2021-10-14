#include <fstream>
#include <string>
#include <vector>

typedef unsigned int uint;

void clear_file(const std::string path)
{
    std::ofstream file;
    file.open(path, std::ios::trunc);
    file.close();
}

std::vector<std::vector<float>> solve_simple_task(const float a_start, const float b_start, const float time_end, const float time_step, const uint k=1)
{
    std::vector<float> a, b;
    const uint amount_iterations = time_end / time_step;
    a.resize(amount_iterations);
    b.resize(amount_iterations);
    a[0] = a_start;
    b[0] = b_start;
    for (uint iteration = 1; iteration < amount_iterations; ++ iteration){
        a[iteration] = a[iteration - 1] + b[iteration - 1] * time_step;
        b[iteration] = b[iteration - 1] - a[iteration - 1] * k * time_step;
    }
    std::vector<std::vector<float>> solution = {a, b};
    return solution;
}

void write_vector_to_file(const std::vector<float> vector_1, const std::string path)
{
    std::ofstream file;
    file.open(path, std::ios::app);
    for (float value: vector_1){
        file << value << ";";
    }
    file << '\n';
    file.close();
}

int main(int argc, char** argv)
{
    const float a_start = 6371000;
    const float b_start = 0;
    const float time_end = 100;
    const float time_step = 1;
    const std::string path = "result.txt";
    const std::vector<std::vector<float>> solution = solve_simple_task(a_start, b_start, time_end, time_step);
    const std::vector<float> a = solution[0];
    const std::vector<float> b = solution[1];
    clear_file(path);
    write_vector_to_file(a, path);
    write_vector_to_file(b, path);
    return 0;
}
