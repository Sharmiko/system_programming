#include <iostream>
#include <fstream>


struct myclass
{
    std::string hello;
    std::string world;
};


std::fstream &operator>>(std::fstream &is, myclass &obj)
{
    is >> obj.hello;
    is >> obj.world;

    return is;
}

std::ostream &operator<<(std::ostream &os, const myclass &obj)
{
    os << obj.hello;
    os << ' ';
    os << obj.world;

    return os;
}


template<typename T, std::size_t N>
void myread(std::fstream &file, T (&str)[N], std::size_t count)
{
    if (count >= N)
    {
        throw std::out_of_range("file.read out of bounds");
    }

    file.read(static_cast<char *>(str), count);
}

int main()
{   
    
    constexpr auto mode = std::ios::in | std::ios::out;

    if (auto file = std::fstream("test.txt", mode))
    {
        std::string hello;
        std::string world;

        file >> hello >> world;
        std::cout << hello << " " << world << '\n';
    }

    if (auto file  = std::fstream("test.txt"))
    {
        myclass obj;
        file >> obj;
        std::cout << obj << '\n';
    }

    if (auto file = std::fstream("test.txt"))
    {
        char c = file.get();
        std::cout << c << '\n';
    }


    if (auto file = std::fstream("test.txt"))
    {
        char buf[25] = {};
        file.read(buf, 11);
        std::cout << buf << '\n';
    }


    if (auto file = std::fstream("test.txt"))
    {
        char buf[25] = {};
        myread(file, buf, 11);
        std::cout << buf << '\n';
    }

    if (auto file = std::fstream("test.txt"))
    {
        std::cout << file.tellg() << '\n';
        char c = file.get();
        std::cout << file.tellg() << '\n';
        c = file.peek();
        std::cout << file.tellg() << '\n';
    }


    if (auto file = std::fstream("test.txt"))
    {
        std::cout << file.tellg() << '\n';
        file.ignore(1);
        std::cout << file.tellg() << '\n';
    }


    if (auto file = std::fstream("test.txt"))
    {
        std::string hello;
        std::string world;

        file >> hello >> world;
        std::cout << hello << " " << world << '\n';

        file.seekg(1);

        file >> hello >> world;
        std::cout << hello << " " << world << '\n';
    }


    if (auto file = std::fstream("test.txt"))
    {
        char buf[25] = {};
        file.getline(buf, 25, '\n');
        std::cout << buf << '\n';
    }


    if (auto file = std::fstream("test.txt"))
    {
        std::string str;
        std::getline(file, str);
        std::cout << str << '\n';
    }


    return 0;
}