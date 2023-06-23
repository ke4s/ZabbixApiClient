#include "ZabbixApiClient.hpp"
#include "Parse.hpp"
#include "utils.hpp"
#include "PTreePrinter.hpp"

#include <vector>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;
namespace pt = boost::property_tree;





void mergePropertyTrees(pt::ptree& dest, const pt::ptree& src)
{
    for (const auto& pair : src)
        dest.add_child(pair.first, pair.second);
}

#include <boost/algorithm/string.hpp>
pt::ptree prepareJsonBody(const ProgramOptions& options)
{
    pt::ptree params;

	
	pt::ptree parameters;
	for (const auto& parameter : options.parameters)
	{
        auto delimiterPos = parameter.find(':');
        auto path = parameter.substr(0, delimiterPos);
        auto value = parameter.substr(delimiterPos + 1);
		parameters.put(path, value);
	}

	if (!options.parameters.empty())
		mergePropertyTrees(params, parameters);


	pt::ptree output;
    for (const auto& key : options.keys)
	{
        pt::ptree key_node;
        key_node.put("", key);
        output.push_back(std::make_pair("", key_node));
    }

    
	if (!options.keys.empty())
    	params.add_child("output", output);
	
    pt::ptree filterTree;
    for (const auto& filter : options.filters)
	{
        auto delimiterPos = filter.find(':');
        auto path = filter.substr(0, delimiterPos);
        auto value = filter.substr(delimiterPos + 1);
        std::istringstream pathStream(path);
        std::string pathSegment;
        pt::ptree* currentNode = &filterTree;
        while (std::getline(pathStream, pathSegment, '.'))
            currentNode = &currentNode->put_child(pathSegment, pt::ptree());
		
        currentNode->put_value(value);
    }

    if (!options.filters.empty())
        params.put_child("filter", filterTree);


	return params;
}




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

	
	//std::cout << options << std::endl;
	
	

	ZabbixApiClient client(options.url, options.username, options.password);

	pt::ptree body = prepareJsonBody(options);



    auto request = client.prepareRequest(options.method, body);
	
	//std::cout << request << std::endl  << std::endl << std::endl << std::endl;

	auto response = client.sendRequest(request);


	if (options.outputFormat == "csv")
		PTreePrinter::printJsonToCsv(response, options.keys);
	else if (options.outputFormat == "json")
		PTreePrinter::printStringToJson(response, options.keys);
	else if (options.outputFormat == "xml")
		PTreePrinter::printJsonToXml(response, options.keys);

	return 0;
}
