#ifndef _PacketTool_hpp_
#define _PacketTool_hpp

#include "IncludesAndDefines.hpp"

struct Packet
{
	byte reciver;
	byte type;
	byte sender;
	byte seq_num;
	int data_size;
	char data[MAX_DATA_SIZE];
};

class Msg
{
	public:
		int len;
		char* msg;
		Msg (Packet packet);
		~Msg ();
};

class PacketTool
{
	public:
		static Packet get_top_packet(char msg[], int& cur_index, int len);
		static std::vector<Packet> parse_packet(char msg[], int len);
};

#endif