#include <iostream>
#include <fstream>
#include <string>

typedef unsigned int uint;

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

int main()
{
    std::string path = "file.txt";
    for (uint number = 1; number < 31; number ++){
        write_file(path, number);
    }
    std::cout << "File is created";
    return 0;
}

