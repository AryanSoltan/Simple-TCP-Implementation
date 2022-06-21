#include "../Includes/PacketTool.hpp"

Msg::Msg (Packet packet)
{
	len = 4 + packet.data_size + 1;
	for (int i = 0 ; i < packet.data_size; i++)
		if (packet.data[i] == 0 || packet.data[i] == 1)
			len++;
	msg = new char[len];
	int cnt = 0;
	for (int i = 0; i < packet.data_size; i++)
	{
		if (packet.data[i] == 0 || packet.data[i] == 1)
			msg[cnt++] = 1;
		msg[cnt++] = packet.data[i];
	}
	msg[cnt] = 0;
}

Msg::~Msg ()
{
	delete [] msg;
}

Packet PacketTool::get_top_packet(char msg[], int& cur_index, int len)
{
	Packet packet;
	packet.reciver = msg[cur_index++];
	packet.type = msg[cur_index++];
	packet.sender = msg[cur_index++];
	packet.seq_num = msg[cur_index++];
	packet.data_size = 0;
	while(cur_index < len)
	{
		if (msg[cur_index] == 0)
			break;
		if (msg[cur_index] == 1)
		{
			cur_index++;
			packet.data[packet.data_size ++] = msg[cur_index++];
		}
		else if (msg[cur_index] == 0)
		{
			cur_index++;
			break;
		}
		else
			packet.data[packet.data_size++] = msg[cur_index++];
	}
	return packet;
}

std::vector<Packet> PacketTool::parse_packet(char msg[], int len)
{
	int cur_index = 0;
	std::vector<Packet> packets;
	while (cur_index < len)
		packets.push_back(get_top_packet(msg, cur_index, len));
	return packets;
}