#include "ZabbixApiClient.hpp"

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;
namespace pt = boost::property_tree;

int main()
{
    ZabbixApiClient client("192.168.1.22", "Admin", "zabbix");

    pt::ptree params;


    params.put("output", "extend");
    params.put("hostids", "10084");
    params.put("with_triggers", true);
    params.put("sortfield", "name");


    std::string response = client.sendRequest(client.getRequestHeader("item.get", params));
    pt::ptree jsonResponse;
    std::istringstream is(response);
    pt::read_json(is, jsonResponse);

    // "result" anahtarındaki diziye erişiyoruz.
    for(auto& item : jsonResponse.get_child("result")) {
        std::string itemid = item.second.get<std::string>("itemid");
        std::string type = item.second.get<std::string>("lastvalue");
        std::cout << "itemid: " << itemid << ", type: " << type << std::endl;
    }

    
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

*/