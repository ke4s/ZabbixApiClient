#include "ZabbixApiClient.hpp"

ZabbixApiClient::ZabbixApiClient(const std::string& apiUrl, const std::string& apiPath, \
const std::string& username, const std::string& password)
: _ioc(), _socket(_ioc), _apiUrl(apiUrl), _apiPath(apiPath), _username(username), _password(password)
{
    connect();
    login();
}


ZabbixApiClient::~ZabbixApiClient()
{
    logout();
    disconnect();
}



void ZabbixApiClient::connect(void)
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
    };
}

void ZabbixApiClient::disconnect(void)
{
    try {
        _socket.shutdown(tcp::socket::shutdown_both);
        _socket.close();
    } catch (const std::exception& e) {
        std::cerr << "Disconnect Error: " << e.what() << std::endl;
    };
}

void ZabbixApiClient::login(void)
{
    try
    {
        pt::ptree params;
        params.put("username", _username);
        params.put("password", _password);

        std::string response = sendRequest(prepareRequest(http::verb::get ,"user.login", params));

        // JSON yanıtı Boost.PropertyTree kullanılarak ayrıştırılır.
        pt::ptree jsonResponse;
        std::istringstream is(response);
        pt::read_json(is, jsonResponse);

        // Kimlik doğrulama tokeni alınır.
        _authToken = jsonResponse.get<std::string>("result");
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    };
}


void ZabbixApiClient::logout(void)
{
    if(_authToken.empty()) {
        return;
    }
    pt::ptree params;
    std::string response = sendRequest(prepareRequest(http::verb::get, "user.logout", params));
    _authToken.clear();
}


/*
    if the type is unkown it will throw an exception
*/
http::request<http::string_body> ZabbixApiClient::prepareRequest(const http::verb& type, const std::string& method, const pt::ptree& params)
{
    if (type == http::verb::unknown)
        throw std::runtime_error("HTTP method is unknown, use capital letters.");

    http::request<http::string_body> req{type, _apiPath, 11};
    req.set(http::field::host, _apiUrl);
    req.set(http::field::content_type, "application/json");

    pt::ptree root;
    root.put("jsonrpc", "2.0");
    root.put("method", method);
    root.add_child("params", params);
    root.put("id", 1);
    if(!_authToken.empty())
    {
        root.put("auth", _authToken);
    }

    std::ostringstream jsonStream;
    pt::write_json(jsonStream, root, false);
    req.body() = jsonStream.str();

    req.prepare_payload();

    return req;
}

std::string ZabbixApiClient::sendRequest(const http::request<http::string_body>& req)
{
    try {
        http::write(_socket, req);

        beast::flat_buffer buffer;
        http::response<http::dynamic_body> res;
        http::read(_socket, buffer, res);

        return beast::buffers_to_string(res.body().data());
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return "";
    }
}