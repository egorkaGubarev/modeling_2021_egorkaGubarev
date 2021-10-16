#include <fstream>
#include <iostream>
#include <math.h>
#include <string>
#include <vector>

typedef unsigned int uint;

void clear_file(const std::string path)
{
    std::ofstream file;
    file.open(path, std::ios::trunc);
    file.close();
}

std::vector<std::vector<std::vector<float>>> solve_simple_task(const float a_start, const float b_start, const float time_end,
                                                               const std::vector<float> time_steps,
                                                               const uint k = 1)
{
    uint time_step_number = 0;
    uint time_steps_amount = time_steps.size();
    std::vector<std::vector<std::vector<float>>> solution(time_steps_amount);
    for (float time_step: time_steps){
        std::vector<float> a, b, energy, time;
        const uint amount_iterations = time_end / time_step;
        a.resize(amount_iterations);
        b.resize(amount_iterations);
        energy.resize(amount_iterations);
        time.resize(amount_iterations);
        a[0] = a_start;
        b[0] = b_start;
        float kinetic_energy = std::pow(b_start, 2) / 2;
        float potential_energy = k * std::pow(a_start, 2) / 2;
        energy[0] = kinetic_energy + potential_energy;
        time[0] = 0;
        for (uint iteration = 1; iteration < amount_iterations; ++ iteration){
            a[iteration] = a[iteration - 1] + b[iteration - 1] * time_step;
            b[iteration] = b[iteration - 1] - a[iteration - 1] * k * time_step;
            kinetic_energy = std::pow(b[iteration], 2) / 2;
            potential_energy = k * std::pow(a[iteration], 2) / 2;
            energy[iteration] = kinetic_energy + potential_energy;
            time[iteration] = time_step * iteration;
        }
        solution[time_step_number] = {a, b, energy, time};
        std::cout << "Solution with time step " << time_step << " is founded!" << '\n';
        ++ time_step_number;
    }
    return solution;
}

void write_vector_to_file(const std::vector<float> data, const std::string path, const uint period)
{
    std::ofstream file;
    file.open(path, std::ios::app);
    uint size = data.size();
    for (uint index = 0; index < size; ++ index){
        uint mod = index % period;
        if (mod == 0){
            const float value = data[index];
            file << value << ";";
        }
    }
    file << '\n';
    file.close();
}

int main(int argc, char** argv)
{
    const uint numbers_to_write = 10;
    const float a_start = 6371000;
    const float b_start = 0;
    const float time_end = 1;
    const std::vector<float> time_steps = {0.1, 0.01, 0.001, 0.0001, 0.00001, 0.000001, 0.0000001, 0.00000001};
    const std::string path = "result.txt";
    const std::vector<std::vector<std::vector<float>>> solution = solve_simple_task(a_start, b_start, time_end, time_steps);
    uint time_steps_amount = time_steps.size();
    clear_file(path);
    for (uint time_step_number = 0; time_step_number < time_steps_amount; ++ time_step_number)
    {
        const std::vector<float> a = solution[time_step_number][0];
        const std::vector<float> b = solution[time_step_number][1];
        const std::vector<float> energy = solution[time_step_number][2];
        const std::vector<float> time = solution[time_step_number][3];
        const std::vector<float> time_step = {time_steps[time_step_number]};
        uint numbers_amount = time_end / time_steps[time_step_number];
        uint period = numbers_amount / numbers_to_write;
        write_vector_to_file(a, path, period);
        write_vector_to_file(b, path, period);
        write_vector_to_file(energy, path, period);
        write_vector_to_file(time, path, period);
        write_vector_to_file(time_step, path, 1);
        std::cout << "Data set number " << time_step_number << " is written!" << '\n';
    }
    return 0;
}
