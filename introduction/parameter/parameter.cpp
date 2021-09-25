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
    uint n;
    std::cout << "Enter n -> ";
    std::cin >> n;
    for (uint number = 1; number < 31; number ++){
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
