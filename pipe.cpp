#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <fstream>
#include <sys/types.h>
#include <sys/wait.h>

#define N 5

using namespace std;


class MyPipe {
public:
	int inp;
	int out;

	MyPipe() 
	{
		int tmp[2];
		if (pipe(tmp) == -1) 
		{
			cerr << "Can't create pipe" << endl;
			exit(-1);
		}
		inp = tmp[0];
		out = tmp[1]; 
	}
	
	int readData(int& buff) 
	{
		int size = (int) read(inp, &buff, N * sizeof(int)) / sizeof(int);
		return size;
	}
	
	int writeData(string file) 
	{
		int buff[N];
		ifstream ifs (file);
		for (int i = 0; i < N; i++) 
		{
			ifs >> buff[i];
		}
		ifs.close();
		int size = (int) write(out, buff, N * sizeof(int)) / sizeof(int);
		return size;
	}
	
	~MyPipe() {
		close(inp);
		close(out);
	}
};

int main() 
{
	MyPipe* channel = new MyPipe();
	if (fork() == 0) 
	{ 
		// child 
		string name = "file.in";
		channel->writeData(name);
		delete channel;
	} 
	else 
	{ 
		// parent
		wait(NULL);
		int arr[N];
		channel->readData(*arr);
		for (int i = 0; i < N; i++) 
		{
			cout << arr[i] << "  ";
		}
		cout << endl;
		delete channel;
	}

	return 0;
}

