#include "PacketTool.hpp"
#include "SocketTool.hpp"

class Host
{
	public:
		Host(std::string name);
		void run();
		void recive();
		void send();

	private:
		std::string name;
		HostState state;
};