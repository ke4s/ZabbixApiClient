#ifndef PARSE_HPP
# define PARSE_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <boost/program_options.hpp>

namespace po = boost::program_options;

typedef struct s_ProgramOptions {
    std::string url;
    std::string username;
    std::string password;
    std::string method;
    std::vector<std::string> keys;
    std::vector<std::string> filters;
    std::string file;		
}	ProgramOptions;



class Parse {
public:

    static void parseFromArgs(int argc, char** argv, ProgramOptions &options)
	{
		try {
			po::options_description desc("Allowed options");
			desc.add_options()
				("help,h", "produce help message")
				("url,u", po::value<std::string>(&options.url)->required(), "The Zabbix API's url")
				("username,n", po::value<std::string>(&options.username)->required(), "The Zabbix API's Username")
				("password,p", po::value<std::string>(&options.password)->required(), "The Zabbix API's Password")
				("method,m", po::value<std::string>(&options.method)->required(), "The 'get' method, that you want to use.")
				("key,k", po::value<std::vector<std::string>>(&options.keys), "Keys, that you want to recieve their datas.\n\tUsage: key key1.key2 key1.key2.key3\n")
				("filter", po::value<std::vector<std::string>>(&options.filters), "Filter the data.\n\tUsage: --filter key:data\n")
				("file", po::value<std::string>(&options.file), "File");
					
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
	}

    static void parseFromFile(const std::string& filename, ProgramOptions& options)
	{
		try
		{
			po::options_description desc("Allowed options");
			desc.add_options()
				("url", po::value<std::string>(&options.url)->required(), "URL")
				("username", po::value<std::string>(&options.username)->required(), "Username")
				("password", po::value<std::string>(&options.password)->required(), "Password")
				("method", po::value<std::string>(&options.method)->required(), "Method")
				("key", po::value<std::vector<std::string>>(&options.keys), "Keys")
				("filter", po::value<std::vector<std::string>>(&options.filters), "Filters");

			std::ifstream settings_file(filename.c_str());
			if (!settings_file) {
				throw std::runtime_error("Could not open file: " + filename);
			}

			po::variables_map vm;
			po::store(po::parse_config_file(settings_file, desc), vm);
			po::notify(vm);

			
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


std::ostream& operator<<(std::ostream& os, const ProgramOptions& options) {
    os << "URL: " << options.url << std::endl;
    os << "Username: " << options.username << std::endl;
    os << "Password: " << options.password << std::endl;
    os << "Method: " << options.method << std::endl;
    os << "Keys: ";
    for (const auto& key : options.keys) {
        os << key << " ";
    }
    os << std::endl;
    os << "Filters: ";
    for (const auto& filter : options.filters) {
        os << filter << " ";
    }
    os << std::endl;
    os << "File: " << options.file << std::endl;
    
    return os;
}





#endif