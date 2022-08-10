#ifndef _TOOLS_H
#define _TOOLS_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h> 
#include <sys/wait.h> 
#include <fcntl.h>
#include <unistd.h>
#include <algorithm>
#include <bits/stdc++.h>
#include <dirent.h>
#include <map>
#include <fcntl.h>

using namespace std;


#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"  
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m" 

#define READ      0
#define WRITE     1
#define MAX_BUFF  4096
#define NUM_OF_PIPES 100

#define DOT           '.'
#define COMMA         ','
#define SPACE         ' '
#define UNDERLINE     '_'
#define EMPTY_STRING  ""
#define DIR_SEPERATOR '/'

#define FIFO_FILE_PATH 	 "./namedPipe/fifo"
#define SWITCH_EXEC_PATH "bin/Switch"
#define SYSTEM_EXEC_PATH "bin/System"

#define EXIT "exit"
#define CONNECT_SYS_TO_SWITCH "Connect"
#define SEND "Send"
#define RECV "Recv"
#define SWITCH "Swc"
#define SYSTEM "Sys"

class Tools
{
public:
	static bool doesContainDot(string line);

	static int findMaximum(vector <int> input);
	static int findMinimum(vector <int> input);

	static string getNamedFifoName(string weightVectorsName);
	static string removeAllSpaces(string);
	static string vectorToString(const vector <int> &vec);

	static vector <int>    splitSlash(string date);
	static vector <string> splitSpace(string date);

	static void splitCommand(string dates, int &id, string &start_date, string &end_date);
};

#endif