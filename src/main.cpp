#include <iostream>
#include "http/socket.hpp"
#include <system_error>

int main() {
    try{
        forge::http::Socket socket = 
            forge::http::Socket::createTCP();
        
        std::cout << "Socket created with fd: " << socket.get() << std::endl;
    }catch(const std::system_error& e){
        std::cerr << "Failed to create socket: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}