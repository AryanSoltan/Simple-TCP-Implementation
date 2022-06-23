#ifndef _DataReciver_hpp_ 
#define _DataReciver_hpp_

#include "PacketTool.hpp"

class DataReciver
{
	private:
		byte recivers_name;
		byte senders_name;
		std::string recived_data;
		byte seq_num;
		int file_number;
		
	public:
		int add_packet(Packet packet);
		void creat_file();
		DataReciver(byte recivers_name, byte senders_name);
};

#endif