#include "LoadBalancer.h"

LoadBalancer::LoadBalancer(string _dirName)
{
	cout << RED "Welcome!" << RESET << endl;
	this -> create_all_pipes();
	this -> read_input();
}

void LoadBalancer::read_input()
{
	string line, field; 
	
	while (flag)
	{
		cout << CYAN << "Enter your command:" << RESET << endl;
		
		command.clear();
		getline(cin, line);
		stringstream ss(line);
		
		while(getline(ss, field,' '))
			command.push_back(field);

		this -> command_handler();
	}
}

void LoadBalancer::command_handler()
{
	if(command.size() < 1)
		return;

	string key, message, res;
	vector <string> help;
	key = command[0];

	if(key == "exit")
	{
		flag = false;
		exit_all_components();
	}
	else if(key == "Switch" )
	{
		if (command.size() != 3)
		{
			cerr << "Bad request!" << endl;
		}
		else if(switch_index.find(command[2]) != switch_index.end() || 
				system_index.find(command[2]) != system_index.end())
		{
			cerr << "Duplicate name!" << endl;
		}
		else
		{
			create_pipe(0);
			switch_index[command[2]] = switch_pipes.size() - 1;
			fill_pipe(switch_index[command[2]], 0);
			fork_component(switch_index[command[2]], 0);
			create_namedPipe(switch_index[command[2]], 0);
			res = get_message(switch_index[command[2]], 0);
		
			if (res == "F")
			{
				cerr << "Switch does not create correctly!" << endl;
				switches.erase(switches.end() - 1);
				switch_pipes.erase(switch_pipes.end() - 1);
				switch_index.erase(command[2]);
			}
			else
			{
				vector<bool> temp;
				temp.push_back(false);
				
				for(int i = 0; i < switchConnections.size();i++)
				{
					switchConnections[i].push_back(false);
					temp.push_back(false);
				}
				switchConnections.push_back(temp);
			}
		}
	}
	else if (key == "System" )
	{
		if (command.size() != 2)
		{
			cerr << "Bad request!" << endl;
		}
		else if(system_index.find(command[1]) != system_index.end() || 
				switch_index.find(command[1]) != switch_index.end())
		{
			cerr << "Duplicate name!" << endl;
		}
		else
		{
			create_pipe(1);
			system_index[command[1]] = system_pipes.size() - 1;
			fill_pipe(system_index[command[1]], 1);
			fork_component(system_index[command[1]], 1);
			create_namedPipe(system_index[command[1]], 1);
			res = get_message(system_index[command[1]], 1);
			if (res == "F")
			{
				cerr << "System does not create correctly!" << endl;
				systems.erase(systems.end() - 1);
				system_pipes.erase(system_pipes.end() - 1);
				system_index.erase(command[1]);
			}
		}
	}
	else if (key == "Connect" )
	{
		if (command.size() != 3 || 
			system_index.find(command[1]) == system_index.end() || 
			switch_index.find(command[2]) == switch_index.end())
		{
			cerr << "Bad request!" << endl;
		}
		else
		{
			vector <string> v = prepare_connect_message();
			send_message(v[0], switch_index[command[2]], 0);
			res = get_message(switch_index[command[2]], 0);
			if(res == "S")
			{
				connections[command[1]] = command[2];
				send_message(v[1], system_index[command[1]], 1);
				res = get_message(system_index[command[1]], 1);
				if(res == "S")
					cout << "Connected!" << endl;
				else
					cerr << "Could not connect!" << endl;
			}
			else
			{
				cerr << "There is no free port on this switch!" << endl;
			}
		}
	}
	else if (key == "Connect_S" )
	{
		bool t = false;
		vector<int> in_cycle;
		in_cycle.push_back(switch_index[command[1]]);
		t = will_cause_loops(switch_index[command[1]], switch_index[command[2]], in_cycle);

		if (command.size() != 3 || switch_index.find(command[1]) == switch_index.end() || switch_index.find(command[2]) == switch_index.end())
		{
			cerr << "Bad request!" << endl;
		}
		else
		{
			vector <string> v = prepare_connect_message();
			send_message(v[0], switch_index[command[2]], 0);
			res = get_message(switch_index[command[2]], 0);
			if (res == "S")
			{
				connections[command[1]] = command[2];
				send_message(v[1], switch_index[command[1]], 0);
				res = get_message(switch_index[command[1]], 0);
				if (res == "S")
				{
					switchConnections[switch_index[command[1]]][switch_index[command[2]]] = true;
					switchConnections[switch_index[command[2]]][switch_index[command[1]]] = true;
					connection_pipes[make_pair(switch_index[command[2]], switch_index[command[1]])] = v;

					cout << "Connected!" << endl;
				}
				else
					cerr << "There is no free port on this switch!" << endl;
			}
			else
			{
				cerr << "There is no free port on this switch!" << endl;
			}
			if (t) 
			{
				vector <bool> temp;
				vector < vector <bool>> spanningTree;

				cout << "This connection caused a loop!" << endl;

				for (int i = 0; i < switchConnections.size(); i++)
					temp.push_back(false);
				for (int i = 0; i < switchConnections.size(); i++)
					spanningTree.push_back(temp);
				
				bfs(switch_index[command[1]], spanningTree);

				int A, B = -1;
				bool flg = false;
				for (int i = 0; i < switchConnections.size(); i++) 
				{
					for (int j = 0; j < switchConnections.size(); j++)
					{
						if (switchConnections[i][j] != spanningTree[i][j])
						{
							A = i;
							B = j;
							flg = true;
							switchConnections[i][j] = spanningTree[i][j];
							switchConnections[j][i] = spanningTree[j][i];
							break;
						}
					}
					if (flg)
						break;
				}

				vector <string> res = find_loop_edge(A, B);
				if(res.size() > 1)
					cout << "For removing loop, we delete edge between switches " << res[0] << "-" << res[1] << endl;

				if(connection_pipes.find(make_pair(A, B)) != connection_pipes.end())
				{
					string str_1, str_2;
					str_1 = "Delete" + connection_pipes[make_pair(A, B)][0];
					str_2 = "Delete" + connection_pipes[make_pair(A, B)][1];
					send_message(str_1, A, 0);
					send_message(str_2, B, 0);
					connection_pipes.find(make_pair(A, B)); 
				}
				else if(connection_pipes.find(make_pair(B, A)) != connection_pipes.end())
				{
					string str_1, str_2;
					str_1 = "Delete " + connection_pipes[make_pair(B, A)][0];
					str_2 = "Delete " + connection_pipes[make_pair(B, A)][1];
					send_message(str_1, B, 0);
					send_message(str_2, A, 0);
					connection_pipes.find(make_pair(B, A));
				}
			}
		}
	}
	else if (key == "Send" )
	{
		if (command.size() != 4)
		{
			cerr << "Bad request!" << endl;
		}
		else if(system_index.find(command[1]) == system_index.end() || 
				system_index.find(command[2]) == system_index.end())
		{
			cerr << "Wrong source or destination!" << endl;
		}
		else
		{
			message = "";
			for (int i = 0; i < command.size(); i++)
				message += command[i] + SPACE;
		
			send_message(message, system_index[command[1]], 1);
		}
	}
	else if (key == "Recv" )
	{
		if (command.size() != 4)
		{
			cerr << "Bad request!" << endl;
		}
		else if(system_index.find(command[1]) == system_index.end() || 
				system_index.find(command[2]) == system_index.end())
		{
			cerr << "Wrong source or destination!" << endl;
		}
		else
		{
			message = "";
			for (int i = 0; i < command.size(); i++)
				message += command[i] + SPACE;
		
			send_message(message, system_index[command[1]], 1);
		}
	}
	else
	{
		cout << "Invalid command!" << endl;
	}
}

void LoadBalancer::create_all_pipes()
{
	for(int i = 0; i < NUM_OF_PIPES; i++ )
		if(pipe(pipefds + i*2) < 0)
			cerr << "Pipe construction failed!" << endl;
	
	//cout << pipefds[0] << " " << pipefds[1] << " " << pipefds[2] << " " << pipefds[3] << endl;
}

vector <string> LoadBalancer::prepare_connect_message()
{
	string message_to_switch, message_to_system;
	vector <int> switch_fds, system_fds;
	vector <string> out_str;

	fcntl(pipefds[pipe_index], F_SETFL, O_NONBLOCK);
	switch_fds.push_back(pipefds[pipe_index]);
	switch_fds.push_back(pipefds[pipe_index + 1]);	
	pipe_index += 2;
	
	fcntl(pipefds[pipe_index], F_SETFL, O_NONBLOCK);
	system_fds.push_back(pipefds[pipe_index]);
	system_fds.push_back(pipefds[pipe_index + 1]);	
	pipe_index += 2;
	
	message_to_switch = CONNECT_SYS_TO_SWITCH;
	message_to_switch += SPACE + to_string(switch_fds[READ]);
	message_to_switch += SPACE + to_string(switch_fds[WRITE]);
	message_to_switch += SPACE + to_string(system_fds[READ]);

	message_to_system = CONNECT_SYS_TO_SWITCH;
	message_to_system += SPACE + to_string(system_fds[READ]);
	message_to_system += SPACE + to_string(system_fds[WRITE]);
	message_to_system += SPACE + to_string(switch_fds[READ]);

	out_str.push_back(message_to_switch);
	out_str.push_back(message_to_system);

	//cout << switch_fds[0] << " " << switch_fds[1] << " " << system_fds[0] << " " << system_fds[1] << " " << pipe_index << endl;
	return out_str;
}

void LoadBalancer::create_pipe(int type)
{
	vector <int> fds;

	fcntl(pipefds[pipe_index], F_SETFL, O_NONBLOCK);
	fds.push_back(pipefds[pipe_index]);
	fds.push_back(pipefds[pipe_index + 1]);	
	pipe_index += 2;

	if (type == 0)
		switch_pipes.push_back(fds);
	else
		system_pipes.push_back(fds);

	//cout << fds[0] << " " << fds[1]  << endl;
}

void LoadBalancer::fill_pipe(int index, int type)
{
	string result = command[1];

	if (type == 0)
	{
		//close(switch_pipes[index][READ]);
		write(switch_pipes[index][WRITE], result.c_str(), result.length());
		//close(switch_pipes[index][WRITE]);
	}
	else
	{
		//close(system_pipes[index][READ]);
		write(system_pipes[index][WRITE], result.c_str(), result.length());
		//close(system_pipes[index][WRITE]);
	}
}

void LoadBalancer::fork_component(int index, int type)
{
	pid_t pid = fork();

	if(pid < 0)
		cerr << "Process construction failed!" << endl;
	else if(pid == 0)
		run_component(index, type);
	else
		components.push_back(pid);
		if (type == 0)
			switches.push_back(pid);
		else
			systems.push_back(pid);

	if (type == 0)
		close(switch_pipes[index][READ]);
	else
		close(system_pipes[index][READ]);
}

void LoadBalancer::run_component(int index, int type)
{
	char* argv[4];
	argv[3] = NULL;

	if (type == 0)
	{
		close(switch_pipes[index][WRITE]);
		argv[0] = (char*) SWITCH_EXEC_PATH; //Switch
		argv[1] = (char*) to_string(switch_pipes[index][READ]).c_str();
		argv[2] = (char*) command[2].c_str();
	}
	else
	{
		close(system_pipes[index][WRITE]);
		argv[0] = (char*) SYSTEM_EXEC_PATH; //System
		argv[1] = (char*) to_string(system_pipes[index][READ]).c_str();
		argv[2] = (char*) command[1].c_str();
	}

	execv(argv[0], argv);
}

void LoadBalancer::create_namedPipe(int index, int type)
{
	string fifoPath;

	if (type == 0)
		fifoPath = Tools::getNamedFifoName(to_string(switches[index]));
	else
		fifoPath = Tools::getNamedFifoName(to_string(systems[index]));

	mkfifo(fifoPath.c_str(), 0666);
}

string LoadBalancer::get_message(int index, int type)
{
	int fd, n;
	string fifoPath, buffStr;
	char buff[MAX_BUFF];
	
	if (type == 0)
		fifoPath = Tools::getNamedFifoName(to_string(switches[index]));
	else
		fifoPath = Tools::getNamedFifoName(to_string(systems[index]));

	fd = open(fifoPath.c_str(), O_RDONLY);
	n = read(fd, buff, MAX_BUFF);
	buff[n] = '\0';
	close(fd);

	buffStr = buff;

	return buffStr;
}

void LoadBalancer::send_message(string message, int index, int type)
{
	
	if (type == 0)
		write(switch_pipes[index][WRITE], message.c_str(), message.length());
	else
		write(system_pipes[index][WRITE], message.c_str(), message.length());
}

void LoadBalancer::exit_all_components()
{
	for(int i = 0 ; i < switches.size() ; i++)
		send_message(EXIT, i, 0);
	for(int i = 0 ; i < systems.size() ; i++)
		send_message(EXIT, i, 1);
}

bool LoadBalancer::will_cause_loops(int A, int B,vector<int> in_cycle)
{
	if (in_cycle[in_cycle.size() - 1] == B)
	{
		return true;
	}
	
	for(int j = 0; j < in_cycle.size()-1;j++)
		if(in_cycle[j] == in_cycle[in_cycle.size()-1])
			return false;
	if(A == B)
		return true;
	for(int i = 0 ; i < switchConnections.size();i++)
	{
		if(switchConnections[A][i]){
			in_cycle.push_back(i);
			bool t = this->will_cause_loops(i,B,in_cycle);
			//in_cycle.pop_back();
			if(t)
				return true;
		}
	}
	return false;
}

void LoadBalancer::print_switch_connections() {
	for (int j = 0; j < switchConnections.size();j++) {
		for (int i = 0; i < switchConnections.size(); i++) {
			if (switchConnections[j][i])
				cout << "1 ";
			else
				cout << "0 ";
		}
		cout << endl;
	}
}

void LoadBalancer::bfs(int A, vector <vector <bool>> &spanningTree)
{ 
	vector <int> q;
	vector <bool> visited;
	
	for (int i = 0; i < switchConnections.size(); i++)
		visited.push_back(false);
		
	q.push_back(A);
	while (!q.empty())
	{
		visited[q[0]] = true;
		for (int i = 0; i < switchConnections.size(); i++)
		{
			if (switchConnections[q[0]][i] && visited[i] == false) 
			{
				spanningTree[q[0]][i] = true;
				spanningTree[i][q[0]] = true;
				q.push_back(i);
				visited[i] = true;
			}
		}
		q.erase(q.begin());
	}
}

vector <string> LoadBalancer::find_loop_edge(int A, int B)
{
	vector <string> res;
	map <string, int>::iterator itr;
    for (itr = switch_index.begin(); itr != switch_index.end(); ++itr) 
	{
        if(itr->second == A)
		{
			res.push_back(itr->first);
			if(res.size() == 2)
				break;
		}
        if(itr->second == B)
		{
			res.push_back(itr->first);
			if(res.size() == 2)
				break;
		}
	}
	return res;
}

LoadBalancer::~LoadBalancer()
{
	for(int i = 0 ; i < components.size() ; i++)
		waitpid(components[i], NULL, 0);
}