#ifndef _PacketTool_hpp_
#define _PacketTool_hpp

#include "IncludesAndDefines.hpp"
#include "SocketTool.hpp"
#include "PacketTool.hpp"

class Router 
{
    public:

        Router(int len_input);
        void run_all();
		void run_recv_thread();
		void run_send_thread();
    private: 

        void add_new_packets(std::vector<Packet> new_packets);
        int accept_new_request(int master_socket);
        int find_host(char* host_name);
		char** packet_queue;
		int len_queue;
        int head;
		int num_packets;
		int server_fd;
        int not_sent;
        char** reciver;
		SocketTools socket_tools;
		std::vector<int> hosts;
		std::vector<int>number_messages_from_host;
		std::vector<char*> name_host;
};

#endif