#include <iostream>
#include "json.hpp"

int main()
{
    nlohmann::json j;

    // Basig types
    j["int"] = 123;
    j["bool"] = false;
    j["float"] = 3.14;
    j["string"] = "test";

    // Example for vector of strings
    std::vector<std::string> v;
    v.emplace_back("word1");
    v.emplace_back("word2");
    j["vector"] = v;

    // Example for map string:int
    std::unordered_map<std::string, int> m;
    m["value1"] = 32;
    m["value2"] = -5;
    j["map"] = m;

    std::cout << j.dump() << std::endl;

    return 0;
}