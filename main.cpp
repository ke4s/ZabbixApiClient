#include "ZabbixApiClient.hpp"
#include "GetProgramOptions.hpp"
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
pt::ptree prepareJsonBody(const ReqConfig& options)
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
    for (const auto& key : options.outputs)
	{
        pt::ptree key_node;
        key_node.put("", key);
        output.push_back(std::make_pair("", key_node));
    }

    
	if (!options.outputs.empty())
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
		GetProgramOptions::getFromFile(argv[2], options);
	}
	else
		GetProgramOptions::getFromArgs(argc, argv, options);

	
	//std::cout << options << std::endl;
	
	

	ZabbixApiClient client(options.apiConfig.url, options.apiConfig.username, options.apiConfig.password);

	pt::ptree body = prepareJsonBody(options.reqConfig);



    auto request = client.prepareRequest(http::verb::get ,options.reqConfig.method, body);
	
	//std::cout << request << std::endl  << std::endl << std::endl << std::endl;

	auto response = client.sendRequest(request);


	std::vector<std::string> &toPrint = (options.outputConfig.keyToPrint.empty() ? options.reqConfig.outputs : options.reqConfig.outputs); //burada eğer  dizi boş ise tamamını bastırmalı.

	if (options.outputConfig.outputFormat == "csv")
		PTreePrinter::printJsonToCsv(response, toPrint);
	else if (options.outputConfig.outputFormat == "json")
		PTreePrinter::printStringToJson(response, toPrint);
	else if (options.outputConfig.outputFormat == "xml")
		PTreePrinter::printJsonToXml(response, toPrint);

	return 0;
}
