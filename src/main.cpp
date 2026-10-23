#include <iostream>
#include <string_view>

#include "http/socket.hpp"

int main()
{
    try {
        forge::http::Socket server =
            forge::http::Socket::createTCP();

        server.bind("127.0.0.1", 8080);
        server.listen();

        std::cout << "Forge listening on http://127.0.0.1:8080\n";

        forge::http::Socket client = server.accept();

        std::cout << "Client connected\n";

        char buffer[4096];

        const ssize_t bytesReceived =
            client.receive(buffer, sizeof(buffer));

        if (bytesReceived > 0) {
            std::cout << "Received request:\n";
            std::cout.write(buffer, bytesReceived);
            std::cout << '\n';
        } else {
            std::cout << "Client closed the connection\n";
            return 0;
        }

        constexpr std::string_view response =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: 12\r\n"
            "Connection: close\r\n"
            "\r\n"
            "Hello Forge!\n";

        client.sendAll(response.data(), response.size());

    }
    catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }

    return 0;
}