#include "webserv.hpp"

int main(int argc, char **argv)
{
	const char *path = "rss/default.conf";
	if (argc == 2)
		path = argv[1];
	std::vector<Configuration>  configs;
	int server_count;
	try
	{
		server_count = read_conf(path, configs);
	}
	catch(std::string error)
	{
		std::cerr << error << std::endl;
		return 1;
	}

	Server server(configs);
	while (1)
	{
		server.main_cycle();
	}	
	return 0;
}