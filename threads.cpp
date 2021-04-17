#include <array>
#include <iostream>
#include <unistd.h>
#include <pthread.h>


int count = 0;
pthread_mutex_t lock;
pthread_mutexattr_t attr;

bool predicate = false;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER;


void *my_thread(void *ptr)
{
    std::cout << "thread id: " << pthread_self() << std::endl;
    (*reinterpret_cast<int *>(ptr))++;
    return ptr;
}


void *my_thread2(void *ptr)
{
    while (true)
    {
        std::clog << static_cast<char *>(ptr) << std::endl;
        sleep(1);
        pthread_yield();
    }
}


void *my_thread3(void *ptr)
{
    pthread_mutex_lock(&lock);
    count++;
    pthread_mutex_unlock(&lock);
}


void *my_thread4(void *ptr)
{
    pthread_mutex_lock(&lock);
    pthread_mutex_lock(&lock);
    pthread_mutex_lock(&lock);
    count++;
    pthread_mutex_unlock(&lock);
    pthread_mutex_unlock(&lock);
    pthread_mutex_unlock(&lock);
}


void *my_thread5(void *ptr)
{
    pthread_mutex_lock(&lock);
    std::cout << "Hello World: 1\n";
    predicate = true;
    pthread_mutex_unlock(&lock);
    pthread_cond_signal(&cond);


    return nullptr;
}

void *my_thread6(void *ptr)
{
    pthread_mutex_lock(&lock);
    while (!predicate)
    {
        pthread_cond_wait(&cond, &lock);
    }
    std::cout << "Hello World: 2\n";
    predicate = true;
    pthread_mutex_unlock(&lock);

    return nullptr;
}


int main()
{
    int in_value = 42;
    void *out_value = nullptr;

    pthread_t thread1;
    pthread_t thread2;


    pthread_create(&thread1, nullptr, my_thread, &in_value);
    pthread_create(&thread2, nullptr, my_thread, &in_value);
     
    pthread_join(thread1, &out_value);
    pthread_join(thread2, &out_value);


    std::cout << "value: " << *reinterpret_cast<int *>(out_value) << std::endl;


    // char name1[9] = "thread 1";
    // char name2[9] = "thread 2";

    // pthread_create(&thread1, nullptr, my_thread2, name1);
    // pthread_create(&thread2, nullptr, my_thread2, name2);
     
    // pthread_join(thread1, nullptr);
    // pthread_join(thread2, nullptr);


    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&lock, &attr);

    while (true)
    {
        count = 0;
        for (auto i = 0; i < 1000; ++i)
        {
            std::array<pthread_t, 8> threads;

            for (auto &t : threads)
            {
                pthread_create(&t, nullptr, my_thread4, nullptr);
            }

            for (auto &t : threads)
            {
                pthread_join(t, nullptr);
            }
        }

        std::cout << "count: " << count << std::endl;

        break;
    }


    pthread_create(&thread1, nullptr, my_thread5, nullptr);
    pthread_create(&thread1, nullptr, my_thread6, nullptr);

    
    pthread_join(thread1, nullptr);
    pthread_join(thread2, nullptr);


    return 0;
}