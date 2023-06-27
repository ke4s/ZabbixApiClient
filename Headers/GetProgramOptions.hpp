#ifndef PARSE_HPP
# define PARSE_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <boost/program_options.hpp>
#include <optional>


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
	std::string httpMethod; //int yapabiliriz.
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

std::ostream& operator<<(std::ostream& os, const ProgramOptions& options);

/*
	Option'ları almaya argv'den başlar.
	Argv'den alnınanlar en üst katmandır değiştirilemez.
	eğer file varsa file'ın içindeki argümanları önceden aynı argümanlar kaydedilmediyse kaydeder.
*/

class GetProgramOptions {
public:

	

	static void getOptions(int argc, char** argv, ProgramOptions &options)
	{
		po::variables_map vm;

		try {
			po::options_description desc("Allowed options");
			desc.add_options()
				("help", "produce help message")
				
				("url", po::value<std::string>(&options.apiConfig.url), "The Zabbix API's url")
				("apipath", po::value<std::string>(&options.apiConfig.apiPath), "The Zabbix API's path def = \"/zabbix/api_jsonrpc.php\"")
				("username", po::value<std::string>(&options.apiConfig.username), "The Zabbix API's Username")
				("password", po::value<std::string>(&options.apiConfig.password), "The Zabbix API's Password")

				("method", po::value<std::string>(&options.reqConfig.method), "The 'get' method, that you want to use.")
				("httpmethod,", po::value<std::string>(&options.reqConfig.httpMethod), "HTTP method")
				("param,par", po::value<std::vector<std::string>>(&options.reqConfig.parameters), "(OPT) Parameters, that you want to use parameters for request.\n\tUsage: param:value\n")
				("output", po::value<std::vector<std::string>>(&options.reqConfig.outputs), "(OPT) Keys, that you want to recieve their datas.\n\tUsage: key key1.key2 key1.key2.key3\n")
				("filter", po::value<std::vector<std::string>>(&options.reqConfig.filters), "(OPT) Filter the data.\n\tUsage: --filter key:data\n")
				
				("outputformat", po::value<std::string>(&options.outputConfig.outputFormat), "Output format. You can use CSV JSON. Default JSON")
				("keytoprint", po::value<std::vector<std::string>>(&options.outputConfig.keyToPrint), "Keys for CSV format.")

				("file", po::value<std::string>(&options.file), "(OPT) File to read arguments")
				("jsonfile", po::value<std::string>(&options.jsonFile), "(OPT) A JSON file for request's params.");
			
			po::store(po::parse_command_line(argc, argv, desc), vm);

			if (vm.count("help")) {
				std::cout << desc << "\n";
				exit(0);
			}

			po::notify(vm);


			if (vm.count("file"))
			{
				options = getFromFile(options.file);
				po::notify(vm); //argv'den aldıklarımızın öne çıkması için tekrar çalıştırıyorum. dosyadan okunanları eziyor bu
			}

			setDefaults(options);
			
			std::pair<std::string, bool> requiredCheck = requieredVarCheck(options);
			if(requiredCheck.second == false)
				throw std::runtime_error("ERROR: Required parameters missing.\n"  + requiredCheck.first + "\nUse --help for more information\n");

			//options check daha fazlası gelecek. keytoprint, filter formatları önemli onları buraya ekleyeceğim
			if (options.outputConfig.outputFormat != "csv" && options.outputConfig.outputFormat != "json")
				throw std::runtime_error("output format can only be internal type: json xml csv");

		}
		catch (std::runtime_error& e)
		{
			std::cerr << e.what() << ", application will now exit" << std::endl;
			exit(2);
		}
		catch (std::exception& e)
		{
			std::cerr << "Unhandled Exception reached the top of main: "
					<< e.what() << ", application will now exit" << std::endl;
			exit(2);
		}
	}
	private:

    static ProgramOptions getFromFile(const std::string& filename)
	{
		ProgramOptions options;
		try
		{
			options.file = filename;
			po::options_description desc("Allowed options");
			desc.add_options()				
				("url", po::value<std::string>(&options.apiConfig.url), "The Zabbix API's url")
				("apipath", po::value<std::string>(&options.apiConfig.apiPath), "The Zabbix API's path def = \"/zabbix/api_jsonrpc.php\"")
				("username", po::value<std::string>(&options.apiConfig.username), "The Zabbix API's Username")
				("password", po::value<std::string>(&options.apiConfig.password), "The Zabbix API's Password")

				("method", po::value<std::string>(&options.reqConfig.method), "The 'get' method, that you want to use.")
				("httpmethod,", po::value<std::string>(&options.reqConfig.httpMethod), "HTTP method")
				("param,par", po::value<std::vector<std::string>>(&options.reqConfig.parameters), "(OPT) Parameters, that you want to use parameters for request.\n\tUsage: param:value\n")
				("output", po::value<std::vector<std::string>>(&options.reqConfig.outputs), "(OPT) Keys, that you want to recieve their datas.\n\tUsage: key key1.key2 key1.key2.key3\n")
				("filter", po::value<std::vector<std::string>>(&options.reqConfig.filters), "(OPT) Filter the data.\n\tUsage: --filter key:data\n")
				
				("outputformat", po::value<std::string>(&options.outputConfig.outputFormat), "Output format. You can use CSV JSON. Default JSON")
				("keytoprint", po::value<std::vector<std::string>>(&options.outputConfig.keyToPrint), "Keys for CSV format.")

				("jsonfile", po::value<std::string>(&options.jsonFile), "(OPT) A JSON file for request's params.");

			std::ifstream settings_file(filename.c_str());
			if (!settings_file) {
				throw std::runtime_error("Could not open file: " + filename);
			}
			po::variables_map vm;
			po::store(po::parse_config_file(settings_file, desc), vm);
			po::notify(vm);


			return options;
		}
		catch (std::ifstream::failure& e)
		{
			std::cerr << "ERROR: " << e.what() << std::endl;
			std::cerr << "Failed to open or read file: " << filename << std::endl;
			exit(1);
		}
		catch (std::exception& e)
		{
			std::cerr << "Unhandled Exception reached the top of main: "
					<< e.what() << ", application will now exit" << std::endl;
			exit(2);
		}

	}

	//if there is a an error it will return errors and 1(bool value)
	static std::pair<std::string, bool> requieredVarCheck(const ProgramOptions& opt)
	{
		std::string requireds = "";

		if (opt.apiConfig.url.empty())
			requireds += "--url is reqiured\n";	
		if (opt.apiConfig.username.empty())
			requireds += "--username is reqiured\n";
		if (opt.apiConfig.password.empty())
			requireds += "--password is reqiured\n";
		if (opt.reqConfig.httpMethod.empty())
			requireds += "--httpmethod is reqiured\n";
		if (opt.reqConfig.method.empty())
			requireds += "--method is reqiured\n";
		
		return std::make_pair(requireds, (requireds.empty() ? true : false)); //
	}


	static void setDefaults(ProgramOptions& options)
	{
		if (options.apiConfig.apiPath.empty())
			options.apiConfig.apiPath = "/zabbix/api_jsonrpc.php";
		if (options.outputConfig.outputFormat.empty())
			options.outputConfig.outputFormat = "json";
	}
};


std::ostream& operator<<(std::ostream& os, const ApiConfig& config)
{
    os << "ApiConfig:\n";
    os << "URL: " << config.url << '\n';
    os << "API Path: " << config.apiPath << '\n';
    os << "Username: " << config.username << '\n';
    os << "Password: " << config.password << '\n';
    os << "Connection Timeout: " << config.timeoutCon << "ms\n";
    return os;
}

std::ostream& operator<<(std::ostream& os, const ReqConfig& config)
{
    os << "ReqConfig:\n";
    os << "Request Method: " << config.httpMethod << '\n';
    os << "Method: " << config.method << '\n';
    os << "Parameters: ";
    for (const auto& parameter : config.parameters)
    {
        os << parameter << ' ';
    }
    os << '\n';
    os << "Outputs: ";
    for (const auto& output : config.outputs)
    {
        os << output << ' ';
    }
    os << '\n';
    os << "Filters: ";
    for (const auto& filter : config.filters)
    {
        os << filter << ' ';
    }
    os << '\n';
    os << "Request Timeout: " << config.timeoutReq << "ms\n";
    return os;
}

std::ostream& operator<<(std::ostream& os, const DataBaseConfig& config)
{
    os << "DataBaseConfig:\n";
    os << "URL: " << config.url << '\n';
    os << "Username: " << config.username << '\n';
    os << "Password: " << config.password << '\n';
    return os;
}

std::ostream& operator<<(std::ostream& os, const OutputConfig& config)
{
    os << "OutputConfig:\n";
    os << "Output Format: " << config.outputFormat << '\n';
    os << "Keys to Print: ";
    for (const auto& key : config.keyToPrint)
    {
        os << key << ' ';
    }
    os << '\n';
    return os;
}

std::ostream& operator<<(std::ostream& os, const ProgramOptions& options)
{
    os << "ProgramOptions:\n";
    os << options.apiConfig;
    os << options.reqConfig;
    os << options.dbConfig;
    os << options.outputConfig;
    os << "File: " << options.file << '\n';
    os << "JSON File: " << options.jsonFile << '\n';
    return os;
}


#endif