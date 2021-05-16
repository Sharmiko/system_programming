#include <iostream>
#include <fstream>


int main()
{   
    
    constexpr auto mode = std::ios::in | std::ios::out | std::ios::trunc;

    if (auto file = std::fstream("test.txt", mode))
    {
        std::cout << "success\n";
    }

    return 0;
}