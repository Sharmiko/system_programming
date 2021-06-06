#include <string.h>

#include <iostream>
#include <fstream>
#include <cstring>


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


std::fstream &operator<<(std::fstream &os, const myclass &obj)
{
    os << obj.hello;
    os << ' ';
    os << obj.world;

    return os;
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


void mywrite(std::fstream &file, const char *str, std::size_t count)
{
    if (count > std::strlen(str))
    {
        std::cerr << count << " " << std::strlen(str) << '\n';
        throw std::out_of_range("file.write out of boounds");
    }

    file.write(str, count);
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


    // writing to a file 


    if (auto file = std::fstream("test.txt"))
    {
        std::string hello = {"Hello"};
        std::string world = {"World"};
        file << hello << " " << world << " " << 42 << '\n';
    }


    if (auto file = std::fstream("test.txt"))
    {
        myclass obj;
        obj.hello = "Hey";
        obj.world = "World";
        file << obj << '\n';
    }


    if (auto file = std::fstream("test.txt"))
    {
        file.put('H');
        file.put('\n');
        file.write("hello world\n", 12);
    }


    if (auto file = std::fstream("test.txt"))
    {
        //mywrite(file, "Hello World\n", 100);
        const char str1[6] = {'H', 'e', 'l', 'l', 'o', '\n'};
        const char str2[6] = {'#', '#', '#', '#', '#', '\n'};

        mywrite(file, str1, 12);
        mywrite(file, str2, 6);
    }


    if (auto file = std::fstream("test.txt"))
    {
        std::cout << file.tellp() << '\n';
        file << "Hello";
        std::cout << file.tellp() << '\n';
        file << ' ';
        std::cout << file.tellp() << '\n';
        file.seekp(0);
        file << "World";
        std::cout << file.tellp() << '\n';
        file << '\n';
        std::cout << file.tellp() << '\n';
    }

    return 0;
}