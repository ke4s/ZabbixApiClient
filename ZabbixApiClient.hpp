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
    // Boost.Asio'nun kullanacağı I/O context (giriş/çıkış bağlamı) oluşturulur.
    net::io_context _ioc;

    /*
    TCP adres çözümleyici oluşturulur.
    tcp::resolver _resolver;
    */
    
    // TCP soket oluşturulur.
    tcp::socket _socket;

    std::string _apiUrl;
    std::string _username;
    std::string _password;
    std::string _authToken;

public:

    ZabbixApiClient(const std::string& apiUrl, const std::string& username, const std::string& password):
    _ioc(), _socket(_ioc),
    _apiUrl(apiUrl), _username(username), _password(password)
    {
        connect();
        login();
    }


    ~ZabbixApiClient()
    {
        logout();
        disconnect();
    }
    
    std::string makeRequest();

private:
    void connect()
    {
        try
        {
            tcp::resolver resolver(_ioc);

            tcp::resolver::results_type end_point = resolver.resolve(_apiUrl, "http");
            boost::asio::connect(_socket, end_point);
        }
        catch(const std::exception& e)
        {
            std::cerr << "Connection Error: " << e.what() << '\n';
        }
    }

    void disconnect()
    {
        
    }

    login();
    logout();

    sendRequest();

};

#endif