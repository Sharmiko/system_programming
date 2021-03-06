#include <time.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/types.h>

#include <iostream>

// size of shared memory
#define N 100

// random numbers range
#define RANDOM_MIN 100
#define RANDOM_MAX 200


/*
 * Custom class for managing shared memory 
 */
class MyShMemory
{
private:
    key_t key;
    /* Helper private function used to get shared
     * memory id. If some kind of error occures 
     * program is termianted.
     */
    int get_shared_memory_id()
    {
        // get shared memory id
        int shm_id = shmget(this->key, N * sizeof(int), IPC_CREAT | IPC_EXCL | 0660);
        if (shm_id == -1) // check error cases
        {
            if (errno == EEXIST) //  if id already exists try to get existing shared memory id.
            {
                shm_id = shmget(this->key, N * sizeof(int), 0);
                if (shm_id == -1)
                {
                    std::cerr << "Error: Could not get identifier for existed shared memory." << std::endl;
                    std::exit(-1);
                }
            }
            else
            {
                std::cerr << "Error: Could not create shared memory." << std::endl;
                std::exit(-1);
            }
        }

        return shm_id;
    }
public:
    /*
     *  Contructor for MyShMemory class. This constructor creates IPC key 
     *  using pathname. In case of error program is terminated
     */
    MyShMemory()
    {
        this->key = ftok("MyShMemory.cpp", 1);
        if (this->key == -1)
        {
            std::cerr << "Error: Could not create key." << std::endl;
            std::exit(-1);
        }
    }

    /*
     * Function that attaches shared memory to calling process and
     * returns pointer to memory space.
     */
    int *get_shared_memory()
    {
        int shm_id = this->get_shared_memory_id();
        int *memory = (int*) shmat(shm_id, NULL, 0);
        if (memory == (int*)(-1))
        {
            std::cerr << "Error: Could not attach shared memory to calling process." << std::endl;
            std::exit(-1);
        }

        return memory;
    }

    /*
     * Function that detaches shared memory form current process. 
     */ 
    void detach_shared_memory(int *memory)
    {
        if (shmdt(memory))
        {
            std::cerr << "Error: Could not detach shared memory." << std::endl;
            std::exit(-1);
        }
    }
};

/*
 * Program 1:
 *  Parent process creates 2 child process and shared memory for 100 
 *  integer values.
 *  First child initializes shared memory with random numbers in range [100, 200].
 *  Second Child using same shared memory calculates how many values are multiples
 *  of 5.
 */
void program_one()
{
    MyShMemory *shm = new MyShMemory();

    decltype(fork()) process_a;
    decltype(fork()) process_b;

    // create 2 child processes
    (process_a = fork()) && (process_b = fork());

    if (process_a == 0)
    {
        // child a
        int *memory = shm->get_shared_memory();

        srand(time(nullptr));
        for (int i = 0; i < N; ++i)
        {
            memory[i] = random() % (RAND_MAX - RANDOM_MIN + 1) + RANDOM_MIN;
        }

        // detach memory
        shm->detach_shared_memory(memory);
    }
    else if (process_b == 0)
    {
        // child b
        int *memory = shm->get_shared_memory();

        // wait untill data is available
        while(!memory[0]);

        // count multiples of 5
        int count = 0;
        for (int i = 0; i < N; ++i)
        {
            if (memory[i] % 5 == 0)
            {
                ++count;
            }
        }

        std::cout << "There are " << count << " multiples of 5." << std::endl;
        shm->detach_shared_memory(memory);
    }
    else
    {
        // parent
        wait(nullptr);
    }
}

int main()
{   

    // execute program one
    std::cout << "Executing program one..." << std::endl;
    program_one();


    // execute program two

    return 0;
}
