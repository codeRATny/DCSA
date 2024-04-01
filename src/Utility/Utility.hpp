
#ifndef _UTILITY_HPP_
#define _UTILITY_HPP_

#include <cstdint>
#include <string>
#include <vector>

namespace utils
{
    std::string EncodeBase64(const std::string &txt);

    std::string DecodeBase64(const std::string &txt);

    std::vector<std::string> Split(const std::string &s, const char *delim);
}

#endif