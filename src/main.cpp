#include "http/socket.hpp"

#include <cstdint>
#include <exception>
#include <iostream>
#include <system_error>

int main()
{
    try {
        forge::http::Socket server = forge::http::Socket::createTCP();
        std::cout << "Socket created with fd: "
                  << server.get() << '\n';

        server.bind("127.0.0.1", 8080);
        std::cout << "Socket bound to 127.0.0.1:8080\n";

        server.listen();
        std::cout << "Server is listening\n";

        forge::http::Socket client = server.accept();

        std::cout << "Client connected! FD: "
                  << client.get() << '\n';
        while (true) {
            char buffer[1024];
            ssize_t bytesReceived = client.receive(buffer, sizeof(buffer));
            if (bytesReceived > 0) {
                std::cout << "Received " << bytesReceived << " bytes: "
                          << std::string(buffer, bytesReceived) << '\n';
            } else {
                std::cout << "Client disconnected\n";
                break;
            }
        }
    }
    catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }

    return 0;
}