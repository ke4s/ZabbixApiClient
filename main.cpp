#include "ZabbixApiClient.hpp"
#include "Parse.hpp"
#include "utils.hpp"


#include <vector>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;
namespace pt = boost::property_tree;


/*
if ((vm.size() > 2))
				throw std::runtime_error("If 'file' option is provided, no other options should be present");
*/
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
	
	
	/*
	if (argc < 5)
	{
		std::cout << "| url | username | password | key_to_find | key_to_find ..." << std::endl;
		return -1;
	}

	ZabbixApiClient client(argv[1], argv[2], argv[3]);


	std::vector<std::string> keys_to_find;
	for (int i = 4; i < argc; i++) {
		keys_to_find.push_back(argv[i]);
	}


	pt::ptree params;
	params.put("output", "extend");
	params.put("hostids", "10084");
	params.put("with_triggers", true);
	params.put("sortfield", "name");


	std::string response = client.sendRequest(client.getRequestHeader("item.get", params));
	pt::ptree jsonResponse;
	std::istringstream is(response);
	pt::read_json(is, jsonResponse);
	
	{
		boost::property_tree::ptree temp_item = jsonResponse.get_child("result");
		for (int i = 0; i < keys_to_find.size(); i++)
		{
			try
			{
				temp_item.get<std::string>(keys_to_find[i]);
			}
			catch(const std::exception& e)
			{
				std::cout << "silinen:" << (keys_to_find.begin() + i)->c_str() << std::endl;
				keys_to_find.erase(keys_to_find.begin() + i);
			}
		}
	}

	for (int i = 0; i < keys_to_find.size(); i++)
	{
		std::cout << keys_to_find[i] <<  ((keys_to_find.size() -1) == i ? "" : ",");
	}
	std::cout << std::endl;



	//"result" anahtarındaki diziye erişiyoruz.
	for(auto& item : jsonResponse.get_child("result"))
	{
		for(int i = 0 ; i < keys_to_find.size(); i++)
		{
			std::string value = item.second.get<std::string>(keys_to_find[i], "N/A");
			std::cout << value << ((keys_to_find.size() -1) == i ? "" : ",");
		}
		std::cout << std::endl;
	}

	*/

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
