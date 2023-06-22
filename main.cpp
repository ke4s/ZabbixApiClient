#include "ZabbixApiClient.hpp"
#include "Parse.hpp"
#include "utils.hpp"


#include <vector>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;
namespace pt = boost::property_tree;


int main(int argc, char** argv)
{
	ProgramOptions options;

	if (int idx = strsncmp("--file", argv, argc, 7) != -1)
	{
		if (argc != 3 || idx != 1)
		{
			std::cerr << "Error: --file parameter need an argument" << std::endl;
			return 1;
		}
		Parse::parseFromFile(argv[2], options);
	}
	else
		Parse::parseFromArgs(argc, argv, options);

	
	std::cout << options << std::endl;
	
	

	ZabbixApiClient client(options.url, options.username, options.password);


	pt::ptree params;
	params.put("output", "extend");
	params.put("hostids", "10084");
	params.put("with_triggers", true);
	params.put("sortfield", "name");


	std::string response = client.sendRequest(client.getRequestHeader(options.method, params));
	pt::ptree jsonResponse;
	std::istringstream is(response);
	pt::read_json(is, jsonResponse);




	for (int i = 0; i < options.keys.size(); i++)
	{
		std::cout << options.keys[i] <<  ((options.keys.size() -1) == i ? "" : ",");
	}
	std::cout << std::endl;


	for(auto& item : jsonResponse.get_child("result"))
	{
		for(int i = 0 ; i < options.keys.size(); i++)
		{
			std::string value = item.second.get<std::string>(options.keys[i], "N/A");
			std::cout << value << ((options.keys.size() -1) == i ? "" : ",");
		}
		std::cout << std::endl;
	}

	return 0;
}

/*

g++ main.cpp -lboost_system -lboost_thread -lboost_date_time -lboost_chrono -lboost_regex -lboost_atomic -lboost_serialization -lboost_filesystem -lboost_log -lboost_log_setup -lboost_program_options -lboost_unit_test_framework

{
  "jsonrpc": "2.0",
  "result": [
    {
      "itemid": "45495",
      "type": "18",
    },
    {
      "itemid": "45492",
      "type": "181",
    },
    {
      "itemid": "4545",
      "type": "189",
    }
    ]
}


itemid: 45494, type: {"fsname":"/boot","options":"rw,relatime","bytes":{"used":136040448,"free":1780183040,"total":2040373248,"pused":7.099404,"pfree":92.900596},"fstype":"ext4","inodes":{"used":316,"free":130756,"total":131072,"pused":0.241089,"pfree":99.758911}}
itemid: 45488, type: {"fsname":"/","options":"rw,relatime","bytes":{"used":6156656640,"free":5250347008,"total":12040970240,"pused":53.972602,"pfree":46.027398},"fstype":"ext4","inodes":{"used":91069,"free":662595,"total":753664,"pused":12.083501,"pfree":87.916499}}
*/
