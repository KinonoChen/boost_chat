#include "chat_server/server.h"

int main(int argc, char* argv[]) {
    try {
        if (argc != 2) {
            std::cerr << "Usage: ChatServer <port>\n";
            return 1;
        }

        boost::asio::io_context io_context;
        ThreadPool thread_pool(2);  // 创建一个包含4个线程的线程池

        ChatServer server(io_context, std::atoi(argv[1]), thread_pool, "tcp://127.0.0.1:3306", "root", "1111", "my_chat_app");

        //io_context.run();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}