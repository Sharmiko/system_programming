#include <fcntl.h>
#include <string.h>
#include <iomanip>
#include <iostream>


class custom_errno {};


std::ostream &operator<<(std::ostream &os, const custom_errno &e)
{
    return os << strerror(errno);
}


struct object_t 
{
    int data1;
    int data2;
};


std::ostream &operator<<(std::ostream &os, const object_t &obj)
{
    os << "data1: " << obj.data1 << "\n";
    os << "data2: " << obj.data2 << "\n";
    return os;
}

std::istream &operator>>(std::istream &is, object_t &obj)
{
    is >> obj.data1;
    is >> obj.data2;
    return is;
}


template<std::size_t N>
class buf_t
{
private:
    char m_buf[N];
public:
    constexpr auto size()
    {
        return N;
    }

    constexpr auto data()
    {
        return m_buf;
    }
};

template<std::size_t N>
std::istream &operator>>(std::istream &is, buf_t<N> &b)
{
    is >> std::setw(b.size()) >> b.data();
    return is;
}


int main()
{

    if (open("filename.txt", O_RDWR) == -1)
    {
        std::cout << custom_errno{} << std::endl;
    }

    object_t obj;

    // std::cin >> obj;
    // std::cout << obj;

    char buf[2];
    scanf("%2s", buf);

    buf_t<2> buf2;
    std::cin >> buf2;

    return 0;
}