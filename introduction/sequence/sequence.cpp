#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

typedef unsigned int uint;
typedef unsigned long long int ullint;

std::vector<ullint> get_fibonacci(uint amount)
{
    std::vector<ullint>sequence;
    uint start = 1;
    uint amount_start_max = 2;
    uint amount_start = std::min(amount, amount_start_max);
    for (uint number = 0; number < amount_start; number ++){
        sequence.push_back(start);
    }
    for (uint number = amount_start_max; number < amount; number ++){
        ullint value = sequence[number - 2] + sequence[number - 1];
        sequence.push_back(value);
    }
    return sequence;
}

void write_file(std::string path, std::vector<ullint>data)
{
    uint amount_spaces = 5;
    std::ofstream file;
    file.open(path, std::ios::trunc);
    if (file.is_open()){
        for (uint number = 0; number < data.size(); number ++){
            uint number_from_1 = number + 1;
            file << number_from_1;
            for (uint space = 0; space < amount_spaces; space ++){
                file << ' ';
            }
            file << data[number] << '\n';
        }
        file.close();
        std::cout << "File is created!" << '\n';
    }else{
        std::cerr << "File isn't opened!" << '\n';
    }
}

int main(int argc, char** argv)
{
    const uint amount_max = 93;
    const uint amount_min = 1;
    const uint argc_need = 1;
    const uint code_error = -1;
    const std::string path = "file.txt";
    if (argc != argc_need){
        std::cerr << "Error! Wrong amount of arguments in main(). Program is aborted!" << '\n';
        return code_error;
    }
    std::cout << "Enter amount of numbers in sequence -> ";
    uint amount;
    std::cin >> amount;
    if (amount < amount_min){
        std::cerr << "Error! Amount of numbers must be non negative. Program is aborted!" << '\n';
        return code_error;
    }
    if (amount > amount_max){
        std::cerr << "Error! Amount of numbers must be less then 93. Program is aborted!" << '\n';
        return code_error;
    }
    std::vector<ullint>sequence = get_fibonacci(amount);
    write_file(path, sequence);
    return 0;
}
