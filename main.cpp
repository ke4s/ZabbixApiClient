#include "Headers/ZabbixApiClient.hpp"
#include "Headers/GetProgramOptions.hpp"
#include "Headers/utils.hpp"
#include "Headers/PTreePrinter.hpp"
	
#include <vector>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;
namespace pt = boost::property_tree;


void mergePropertyTrees(pt::ptree& dest, const pt::ptree& src);

pt::ptree prepareJsonBody(const ReqConfig& options);


int main(int argc, char** argv)
{
	try
	{

		ProgramOptions options;

		GetProgramOptions::getOptions(argc, argv, options);
		//std::cout << options << std::endl;
		
		//options check

		ZabbixApiClient client(options.apiConfig.url, options.apiConfig.apiPath, \
							options.apiConfig.username, options.apiConfig.password);

		pt::ptree body;
		if (!options.jsonFile.empty())
		{
			std::ifstream jsonfile(options.jsonFile.c_str());
			if (!jsonfile)
				throw std::runtime_error("Could not open file: " + options.jsonFile);
			pt::read_json(jsonfile, body);
		}
		else
			body = prepareJsonBody(options.reqConfig);

		//pt::write_json(std::cout, body);


		


		auto request = client.prepareRequest(http::string_to_verb(options.reqConfig.httpMethod), \
														options.reqConfig.method, body);
		
		//std::cout << request << std::endl  << std::endl << std::endl << std::endl;

		auto response = client.sendRequest(request);


		std::vector<std::string> &toPrint = (options.outputConfig.keyToPrint.empty() ? options.reqConfig.outputs : options.outputConfig.keyToPrint); //burada eğer  dizi boş ise tamamını bastırmalı.
		
		if (options.outputConfig.outputFormat == "json")
			PTreePrinter::printStringToJson(response);
		else if (options.outputConfig.outputFormat == "csv")
		{	
			/*
			std::istringstream is(response);
			pt::ptree jsonResponse;
			pt::read_json(is, jsonResponse);
			std::cout << PTreePrinter::jsonToCsv(jsonResponse.get_child("result"));
			*/
			PTreePrinter::printJsonToCsv(response, toPrint);
		}
		//else if (options.outputConfig.outputFormat == "xml")
		//	PTreePrinter::printJsonToXml(response, toPrint);

		return 0;
	}
	catch(std::exception& e)
	{
		std::cout << "ERROR: " << e.what() << std::endl;
	}
}







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