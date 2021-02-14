#include <fstream>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>



class MyPipe {
public:
    int input;
    int output;

    MyPipe()
    {
        int temp[2];
        if (pipe(temp) == -1)
        {
            std::cerr << "Could not create pipe." << std::endl;
            std::exit(-1);
        }

        input = temp[0];
        output = temp[1];
    }
};


