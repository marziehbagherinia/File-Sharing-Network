#include "Switch.h"

int main(int argc, char const *argv[])
{
	int pipe_fd = atoi(argv[1]);
    string name = argv[2];
    Switch Switch(pipe_fd, name);
 
    exit(0);
}