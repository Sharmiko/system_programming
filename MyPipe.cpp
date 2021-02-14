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
    std::cout << message_prompt << std::endl;
    std::cout << "#: ";
    std::cin >> choice;

    if (choice != 1 && choice != 2)
    {
        std::cerr << "Invalid input: " << choice << ". Input must be either 1 or 2." << std::endl;
        std::exit(-1);
    }
}


void write_data(MyPipe &myPipe, int input_type, std::string process_name)
{
    std::cout << "Process (" << process_name << ") ";
    std::cout << "Enter " << INPUT_SIZE << " numbers:" << std::endl;

    int buff[INPUT_SIZE];
    for (int i = 0; i < INPUT_SIZE; ++i)
    {
        std::cin >> buff[i];
    }

    if (input_type == 1)
    {
        myPipe.writeData(*buff);
    }
    else
    {
        std::ofstream file(OUTPUT_FILE);
        for (int i = 0; i < INPUT_SIZE; ++i)
        {
            file << buff[i];
        }
    }
}

void read_data(MyPipe &myPipe, int output_type, std::string process_name)
{
    int buffer[INPUT_SIZE];
    myPipe.readData(*buffer);
    
    std::cout << "Process (" << process_name << ") ";

    if (output_type == 1)
    {
        for (int i = 0; i < INPUT_SIZE; ++i)
        {
            std::cout << buffer[i] << " ";
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

    process_a = fork();
    if (process_a == 0)
    {
        // Child process a
        if (child_process == 1)
        {
            // write data
            std::cout << "A writing\n"; 
            write_data(*myPipe, input_type, "a");
        }
        else
        {
            // read data
            std::cout << "A reading data\n";
            read_data(*myPipe, output_type, "a");
        }
    }
    else if (process_a == -1)
    {
        // error occured
        std::cerr << "Could not create process: Error!" << std::endl;
        std::exit(-1);
    }
    else 
    {
        // Parent process
        wait(NULL);

        process_b = fork();
        if (process_b == 0)
        {
            // Child process b
            if (child_process == 2)
            {
                // write data
                std::cout << "B writing\n";
                write_data(*myPipe, input_type, "b");
            }
            else
            {
                // read data
                std::cout << "B reading\n";
                read_data(*myPipe, output_type, "b");
            }
        }
        else if (process_b == -1)
        {
            // error occured
            std::cerr << "Could not create process: Error!" << std::endl;
            std::exit(-1);
        }
        else
        {
            // Parent process
            wait(NULL);
        }
    }
}


