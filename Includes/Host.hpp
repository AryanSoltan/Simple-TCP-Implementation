#ifndef _Host_hpp_
#define _Host_hpp_

#include "PacketTool.hpp"
#include "SocketTool.hpp"

class Host
{
	public:
		Host(byte name);
		void run();
		void recive();
		void send(std::string command);

	private:
		byte name;
		HostState state;
		int fd;
};

#endif