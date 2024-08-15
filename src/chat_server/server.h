#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <boost/asio.hpp>
#include <thread>
#include "threadpool/threadpool.h"  // 假设您有一个自定义的线程池实现
#include <unordered_map>
#include <fstream>
#include "db/db_mannager.h"

using boost::asio::ip::tcp;

class ChatServer {
public:
    ChatServer(boost::asio::io_context& io_context, short port, ThreadPool& pool, 
                const std::string& host, const std::string& user, const std::string& password, const std::string& db);
    bool register_user(const std::string& request, std::string& response);
    bool login_user(const std::string& username, const std::string& password, std::string& response);
    void do_accept();
    void handle_session(tcp::socket& socket);
    std::string read_request(tcp::socket& socket);
    void send_response(tcp::socket& socket, const std::string& response);
    void echo_messages(tcp::socket& socket) ;
    std::pair<std::string, std::string> extract_credentials(const std::string& request);
private:
    DBManager db;
    std::mutex mysql_mutex;
    boost::asio::ip::tcp::acceptor acceptor_;
    ThreadPool& thread_pool_;
};

#endif // SERVER_H
