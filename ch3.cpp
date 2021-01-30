#include <iostream>
#include <limits>
#include <cstddef>


#pragma pack(push, 1)
struct mystruct
{
    uint64_t data1;
    uint16_t data2;
};
#pragma pack(pop)


int main(void)
{
    const char *str = "Hello World\n";

    std::cout << str << std::endl;


    auto num_bytes_signed = sizeof(signed char);
    auto min_signed = std::numeric_limits<signed char>().min();
    auto max_signed = std::numeric_limits<signed char>().max();

    auto num_bytes_unsigned = sizeof(unsigned char);
    auto min_unsigned = std::numeric_limits<unsigned char>().min();
    auto max_unsigned = std::numeric_limits<unsigned char>().max();

    std::cout << "Char:" << std::endl;

    std::cout << "\tNum bytes (signed): " << num_bytes_signed << std::endl;
    std::cout << "\tMin value (signed): " << +min_signed << std::endl;
    std::cout << "\tMax value (signed): " << +max_signed << std::endl;

    std::cout << std::endl;

    std::cout << "\tNum bytes (unsigned): " << num_bytes_unsigned << std::endl;
    std::cout << "\tMin value (unsigned): " << +min_unsigned << std::endl;
    std::cout << "\tMax value (unsigned): " << +max_unsigned << std::endl;
    std::cout << std::endl;

    std::cout << "Size of mystruct: " << sizeof(mystruct) << std::endl;
}