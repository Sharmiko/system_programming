#include <memory>
#include <thread>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <sys/mman.h>


constexpr auto PROT_RW = PROT_READ | PROT_WRITE;
constexpr auto PROT_RE = PROT_READ | PROT_EXEC;
constexpr auto MAP_ALLOC = MAP_PRIVATE | MAP_ANONYMOUS;


auto name = "/shm";


class mmap_deleter
{
private:
    std::size_t m_size;
public:
    mmap_deleter(std::size_t size) : m_size{size} {}

    void operator()(int *ptr)
    {
        munmap(ptr, m_size);
    }
};

template<typename T, typename... Args>
auto mmap_unique(Args&&... args)
{
    if (auto ptr = mmap(0, sizeof(T), PROT_RW, MAP_ALLOC, -1, 0))
    {
        auto obj = new (ptr) T(args...);
        auto del = mmap_deleter(sizeof(T));
        return std::unique_ptr<T, mmap_deleter>(obj, del);
    }

    throw std::bad_alloc();
}

template<typename T, typename... Args>
auto mmap_unique_server(Args&&... args)
{
    if (int fd = shm_open(name, O_CREAT | O_RDWR, 0644); fd != -1)
    {
        ftruncate(fd, sizeof(T));
        if (auto ptr = mmap(0, sizeof(T), PROT_RW, MAP_SHARED, fd, 0))
        {
            auto obj = new (ptr) T(args...);
            auto del = mmap_deleter(sizeof(T));

            return std::unique_ptr<T, mmap_deleter>(obj, del);
        }
    }

    throw std::bad_alloc();
}

template<typename T>
auto mmap_unique_client()
{
    if (int fd = shm_open(name, O_RDWR, 0644); fd != -1)
    {
        ftruncate(fd, sizeof(T));

        if (auto ptr = mmap(0, sizeof(T), PROT_RW, MAP_SHARED, fd, 0))
        {
            auto obj = static_cast<T*>(ptr);
            auto del = mmap_deleter(sizeof(T));

            return std::unique_ptr<T, mmap_deleter>(obj, del);
        }
    }

    throw std::bad_alloc();
}

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


    //t1 = std::thread(thread1);

    //t1.join();
    //t2.join();


    // Mappings and Permissions

    auto ptr14 = mmap(0, 0x1000, PROT_RW, MAP_ALLOC, -1, 0);
    std::cout << "ptr14: " << ptr14 << "\n";

    auto ptr15 = mmap(0, 42, PROT_RW, MAP_ALLOC, -1, 0);
    auto ptr16 = mmap(0, 42, PROT_RW, MAP_ALLOC, -1, 0);

    std::cout << ptr15 << "\n";
    std::cout << ptr16 << "\n";

    munmap(ptr15, 42);
    munmap(ptr16, 42);

    auto ptr17 = mmap(0, 0x1000, PROT_RW, MAP_ALLOC, -1, 0);
    std::cout << ptr17 << "\n";

    if (mprotect(ptr17, 0x1000, PROT_READ) == -1)
    {
        std::clog << "ERROR: Failed to change memory permissions\n";
        std::exit(EXIT_FAILURE);
    }

    munmap(ptr17, 0x1000);

    auto ptr18 = mmap_unique<int>(42);
    std::cout << *ptr18 << "\n";


    auto ptr19 = mmap_unique_server<int>(42);
    auto ptr20 = mmap_unique_client<int>();
    std::cout << *ptr19 << "\n";
    std::cout << *ptr20 << "\n";

    return 0;
}