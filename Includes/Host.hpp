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
		void send_data(std::string command);
		void send_packets(const std::vector<Packet>& packets, int l, int window_size);

	private:
		byte name;
		HostState state;
		int fd;
};

#endif