#include "../Includes/Host.hpp"

bool flag;

Host::Host(byte name)
: name(name)
{
}

void Host::run()
{
	SocketTools socket_tools;
	fd = socket_tools.connect_to_server(PORT);
	char buf[MAX_DATA_SIZE];
	buf[0] =  name;
	buf[1] = 0;
	send(fd, &buf, 2, 0);

	fd_set master_set, working_set;
	int max_fd = fd;

	FD_ZERO(&master_set);
	FD_SET(STDIN_FILENO, &master_set);
	FD_SET(fd, &master_set);
	FD_SET(STDIN_FILENO, &master_set);

	struct timeval t;
	t.tv_sec = 3;
	// t.tv_usec = 0;

	while (true)
	{
		working_set = master_set;
		if  (select(max_fd + 1, &working_set, NULL, NULL, NULL) == -1)
		{
			std::cout << "Select failed while waiting" << std::endl;
			return;
		}

		if (FD_ISSET(STDIN_FILENO, &working_set) || flag == 1)
		{
			if(flag)
			{
				if(name == 'd')
					continue;
				strcpy(buf, "./huge_file.txt d 10 1500");	
			}
			else
				read(STDIN_FILENO, buf, sizeof(buf));
			std::string s_tmp(buf);
			send_data(s_tmp);
		}
		else 
		{

			int msg_size = recv(fd, buf, 1024, 0);

			std::string s_tmp;
			for (int i = 0; i < msg_size; i++)
				s_tmp += buf[i];
			std::cout << "Start Reciving" << std::endl;
			recive(s_tmp);
			std::cout << "End of Reciving" << std::endl;
		}
	}
}

void Host::send_data(std::string command)
{
	std::string path;
	byte tmp;
	byte reciver;
	int window_size;
	int packet_size;
	std::istringstream in(command);
	in >> path >> tmp >> window_size >> packet_size;
	reciver = tmp;

	struct timeval tv;
	tv.tv_sec = 3;
	tv.tv_usec = 0;
	setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);

	std::ifstream file(path);
	std::ostringstream oss;
    oss << file.rdbuf();
	std::string contents = oss.str();
	std::vector<Packet> packets = PacketTool::creat_packets(contents, packet_size, reciver, DATA_TYPE, name);

	auto start = std::chrono::high_resolution_clock::now();

	std::cout << "Start sending" << std::endl;
	send_packets(packets, 0, window_size);
	std::cout << "End of sending" << std::endl;

	auto stop = std::chrono::high_resolution_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

	std::cout << "Execution time : " << duration.count() << " microseconds" << std::endl;
}

void Host::send_packets(const std::vector<Packet>& packets, int cur_seq_num, int window_size)
{

	int last = cur_seq_num;
	char buf[MAX_DATA_SIZE];

	std::map<byte, bool> acks;

	while (cur_seq_num < packets.size())
	{

		while(last < std::min((int) packets.size(), cur_seq_num + window_size))
		{
			Msg msg(packets[last++]);
			Packet pkt = packets[last-1];
			std::cerr << "Packet is sending: sender reciver seqnum: " << msg.msg[2] << " " << msg.msg[0] << " " << int(msg.msg[3])
					<< std::endl;
			send(fd, msg.msg, msg.len, 0);
		}
		
		int len = recv(fd, buf, MAX_DATA_SIZE, 0);
		if (len == -1)
		{
			send_packets(packets, cur_seq_num, window_size);
			return;
		}
		std::vector<Packet> res = PacketTool::parse_packet(buf, len);
		for (auto packet : res)
		{
			if (packet.type == ACK_TYPE)
			{
				acks[packet.seq_num] = true;
			}
				
		}
			

		while (cur_seq_num < last)
		{
			if (acks[cur_seq_num])
			{
				acks[cur_seq_num] = false;
				cur_seq_num++;
			}
			else
				break;
		}

	}
}

void Host::recive(std::string data)
{
	std::vector<Packet> packets = PacketTool::parse_packet(data);
	int ind;
	int ack;
	for (auto packet : packets)
	{
		if (has_reciver.find(packet.sender) == has_reciver.end())
		{
			has_reciver[packet.sender] = data_recivers.size();
			data_recivers.push_back(DataReciver(name, packet.sender));
		}
		ind = has_reciver[packet.sender];
		ack = data_recivers[ind].add_packet(packet);
		if (ack < 0)
			continue;
		byte ret = (ack % 256);
		Packet pkt(packet.sender, ACK_TYPE, packet.reciver, ret); 
		Msg msg(pkt);
		std::cerr << "Ack is sending: sender reciver seq_num: " << msg.msg[2] << " " << msg.msg[0] << " " << int(msg.msg[3])
					<< std::endl;
		send(fd, msg.msg, msg.len, 0);
	}
}

Host::~Host()
{
	close(fd);
}

int main(int argc, char* argv[])
{
	if (argc != 2 && argc != 3)
	{
		std::cout << "Please write the number of Arguemants for the running Host" << std::endl;
		return 0;
	}
	if(argc == 3)
	{
		flag = 1;
	}
	Host* host = new Host((byte)argv[1][0]);
	host->run();
	return 0;
}