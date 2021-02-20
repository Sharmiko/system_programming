#include <unistd.h>
#include <iostream>
#include <sys/shm.h>
#include <sys/wait.h>

#include <array>
#include <string_view>


char *get_shared_memory()
{
    auto key = ftok("myfile", 42);
    auto shm = shmget(key, 0x1000, 0666 | IPC_CREAT);

    return static_cast<char *>(shmat(shm, nullptr, 0));
}

void mysystem(const char *command)
{
    if (fork() == 0)
    {
        execlp(command, command, nullptr);
    }
    else{
        wait(nullptr);
    }
}


int data = 0;

class mypipe {
private:
    std::array<int, 2> m_handles;
public:
    mypipe()
    {
        if (pipe(m_handles.data()) < 0)
        {
            std::exit(-1);
        }
    }

    ~mypipe()
    {
        close(m_handles.at(0));
        close(m_handles.at(1));
    }

    std::string read()
    {
        std::array<char, 256> buf;
        std::size_t bytes = ::read(m_handles.at(0), buf.data(), buf.size());

        if (bytes > 0)
        {
            return {buf.data(), bytes};
        }

        return {};
    }

    void write(const std::string &msg)
    {
        ::write(m_handles.at(1), msg.data(), msg.size());
    }

    void redirect()
    {
        dup2(m_handles.at(1), STDOUT_FILENO);
        close(m_handles.at(0));
        close(m_handles.at(1));
    }
};

int main(void)
{
    mypipe p;

    if(fork() != 0)
    {
        sleep(1);
        std::cout << "Parent\n";
        //auto msg = get_shared_memory();
        //msg[0] = 42;
        //p.write("done");


        wait(nullptr);
        std::cout << p.read() << std::endl;
    }
    else
    {
        //auto msg = p.read();
        //auto msg1 = get_shared_memory();
        //while (msg1[0] != 42);

        p.redirect();
        execlp("ls", "ls", nullptr);

        std::cout << "child\n";
        //std::cout << "msg: " << msg1 << std::endl;
    }

    
    //execl("/bin/ls", "ls", nullptr);
    std::cout << "Hello WWorld\n";

}