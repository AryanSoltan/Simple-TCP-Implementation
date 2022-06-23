#include "../Includes/Host.hpp"

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cout << "Wring number of Argumants for runnig Host" << std::endl;
		return 0;
	}
	std::string host_name(argv[1]);
	Host host((byte)stoi(host_name));
	host.run();
	return 0;
}