#include <iostream>
#include <deque>
#include <vector>
#include <list>


template<typename T>
class myallocator
{
public:
    using value_type = T;
    using pointer = T *;
    using size_type = std::size_t;
    using is_always_equal = std::true_type;

public:

    myallocator()
    {
        std::cout << this << " constructor, sizeof(T): " << sizeof(T) << '\n';
    }

    myallocator(myallocator &&other) noexcept
    {
        (void) other;
        std::cout << this << " move constructor, sizeof(T): " << sizeof(T) << '\n';
    }

    template<typename U>
    myallocator(const myallocator<U> &other) noexcept
    {
        (void) other;
    }

    pointer allocate(size_type n)
    {
        if (auto ptr = static_cast<pointer>(malloc(sizeof(T) * n)))
        {
            std::cout << this << " A [" << n << "]: " << ptr << '\n';
            return ptr;
        }

        throw std::bad_alloc();
    }

    void deallocate(pointer p, size_type n)
    {
        (void) n;
        std::cout << this << " D [" << n << "]: " << p << '\n';
        return free(p);
    }


    myallocator &operator=(myallocator &&other) noexcept
    {
        (void) other;
        std::cout << this << " move assigment, sizeof(T): " << sizeof(T) << '\n';
        return *this;
    }

    myallocator(const myallocator &other) noexcept
    {
        (void) other;
        std::cout << this << " copy constructor, sizeof(T): " << sizeof(T) << '\n';
    }

    myallocator &operator=(const myallocator *other) noexcept
    {
        (void) other;
        std::cout << this << " copy asignment, sizeof(T): " << sizeof(T) << '\n';
        return *this;
    }
};



int main()
{

    std::list<int, myallocator<int>> mylist;
    mylist.emplace_back(42);

    std::vector<int, myallocator<int>> myvector;
    myvector.emplace_back(42);
    myvector.emplace_back(42);
    myvector.emplace_back(42);

    std::deque<int, myallocator<int>> mydeque;
    mydeque.emplace_back(42);
    mydeque.emplace_back(42);
    mydeque.emplace_back(42);



    return 0;
}