#ifndef PARSE_HPP
# define PARSE_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <boost/program_options.hpp>

namespace po = boost::program_options;

typedef struct s_ApiConfig
{
	std::string url;
	std::string apiPath;  //def /zabbix/api_jsonrpc.php
    std::string username;
    std::string password;
	int timeoutCon; //ms
} ApiConfig;

typedef struct s_ReqConfig
{
	std::string reqMethod; //int yapabiliriz.
    std::string method;
	std::vector<std::string> parameters;
    std::vector<std::string> outputs; // output olmalı
    std::vector<std::string> filters;
	int timeoutReq; //ms
} ReqConfig;

typedef struct s_DataBaseConfig
{
	std::string url;
	std::string username;
    std::string password;
} DataBaseConfig;

typedef struct s_OutputConfig
{
	std::string	outputFormat;
	std::vector<std::string> keyToPrint; //programın çıktısında gösterilecek olan veriler. Default olarak hepsi
} OutputConfig;

typedef struct s_ProgramOptions
{
	ApiConfig apiConfig;
	ReqConfig reqConfig;
	DataBaseConfig dbConfig;
	OutputConfig	outputConfig;
    
	std::string file;
	std::string jsonFile;

}	ProgramOptions;



class GetProgramOptions {
public:

	static void getFromArgs(int argc, char** argv, ProgramOptions &options)
	{	
		/*
		try {
			po::options_description desc("Allowed options");
			desc.add_options()
				("help,h", "produce help message")
				("url,u", po::value<std::string>(&options.url)->required(), "The Zabbix API's url")
				("username,n", po::value<std::string>(&options.username)->required(), "The Zabbix API's Username")
				("password,p", po::value<std::string>(&options.password)->required(), "The Zabbix API's Password")
				("method,m", po::value<std::string>(&options.method)->required(), "The 'get' method, that you want to use.")
				("param,par", po::value<std::vector<std::string>>(&options.parameters), "(OPT) Parameters, that you want to use parameters for request.\n\tUsage: param:value\n")
				("key,k", po::value<std::vector<std::string>>(&options.keys), "(OPT) Keys, that you want to recieve their datas.\n\tUsage: key key1.key2 key1.key2.key3\n")
				("filter", po::value<std::vector<std::string>>(&options.filters), "(OPT) Filter the data.\n\tUsage: --filter key:data\n")
				("file", po::value<std::string>(&options.file), "(OPT) File to read arguments")
				("output,o", po::value<std::string>(&options.outputFormat)->default_value(std::string("csv")), "Output format. You can use CSV JSON XML");
					
			po::variables_map vm;
			po::store(po::parse_command_line(argc, argv, desc), vm);

			if (vm.count("help")) {
				std::cout << desc << "\n";
				exit(0);
			}

			po::notify(vm);

			// If "file" is provided, no other option should be present
			if (!options.file.empty() && (vm.size() > 2))
				throw std::runtime_error("If 'file' option is provided, no other options should be present");

			if (options.outputFormat != "csv" && options.outputFormat != "xml" && options.outputFormat != "json")
				throw std::runtime_error("output format can only be internal type: json xml csv");

		}
		catch (po::required_option& e)
		{
			std::cerr << "ERROR: " << e.what() << std::endl << std::endl;
			//std::cerr << desc;
			exit(1);
		}
		catch (std::exception& e)
		{
			std::cerr << "Unhandled Exception reached the top of main: "
					<< e.what() << ", application will now exit" << std::endl;
			exit(2);
		}
		*/
		exit(6); // kaldırmayı unutma
	}

    static void getFromFile(const std::string& filename, ProgramOptions& options)
	{
		try
		{
			options.file = filename;
			po::options_description desc("Allowed options");
			desc.add_options()
				("help", "produce help message")
				
				("url", po::value<std::string>(&options.apiConfig.url)->required(), "The Zabbix API's url")
				("apipath", po::value<std::string>(&options.apiConfig.apiPath)->default_value(std::string("/zabbix/api_jsonrpc.php")), "The Zabbix API's path def = \"/zabbix/api_jsonrpc.php\"")
				("username", po::value<std::string>(&options.apiConfig.username)->required(), "The Zabbix API's Username")
				("password", po::value<std::string>(&options.apiConfig.password)->required(), "The Zabbix API's Password")

				("method", po::value<std::string>(&options.reqConfig.method)->required(), "The 'get' method, that you want to use.")
				("reqmethod,", po::value<std::string>(&options.reqConfig.reqMethod)->default_value(std::string("def")), "request method")
				("param,par", po::value<std::vector<std::string>>(&options.reqConfig.parameters), "(OPT) Parameters, that you want to use parameters for request.\n\tUsage: param:value\n")
				("output", po::value<std::vector<std::string>>(&options.reqConfig.outputs), "(OPT) Keys, that you want to recieve their datas.\n\tUsage: key key1.key2 key1.key2.key3\n")
				("filter", po::value<std::vector<std::string>>(&options.reqConfig.filters), "(OPT) Filter the data.\n\tUsage: --filter key:data\n")
				
				("outputformat", po::value<std::string>(&options.outputConfig.outputFormat)->default_value(std::string("csv")), "Output format. You can use CSV JSON XML")
				("keytoprint", po::value<std::vector<std::string>>(&options.outputConfig.keyToPrint), "Key to print")

				("file", po::value<std::string>(&options.file), "(OPT) File to read arguments")
				("jsonfile", po::value<std::string>(&options.jsonFile), "(OPT) File to read Request");

			std::ifstream settings_file(filename.c_str());
			if (!settings_file) {
				throw std::runtime_error("Could not open file: " + filename);
			}
			po::variables_map vm;
			po::store(po::parse_config_file(settings_file, desc), vm);
			po::notify(vm);

			if (options.outputConfig.outputFormat != "csv" && options.outputConfig.outputFormat != "xml" && options.outputConfig.outputFormat != "json")
				throw std::runtime_error("output format can only be internal type: json xml csv");

			
		}
		catch (std::ifstream::failure& e)
		{
			std::cerr << "ERROR: " << e.what() << std::endl;
			std::cerr << "Failed to open or read file: " << filename << std::endl;
			exit(1);
		}
		catch (po::required_option& e) 
		{
			std::cerr << "ERROR: " << e.what() << std::endl << std::endl;
			// std::cerr << desc;
			exit(1);
		}
		catch (std::exception& e)
		{
			std::cerr << "Unhandled Exception reached the top of main: "
					<< e.what() << ", application will now exit" << std::endl;
			exit(2);
		}

	}
};

#endif