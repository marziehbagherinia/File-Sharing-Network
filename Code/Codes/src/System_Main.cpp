#include "System.h"

int main(int argc, char const *argv[])
{
	int pipe_fd = atoi(argv[1]);
    string name = argv[2];
    System system(pipe_fd, name);
    
    exit(0);
}