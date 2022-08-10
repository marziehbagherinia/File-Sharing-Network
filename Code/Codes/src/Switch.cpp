#include "Switch.h"

Switch::Switch(int pipeFd, string switch_name)
{
	int n;
	string temp; 
	char buff[MAX_BUFF];
	
	n = read(pipeFd, buff, MAX_BUFF);
	buff[n] = '\0';

	fifoPath = Tools::getNamedFifoName(to_string(getpid()));
	
	if (n == -1 && errno == EAGAIN)
	{
		
		send_message_LB("F");
		close(pipeFd);
    }
	else
	{
		temp = buff;
		waited = true;
		pipe_fd = pipeFd;
		name = switch_name;
		number_of_ports = stoi(temp);
		remained_ports = number_of_ports;
		send_message_LB("S");
		wait_for_command();
	}
}

void Switch::send_message_LB(string message)
{
	int fd = open(fifoPath.c_str(), O_WRONLY | O_CREAT, 0666);
	write(fd, message.c_str(), message.length());
	close(fd);
}

string Switch::wait_for_command()
{
	int n, fd;
	string str_buff;
	vector <string> v;
	char buff[MAX_BUFF];

	while (waited)
	{
		n = read(pipe_fd, buff, MAX_BUFF);
		buff[n] = '\0';	

		if (n != -1)
		{
			str_buff = buff;
			command_handler(Tools::splitSpace(str_buff));
		}		
		check_pipes();
	}

	close(pipe_fd);
	return str_buff;
}

void Switch::command_handler(vector <string> command)
{
	string message;

	if (command.size() < 1)
	{
		cerr << "Failed connection!" << endl;
		return; 
	}
	
	if (command[0] == "exit")
	{
		waited = false;
	}
	else if(command[0] == "Connect")
	{
		if (remained_ports > 0 && command.size() == 4)
		{
			create_pipe(command);
			send_message_LB("S");
		}
		else
		{
			send_message_LB("F");
		}
	}	
	else if (command[0] == "Send")
	{
		update_lookup(command[1], stoi(command[command.size() - 1]));
		message = prepare_message_send(command);
		send_message(message, command[1], command[3]);
		cout << "The switch " << name << " send the <Send> message." << endl;
	}
	else if (command[0] == "Recv")
	{
		update_lookup(command[1], stoi(command[command.size() - 1]));
		message = prepare_message_recv(command);
		send_message(message, command[1], command[3]);
		cout << "The switch " << name << " send the <Recv> message." << endl;
	}
	else if (command[0] == "Delete")
	{		
		delete_pipe(stoi(command[2]));
	}
}

void Switch::delete_pipe(int pipes_str)
{
	vector <int> v;
	v.push_back(-1);
	v.push_back(-1);
	v.push_back(-1);

	for (int i = 0; i < system_pipes.size(); i++)
	{
		if(system_pipes[i][0] == pipes_str)
		{ 
			system_pipes[i] = v;
			remained_ports++;
			delete_from_sp(i);
		}
	}
	
}

void Switch::delete_from_sp(int i)
{
	map <string, int>::iterator itr;
    for (itr = lookup_table.begin(); itr != lookup_table.end(); ++itr) 
	{
        if(itr->second == i)
		{
			lookup_table.erase(itr->first);
			break;
		}
    }
}

void Switch::create_pipe(vector <string> command)
{
	vector <int> fds;

	fds.push_back(stoi(command[1]));
	fds.push_back(stoi(command[2]));
	fds.push_back(stoi(command[3]));

	system_pipes.push_back(fds);
	remained_ports--;
}

void Switch::create_namedPipe(string prefix)
{
	string fifoPath = Tools::getNamedFifoName(prefix);
	mkfifo(fifoPath.c_str(), 0666);
	
	pipes_index[prefix] = pipes_name.size();
	pipes_name.push_back(prefix);
}

void Switch::check_pipes()
{
	int fd, n;
	string str_buff;
	char buff[MAX_BUFF];
	vector <string> str;

	for (int i = 0; i < system_pipes.size(); i++)
	{
		if(system_pipes[i][0] == -1)
			continue;

		n = read(system_pipes[i][2], buff, MAX_BUFF);
		buff[n] = '\0';
		if (n != -1)
		{
			str_buff = buff;
			str = Tools::splitSpace(str_buff);
			str.push_back(to_string(i));
			command_handler(str);
		}
	}
}

void Switch::update_lookup(string id, int index)
{
	if (lookup_table.find(id) != lookup_table.end())
		return;

	lookup_table[id] = index;
}

void Switch::send_message(string message, string src, string dest)
{
	if (lookup_table.find(dest) != lookup_table.end())
	{
		write(system_pipes[lookup_table[dest]][WRITE], message.c_str(), message.length());
	}
	else
	{
		for (int i = 0; i < system_pipes.size(); i++)
		{
			if (lookup_table[src] == i)
				continue;
			
			if(system_pipes[i][0] == -1)
				continue;
			
			write(system_pipes[i][WRITE], message.c_str(), message.length());	
		}
	}
}

string Switch::prepare_message_send(vector <string> command)
{
	string message = command[0];
	message += SPACE + name;
	message += SPACE + command[2];
	message += SPACE + command[3];
	message += SPACE + command[4];

	for (int i = 5; i < command.size() - 1; i++)
		message += SPACE + command[i];

	return message;
}

string Switch::prepare_message_recv(vector <string> command)
{
	string message = command[0];
	message += SPACE + name;
	message += SPACE + command[2];
	message += SPACE + command[3];
	message += SPACE + command[4];

	return message;
}

Switch::~Switch()
{
	exit(0);
}