#include "Tools.h"

class LoadBalancer
{
  public:
	LoadBalancer(string _dirname);
	~LoadBalancer();
	
	void read_input();
	void command_handler();
	void create_all_pipes();
	void exit_all_components();
	void create_pipe(int type);
	void fill_pipe(int index, int type);
	void run_component(int index, int type);
	void fork_component(int index, int type);
	void create_namedPipe(int index, int type);
	void send_message(string message, int index, int type);
	bool will_cause_loops(int A, int B, vector<int> in_cycle);
	void bfs(int A, vector <vector <bool>> &spanningTree);
	vector <string> find_loop_edge(int A, int B);

	string get_message(int index, int type);
	vector <string> prepare_connect_message();
	void print_switch_connections();
	vector <vector <bool>> switchConnections;
	
  private:
	bool flag = true;
	int pipe_index = 0;
	int pipefds[2*NUM_OF_PIPES];

	vector <string> command;
	vector <pid_t> switches;
	vector <pid_t> systems;
	vector <pid_t> components;
	vector <vector <int>> switch_pipes;	
	vector <vector <int>> system_pipes;

	map <string, int> switch_index;
	map <string, int> system_index;
	map <string, string> connections;
	map <pair <int, int>, vector <string>> connection_pipes;
};