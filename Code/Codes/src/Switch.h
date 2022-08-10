#include "Tools.h"

class Switch
{
  public:
	Switch(int pipeFd, string switch_name);
	~Switch();

	void check_pipes();
	void delete_from_sp(int i);
	void delete_pipe(int pipes_str);
	void send_message_LB(string message);
	void create_namedPipe(string prefix);
	void update_lookup(string id, int index);
	void create_pipe(vector <string> command);
	void command_handler(vector <string> command);
	void send_message(string message, string src, string dest);

	string wait_for_command();
	string prepare_message_send(vector <string> command);
	string prepare_message_recv(vector <string> command);

  private:
	bool waited;
	int pipe_fd;
	int remained_ports;
	int number_of_ports;
	
	string name;
	string fifoPath;

	map <string, int> lookup_table;
	vector <vector <int>> system_pipes;

	vector <string> pipes_name;
	map <string, int> pipes_index;
};
