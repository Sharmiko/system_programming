#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <iostream>


bool loop = true;


void handler(int signal)
{
    if (signal == SIGINT)
    {
        std::cout << "Handler called" << std::endl;
        ::loop = false;
    }
}


void my_system(const char *command)
{
    if (auto id = fork(); id > 0)
    {
        sleep(2);
        kill(id, SIGINT);
    }
    else 
    {
        execlp(command, command, nullptr);
    }
}


int main(void)
{

    signal(SIGINT, handler);

    while (::loop)
    {
        std::cout << "Hello World" << std::endl;
        sleep(1);
    }

    my_system("b.out");

    return 0;
}