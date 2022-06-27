#ifndef _Router_hpp_
#define _Router_hpp_

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
		~Router ();

    protected: 

        void add_new_packets(std::vector<Packet> new_packets);
        int accept_new_request(int master_socket);
        int find_host(byte host_name);
		int len_queue;
		int router_fd;
		SocketTools socket_tools;
		std::vector<int> hosts;
		std::vector<int>number_messages_from_host;
		byte name_host[MAX_CONNECTIONS];
		std::queue<Packet> router_queue;
};

class RouterRed: Router 
{
	public:
		RouterRed(int len_input);
		void run_red();
	private:
		void run_send_thread_red();
		void run_recv_thread_red();
		double calculate_new_probablility();
		void add_new_packets_red(std::vector<Packet> new_packets);
		void calculate_avg();
		double new_avg;
		double w_q;
		double min_th;
		double max_th;
};

#endif