#pragma once
#include <http/socket.hpp>
#include <http/request.hpp>
#include <string>

namespace forge::http{
    class Parser{
    
        void parseRequestLine(const std::string& line, Request& request);
        std::pair<std::string, std::string> parseHeaderLine(const std::string& line);

    public:
        Request parseRequest(const std::string& requestString);
    };
}