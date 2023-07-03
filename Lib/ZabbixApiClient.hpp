#ifndef CLIENT_HPP  
# define CLIENT_HPP

#include <iostream>
#include <string>
#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>


namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;
namespace pt = boost::property_tree;

class ZabbixApiClient
{
    private:

        net::io_context _ioc;
        tcp::socket _socket;
        std::string _apiUrl;
        std::string _apiPath;
        std::string _username;
        std::string _password;
        std::string _authToken;

    public:

        ZabbixApiClient(const std::string& apiUrl, const std::string& apiPath, const std::string& username, const std::string& password);
        ~ZabbixApiClient();

        http::request<http::string_body> prepareRequest(const http::verb& type, const std::string& method, const pt::ptree& params);
        std::string sendRequest(const http::request<http::string_body>& req);


    private:
    
        void connect(void);
        void disconnect(void);
        void login(void);
        void logout(void);
};


#endif
