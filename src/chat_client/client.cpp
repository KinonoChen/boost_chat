#include "client.h"

ChatClient::ChatClient(const std::string& host, const std::string& port, boost::asio::io_context& io_context)
    : resolver_(io_context), endpoints(resolver_.resolve(tcp::v4(), host, port)) {
        do_connect(endpoints);
}

bool ChatClient::register_user(const std::string& username, const std::string& password,tcp::socket& socket){
    std::string register_request = "REGISTER " + username + " " + password;
    send_message(socket, register_request);
    auto reply = read_response(socket, "\n");
    if (reply == "Registration successful") {
        std::cout << "Now you can log in with your credentials." << std::endl;
        //option = "login";
        return true;
    } else {
        std::cout << reply << std::endl;
        return false;
    }
};

bool ChatClient::login_user(const std::string& username, const std::string& password,tcp::socket& socket){
    send_message(socket, "LOGIN");
    send_message(socket, username, password);
    auto reply = read_response(socket, "\n");
    if (reply.find("Login failed") != std::string::npos) {
            std::cerr << "Login failed. Exiting...\n";
            return false;
    }else if(reply.find("Login successful") != std::string::npos){
        std::cout << "Login successful\n";
        return true;
    }else{
        return false;
    }

};

void ChatClient::send_message(tcp::socket& socket,const std::string& message)
{
    boost::asio::write(socket, boost::asio::buffer(message + "\n"));
};

void ChatClient::send_message(tcp::socket& socket,const std::string& message_a, const std::string& message_b){
    //std::cout << "Sending message: " << message_a << " " << message_b << std::endl;
    boost::asio::write(socket, boost::asio::buffer(message_a + "\n"));
    boost::asio::write(socket, boost::asio::buffer(message_b + "\n"));
};

void ChatClient::echo_message(tcp::socket& socket){
    for (std::string message; std::getline(std::cin, message);) {
        if(message.size() == 0){
            continue;
        }
        send_message(socket, message);
        //if(socket.available() == 0){
            //  continue;
        //}
        auto reply = read_response(socket, "\n");
        std::cout << "Reply is: " << reply << std::endl;
        //std::cout.write(reply, reply.size());
    }
};

std::string ChatClient::read_response(tcp::socket& socket, const std::string& flag){
    boost::asio::streambuf buffer;
    boost::asio::read_until(socket, buffer, flag); 
    std::istream is(&buffer);
    std::string reply;
    std::getline(is, reply);
    return reply;
};

std::pair<std::string, std::string> ChatClient::get_credential(){
    std::string username, password;
    std::cout << "Enter username: ";
    std::cin >> username;
    std::cout << "Enter password: ";
    std::cin >> password;
    return {username, password};
};

void ChatClient::do_connect(tcp::resolver::results_type endpoints){
    tcp::socket socket(resolver_.get_executor());
    boost::asio::connect(socket, endpoints);
    handle_session(socket);
};

void ChatClient::handle_session(tcp::socket& socket){
    try
    {
        std::string option;
        std::cout << "Type 'register' to register or 'login' to login: ";
        std::cin >> option;

        if (option == "register") {
            auto [username, password] = get_credential();
            if(register_user(username, password,socket)){
                option = "login";
            }else{
                std::cerr << "Registration failed. Exiting...\n";
                return;
            }       
        }
        if (option == "login") {
            auto [username, password] = get_credential();
            if(login_user(username, password,socket)){
                echo_message(socket);
            }else{
                std::cerr << "Login failed. Exiting...\n";
                return;
            }
        }

    }catch (std::exception& e) {
    std::cerr << "Exception in thread: " << e.what() << std::endl;
    }
};