#include "Tools.h"

class System
{
  public:
	System(int pipeFd, string sys_name);
	~System();

	string wait_for_command();
	string read_file(string filename);
	string prepare_message_send(vector <string> command);
	string prepare_message_recv(vector <string> command);

	void check_pipe();
	void send_message(string message);
	void send_message_LB(string message);
	void create_pipe(vector <string> command);
	void command_handler(vector <string> command);
	void handle_message(vector <string> str, string message_content);
	
  private:
	bool waited = true;
	bool connected = false;

	int pipe_fd;
	vector <int> fds;
	string name, fifoPath, file_content;
	map <string, bool> files;
};
