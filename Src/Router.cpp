#include "Router.hpp"

#define LEN_PACKET 1024

Router::Router(int len_input)
{
	len_queue = len_input;
	packet_queue = (char**)malloc(sizeof(char*) * len_input);
	head = 0;
	not_sent = 0;
}

int Router::find_host(char* host_name)
{
	for(int i = 0; i < name_host.size(); i++)
	{
		if(strcmp(host_name, name_host[i]))
		{
			return hosts[i];
		}
	}
}

void Router::run_send_thread()
{
	while(true)
	{
		if(not_sent)
		{
			int indx = head - not_sent + 1;
			if(indx < 0)
				indx += len_queue;
			char* msg_str = packet_queue[indx];
			int fd_need = find_host(reciver[indx]);		
			send(fd_need, &fd_need, sizeof(msg_str), 0);
			not_sent--;
		}
	}	
}

int Router::accept_new_request(int master_socket)
{
	struct sockaddr_in caddr;
	int len = sizeof(caddr);
	int* caddrlen = &len;

	int new_socket;
	if((new_socket = accept(master_socket, (struct sockaddr *) &caddr, (unsigned int *) caddrlen)) < 0){
		std::cerr << "New Host connection failed." << std::endl;
		return 1;
	}
	else
		std::cout << "Host accepted" << std::endl;

	return new_socket;
}

void Router::add_new_packets(std::vector<Packet> new_packets)
{
	for(int i = 0; i < new_packets.size(); i++)
	{
		char* msg_now;
		Msg msg = Msg(new_packets[i]);
		msg_now = msg.msg;
		packet_queue[head % len_queue] = msg_now;
		strcpy(reciver[head % len_queue], (char*) new_packets[i].reciver);
		head++;
		not_sent++;
	}
}

void Router::run_recv_thread()
{
	fd_set master_set, working_set;
	char buf[LEN_PACKET];

	int server_fd = 0;
	int max_fd = server_fd;

	FD_ZERO(&master_set);
	FD_SET(server_fd, &master_set);

	while(true)
	{
		working_set = master_set;
		if (select(max_fd + 1, &working_set, NULL, NULL, NULL) == -1)
		{
			std::cerr << "Select failed" << std::endl;
			exit(-1);
		}

		if (FD_ISSET(server_fd, &working_set))
		{
			int new_fd = accept_new_request(server_fd);
			hosts.push_back(new_fd);
			number_messages_from_host.push_back(0);
			max_fd = std::max(max_fd, new_fd);
			FD_SET(new_fd, &master_set);
		}
		else
		{
			int len = hosts.size();
			for (int i = 0; i < len; i++)
			{
				if (!FD_ISSET(hosts[i], &working_set))
					continue;
				int msg_size = recv(hosts[i], &buf, sizeof(buf), 0);
				if(msg_size <= 0)
				{
					if(msg_size < 0)
						std::cerr << "Message is broken" << std::endl;
					else if(msg_size == 0)
					{
						std::cout << "A client quits" << std::endl;
						FD_CLR(hosts[i], &master_set);
						close(hosts[i]);
						hosts.pop_back();
					}
				}
				else{
					buf[msg_size] = '\0';
					if(!number_messages_from_host[i])
					{
						strcpy(name_host[i], buf);
					}
					else 
					{
						std:: vector<Packet> new_packets = PacketTool::parse_packet(buf, msg_size);
						add_new_packets(new_packets);
					}
					number_messages_from_host[i]++;
				}
			}
		}
	}
}

void Router::run_all()
{
	std::thread thread1(&Router::run_recv_thread, this);
	std::thread thread2(&Router::run_send_thread, this);
	thread1.join();
	thread2.join();
}