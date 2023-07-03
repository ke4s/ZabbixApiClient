#ifndef GET_PROGRAM_OPTIONS_HPP
# define GET_PROGRAM_OPTIONS_HPP

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


/*
	Option'ları almaya argv'den başlar.
	Argv'den alnınanlar en üst katmandır değiştirilemez.
	eğer file varsa file'ın içindeki argümanları önceden aynı argümanlar kaydedilmediyse kaydeder.
*/

class GetProgramOptions
{
	public:

		static void getOptions(int argc, char** argv, ProgramOptions &options);

	private:

    	static ProgramOptions getFromFile(const std::string& filename);

		static std::pair<std::string, bool> requieredVarCheck(const ProgramOptions& opt);

		static void setDefaults(ProgramOptions& options);
};


std::ostream& operator<<(std::ostream& os, const ProgramOptions& options);
std::ostream& operator<<(std::ostream& os, const ApiConfig& config);
std::ostream& operator<<(std::ostream& os, const ReqConfig& config);
std::ostream& operator<<(std::ostream& os, const DataBaseConfig& config);
std::ostream& operator<<(std::ostream& os, const OutputConfig& config);


#endif