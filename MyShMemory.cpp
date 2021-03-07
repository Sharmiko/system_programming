#include <time.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/types.h>

#include <iostream>


/*
 * Custom class for managing shared memory 
 */
class MyShMemory
{
private:
    key_t key;
    int memory_size;
    /* Helper private function used to get shared
     * memory id. If some kind of error occures 
     * program is termianted.
     */
    int get_shared_memory_id()
    {
        // get shared memory id
        int shm_id = shmget(this->key, this->memory_size, IPC_CREAT | IPC_EXCL | 0660);
        if (shm_id == -1) // check error cases
        {
            if (errno == EEXIST) //  if id already exists try to get existing shared memory id.
            {
                shm_id = shmget(this->key, this->memory_size, 0);
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
    MyShMemory(int memory_size)
    {
        this->memory_size = memory_size;
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
    void *get_shared_memory()
    {
        int shm_id = this->get_shared_memory_id();
        void *memory = (void*) shmat(shm_id, NULL, 0);
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
    void detach_shared_memory(void *memory)
    {
        if (shmdt(memory))
        {
            std::cerr << "Error: Could not detach shared memory." << std::endl;
            std::exit(-1);
        }
    }
};
