#include <memory>
#include <thread>
#include <iostream>


// global memory
int bss_mem = 0;

class myclass
{
private:
    int m_data{0};
public:
    ~myclass()
    {
        std::cout << "my delete\n";
    }

    void inc()
    {
        this->m_data++;
    }
};

    
std::thread t1;
std::thread t2;

void thread2(const std::shared_ptr<myclass> ptr)
{
    for (auto i = 0; i < 100000; ++i)
    {
        ptr->inc();
    }

    std::cout << "Thread2: complete\n";
}

void thread1()
{
    auto ptr = std::make_shared<myclass>();
    t2 = std::thread(thread2, ptr);

    for (auto i = 0; i < 10; ++i)
    {
        ptr->inc();
    }

    std::cout << "thread1: complete\n";
}

using aligned_int alignas(0x1000) = int;


void *operator new(std::size_t count)
{
    // WARNING: Do not use std::cout here
    return malloc(count);
}

void operator delete(void *ptr)
{
    // WARNING: Do not use std::cout here
    return free(ptr);
}


class int_deleter
{
public:
    void operator()(int *ptr) const 
    {
        std::cout << "my delete\n";
        delete ptr;
    }
};


int main()
{

    auto flags = std::cout.flags();
    // stack memory
    int stack_mem = 42;

    // exceeds total size of stack
    //int stack_mem2[2412412412421];

    // heap memory (dynamic memory)
    auto ptr = new int;
    delete ptr;

    auto ptr2 = new int[42];
    delete[] ptr2;

    auto ptr3 = new myclass[2];
    delete[] ptr3;

    alignas(0x1000) int ptr4[42];
    std::cout << ptr4 << "\n";


    char buffer[0x2000];
    auto ptr5 = reinterpret_cast<uintptr_t>(buffer);
    auto ptr6 = ptr5 - (ptr5 % 0x1000) + 0x1000;
    std::cout << std::hex << std::showbase;
    std::cout << ptr5 << "\n";
    std::cout << ptr6 << "\n";


    int *ptr7;
    if (posix_memalign(reinterpret_cast<void **>(&ptr), 0x1000, 42 * sizeof(int)))
    {
        std::clog << "ERROR: unable to allocate alligned memory\n";
        std::exit(EXIT_FAILURE);
    }

    std::cout << ptr7 << "\n";


    if (auto ptr8 = aligned_alloc(0x1000, 42 * sizeof(int)))
    {
        std::cout << ptr8 << "\n";
        std::free(ptr8);
    }

    auto ptr9 = new aligned_int[42];
    std::cout << ptr9 << "\n";
    delete[] ptr9;

    auto ptr10 = new (std::nothrow) int;
    std::cout << ptr10 << "\n";
    delete ptr10;


    // placement of new
    char buf[0x1000];
    auto ptr11 = new (buf) int;
    std::cout << ptr11 << "\n";

    std::cout.flags(flags);

    
    // smart pointers
    auto ptr12 = std::make_unique<int>(42);    
    std::cout << *ptr12 << "\n";

    auto ptr13 = std::unique_ptr<int, int_deleter>(new int, int_deleter());
    std::cout << ptr13.get() << "\n";

    
    // shared pointers


    t1 = std::thread(thread1);

    t1.join();
    t2.join();


    return 0;
}