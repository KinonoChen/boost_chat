#include "server.h"

ChatServer::ChatServer(boost::asio::io_context& io_context, short port, ThreadPool& pool,
                        const std::string& host, const std::string& user, const std::string& password, const std::string& db)
        : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)), thread_pool_(pool), db(host, user, password, db) {
        do_accept();
}

bool ChatServer::register_user(const std::string& request, std::string& response) {
    auto [username, password] = extract_credentials(request);
    std::cout << "username: " << username << " password: " << password << std::endl;
    std::lock_guard<std::mutex> lock(mysql_mutex);

    if (!db.is_user_exists(username)) {
        db.save_user(username, password);
        response = "Registration successful\n";
        return true;
    } else {
        response = "Username already exists\n";
        return false;
    }
}

bool ChatServer::login_user(const std::string& username, const std::string& password, std::string& response) {
    std::lock_guard<std::mutex> lock(mysql_mutex);
    if (db.validate_user(username, password)) {
        response = "Login successful\n";
        return true;
    } else {
        response = "Login failed\n";
        return false;
    }
}

void ChatServer::do_accept(){
    for (;;) {
        try {
            tcp::socket socket(acceptor_.get_executor());
            acceptor_.accept(socket);
            std::cout << "New connection accepted." << std::endl;

            auto socket_ptr = std::make_shared<tcp::socket>(std::move(socket));
            thread_pool_.execute([this, socket_ptr]() mutable {
                handle_session(*socket_ptr);
            });
        } catch (const boost::system::system_error& e) {
            std::cerr << "Error during accept: " << e.what() << std::endl;
            continue;
        }
    }
}

void ChatServer::handle_session(tcp::socket& socket) {
    try {
        for (;;) {
            std::string request = read_request(socket);
            if (request.find("REGISTER") != std::string::npos) {
                std::string response;
                register_user(request, response);
                send_response(socket, response);
            } else if (request.find("LOGIN") != std::string::npos) {
                std::string username, password;
                request = read_request(socket); // 读取用户名
                username = request;
                //std::cout << "Received username: " << username << std::endl;    
                request = read_request(socket); // 读取密码
                password = request;
               // std::cout << "Received password: " << password << std::endl;

                std::string response;
                username.erase(username.find_last_not_of(" \n\r\t") + 1);
                password.erase(password.find_last_not_of(" \n\r\t") + 1);
                if (login_user(username, password, response)) {
                    send_response(socket, response);
                    echo_messages(socket);
                    
                } else {
                    send_response(socket, response);
                }
            } else {
                send_response(socket, "Invalid request\n");
            }
        }
    } catch (std::exception& e) {
        std::cerr << "Exception in thread: " << e.what() << std::endl;
    }
}
std::string ChatServer::read_request(tcp::socket& socket) {
    char data[1024];
    boost::system::error_code error;
    size_t length = socket.read_some(boost::asio::buffer(data), error);
    if (error) throw boost::system::system_error(error);
    return std::string(data, length);
}

void ChatServer::send_response(tcp::socket& socket, const std::string& response) {
    boost::asio::write(socket, boost::asio::buffer(response));
}

void ChatServer::echo_messages(tcp::socket& socket) {
    char data[1024];
    for (;;) {
        boost::system::error_code error;
        //if (socket.available() == 0) {
            //   continue;
        // }
        size_t length = socket.read_some(boost::asio::buffer(data), error);
        if (error == boost::asio::error::eof)
            break;
        else if (error)
            throw boost::system::system_error(error);

        boost::asio::write(socket, boost::asio::buffer(data, length));
    }
}

std::pair<std::string, std::string> ChatServer::extract_credentials(const std::string& request) {
    std::string username = request.substr(9, request.find(' ', 9) - 9);
    std::string password = request.substr(request.find(' ', 9) + 1);
    username.erase(username.find_last_not_of(" \n\r\t") + 1);
    password.erase(password.find_last_not_of(" \n\r\t") + 1);
    return {username, password};
}
