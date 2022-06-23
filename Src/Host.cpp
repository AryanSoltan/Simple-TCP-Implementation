#include "../Includes/Host.hpp"

Host::Host(byte name)
: name(name)
{
}

void Host::run()
{
	SocketTools socket_tools;
	fd = socket_tools.connect_to_server(PORT);
	std::cout << "Connected to Router";
	char buf[MAX_DATA_SIZE];
	buf[0] = '0' + name;
	buf[1] = 0;
	write(fd, buf, 1);

	fd_set master_set, working_set;
	int max_fd = fd;

	FD_ZERO(&master_set);
	FD_SET(STDIN_FILENO, &master_set);
	FD_SET(fd, &master_set);
	FD_SET(STDIN_FILENO, &master_set);

	struct timeval t;
	t.tv_sec = 2;
	t.tv_usec = 0;

	while (true)
	{
		working_set = master_set;
		if  (select(max_fd + 1, &working_set, NULL, NULL, NULL) == -1)
		{
			std::cout << "Select failed while waiting" << std::endl;
			return;
		}
		if (FD_ISSET(STDIN_FILENO, &working_set))
		{
			read(STDIN_FILENO, buf, sizeof(buf));
			std::string s_tmp(buf);
			send(s_tmp);
		}
		else 
		{
			recive();
		}
	}
}

void Host::send(std::string command)
{
	std::string path;
	byte reciver;
	int window_size;
	int packet_size;
	std::istringstream in(command);
	in >> path >> reciver >> window_size >> packet_size;

	std::ifstream file(path);
	std::string contents((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
	char
}