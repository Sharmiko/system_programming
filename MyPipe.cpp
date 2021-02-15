#include <fstream>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

#define INPUT_SIZE 5
#define INPUT_FILE "input.txt"
#define OUTPUT_FILE "output.txt"


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

    ~MyPipe()
    {
        close(input);
        close(output);
    }

    int readData(int &buff)
    {
        auto bytes = ::read(this->input, &buff, INPUT_SIZE * sizeof(int)) / sizeof(int);
        return (int) bytes;
    }

    int writeData(int &buff)
    {
	    auto size = ::write(this->output, &buff, INPUT_SIZE * sizeof(int)) / sizeof(int);
	    return (int) size;
    }
};


void get_user_input(std::string message_prompt, short &choice)
{
    /*
     *  Helper function for displaying message prompot to user 
     *  and getting input from 
     */
    std::cout << message_prompt << std::endl;
    std::cout << "#: ";
    std::cin >> choice;

    if (choice != 1 && choice != 2)
    {
        std::cerr << "Invalid input: " << choice << ". Input must be either 1 or 2." << std::endl;
        std::exit(-1);
    }
}


void read_data(MyPipe &myPipe, int input_type, std::string process_name)
{
    /*  
     *  Helper function for reading data either from console or file
     *  
     *  Paramters:
     *      &myPipe (MyPipe): Pipe in which processes communicate
     *      input_type (int): type of input, tells from where to read data
     *                        examples:
     *                          1 - read data from console and write it to pipe
     *                          2 - read data from file and write it to pipe
     *      process_name (std::string): name of process which is performing certain task 
     */
    std::cout << "Process (" << process_name << ") ";

    if (input_type == 1)
    {
        std::cout << "Enter " << INPUT_SIZE << " numbers:" << std::endl;
        int buff[INPUT_SIZE];
        for (int i = 0; i < INPUT_SIZE; ++i)
        {
            std::cin >> buff[i];
        }
        myPipe.writeData(*buff);
    }
    else
    {
        std::cout << "reading data from file" << std::endl;
        std::ifstream file(INPUT_FILE);
        std::string text;
        int buff[INPUT_SIZE];
        int i = 0;
        while (std::getline(file, text))
        {
            for (char c : text)
            {

                if (isdigit(c))
                {
                    buff[i] = (int) c - 48;
                    i += 1;
                    if (i == INPUT_SIZE)
                    {
                        break;
                    }
                }
            }
        }
        myPipe.writeData(*buff);
    }
}

void write_data(MyPipe &myPipe, int output_type, std::string process_name)
{
    /*
     * Helper function for displaying data either in console or writing it into the file
     * 
     *  Paramters:
     *      &myPipe (MyPipe): Pipe in which processes communicate
     *      input_type (int): type of input, tells where to write data
     *                        examples:
     *                          1 - read data from pipe and display it into console
     *                          2 - read data from pipe and write it into file
     *      process_name (std::string): name of process which is performing certain task 
     * 
     */
    int buffer[INPUT_SIZE];
    myPipe.readData(*buffer);
    if (output_type == 1)
    {
        std::cout << "Process (" << process_name << ") displaying data to console" << std::endl;
        for (int i = 0; i < INPUT_SIZE; ++i)
        {
            std::cout << buffer[i] << " ";
        }
        std::cout << std::endl;
    }
    else
    {
        std::cout << "Process (" << process_name << ") writing data to file" << std::endl;
        std::ofstream file(OUTPUT_FILE);
        for (int i = 0; i < INPUT_SIZE; ++i)
        {
            file << buffer[i] << " ";
        }
    }
}


int main()
{
    short child_process;
    short input_type;
    short output_type;
    
    get_user_input("Choose child process: child a or child b? (1 for a, 2 for b)", child_process);
    get_user_input("Choose input type: (1 for console, 2 for file)", input_type);
    get_user_input("Choose output type: (1 for console, 2 for file)", output_type);


    MyPipe *myPipe = new MyPipe();

    decltype(fork()) process_a;
    decltype(fork()) process_b;
    int status;

    (process_a = fork()) && (process_b = fork());

    if (process_a == 0)
    {
        // child a
        if (child_process == 1)
        {
            // read data 
            read_data(*myPipe, input_type, "a");
        }
        else
        {
            // write data
            while(waitpid(process_b, &status, 0) != -1);
            write_data(*myPipe, output_type, "a");
        }
    }
    else if (process_b == 0)
    {
        // child b
        if (child_process == 2)
        {
            // read data
            read_data(*myPipe, input_type, "b");
        }
        else
        {
            // write data
            while(waitpid(process_a, &status, 0) != -1);
            write_data(*myPipe, output_type, "b");
        }
    }
    else
    {
        // parent code 
        wait(nullptr);
    }
}

