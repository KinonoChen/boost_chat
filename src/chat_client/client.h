#ifndef CLIENT_H
#define CLIENT_H
#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class ChatClient { 
public:
    ChatClient(const std::string& host, const std::string& port, boost::asio::io_context& io_context);
    
    bool register_user(const std::string& username, const std::string& password, tcp::socket& socket);
    bool login_user(const std::string& username, const std::string& password, tcp::socket& socket);
    void send_message(tcp::socket& socket, const std::string& message);
    void send_message(tcp::socket& socket, const std::string& message_a, const std::string& message_b);
    void echo_message(tcp::socket& socket);
    std::string read_response(tcp::socket& socket, const std::string& flag);
    std::pair<std::string, std::string> get_credential();
    void do_connect(tcp::resolver::results_type endpoints);
    void handle_session(tcp::socket& socket);

private:
    tcp::resolver resolver_;
    tcp::resolver::results_type endpoints;
};

#endif // CLIENT_H