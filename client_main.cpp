#include "chat_client/client.h"

int main(int argc, char* argv[]) {
    try {
        if (argc != 3) {
            std::cerr << "Usage: ChatClient <host> <port>\n";
            return 1;
        }

        boost::asio::io_context io_context;
        ChatClient client(argv[1], argv[2], io_context);
        
        //std::thread t([&io_context](){ io_context.run(); });
        //t.join();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
