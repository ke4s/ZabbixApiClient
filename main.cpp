#include "Lib/ZabbixApiClient.hpp"
#include "Lib/GetProgramOptions.hpp"
#include "Lib/PrepareRequestFromOpt.hpp"
#include "Lib/PTreePrinter.hpp"
	
#include <vector>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;
namespace pt = boost::property_tree;

int main(int argc, char** argv)
{
	try
	{

		ProgramOptions options;

		GetProgramOptions::getOptions(argc, argv, options);
		
		//std::cout << options << std::endl;
		
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
			PTreePrinter::printJsonToCsv(response, toPrint);



		return 0;
	}
	catch(std::exception& e)
	{
		std::cout << "ERROR: " << e.what() << std::endl;
	}
}



