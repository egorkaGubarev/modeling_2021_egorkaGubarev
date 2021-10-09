#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

typedef unsigned int uint;

void clear_file(std::string path)
{
    std::ofstream file;
    file.open(path, std::ios::trunc);
    file.close();
}

void write_file(std::string path, uint number)
{
    std::ofstream file;
    file.open(path, std::ios::app);
    if (file.is_open()){
        file << number << ' ';
        file.close();
    }else{
        std::cerr << "File isn't opened!";
    }
}

int main(int argc, char** argv)
{
    uint n;
    uint number_max = 30;
    std::string path = "file.txt";
    if (argc != 2){
        std::cerr << "Error! Wrong amount of arguments in main(). Program is aborted!" << '\n';
        return -1;
    }
    std::istringstream string_input(argv[1]);
    if (!(string_input >> n)){
        std::cerr << "Error! Invalid number: " << argv[1] << ". Program is aborted!" << '\n';
        return -1;
    }
    if (!(string_input.eof())){
        std::cerr << "Error! Wrong characters after number. Program is aborted!" << '\n';
        return -1;
    }
    if (n < 0 || n > number_max){
        std::cerr << "Error! Number must be from 0 to 30. Program is aborted!" << '\n';
        return -1;
    }
    clear_file(path);
    for (uint number = 1; number <= number_max; number ++){
        if (number <= n){
            write_file(path, number);
        }else{
            std::cout << number << ' ';
        }
    }
    std::cout << '\n';
    std::cout << "File is created";
    return 0;
}
