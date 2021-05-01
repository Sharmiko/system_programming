#include <fcntl.h>
#include <string.h>
#include <iomanip>
#include <iostream>


#ifndef DEBUG_LEVEL 
#define DEBUG_LEVEL 0
#endif

#ifndef NDEBUG
#define DEBUG(fmt, args...) \
    fprintf(stdout, "%s [%d]: " fmt, __FILE__, __LINE__, args);
#else 
#define DEBUG(...)
#endif 


#ifdef DEBUG_LEVEL
constexpr auto g_debug_level = DEBUG_LEVEL;
#else 
constexpr auto g_debug_level = 0;
#endif 

#ifdef NDEBUG
constexpr auto g_ndebug = true;
#else 
constexpr auto g_ndebug = false;
#endif


#define console std::cout << __FILE__ << " [" << __LINE__ << "]: "

template<std::size_t LEVEL>
const void debug(void(*func)())
{
    if constexpr (!g_ndebug && (LEVEL <= g_debug_level))
    {
        std::cout << "\033[1;32mDEBUG\033[0m ";
        func();
    };
}

template<std::size_t LEVEL>
const void warning(void(*func)())
{
    if constexpr (!g_ndebug && (LEVEL <= g_debug_level))
    {
        std::cout << "\033[1;33mWARNING\033[0m ";
        func();
    };
}

template<std::size_t LEVEL>
const void fatal(void(*func)())
{
    if constexpr (!g_ndebug && (LEVEL <= g_debug_level))
    {
        std::cout << "\033[1;31mFATAL ERROR\033[0m ";
        func();
        std::exit(-1);
    };
}

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

    DEBUG("The answer is: %d\n", 42);
    DEBUG("The answer is: %d\n", 43);

    if constexpr (!g_ndebug && (0 <= g_debug_level))
    {
        std::cout << "The answer is: " << 44 << "\n";
    }

        if constexpr (!g_ndebug && (1 <= g_debug_level))
    {
        std::cout << "The answer is: " << 45 << "\n";
    }

    debug<0>([] {
        console << "The answer is: " << 46 << "\n";
    });

    debug<1>([] {
        console << "The answer is: " << 47 << "\n";
    });

    warning<0>([] {
        console << "The answer might be: " << 47 << "\n";
    });

    fatal<0>([] {
        console << "The answer was not: " << 48 << "\n";
    });

    return 0;
}