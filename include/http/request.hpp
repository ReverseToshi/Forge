#pragma once

#include <unordered_map>
#include <string>

namespace forge::http{
    struct Request{
        std::string method;
        std::string path;
        std::string version;
        std::unordered_map<std::string, std::string> headers;
    };
}