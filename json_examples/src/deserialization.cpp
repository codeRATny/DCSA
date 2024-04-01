#include <iostream>
#include "json.hpp"

int main()
{
    std::string data;

    // From serialization.cpp
    {
        nlohmann::json j;
        j["int"] = 123;
        j["bool"] = false;
        j["fload"] = 3.14;
        j["string"] = "test";

        std::vector<std::string> v;
        v.emplace_back("word1");
        v.emplace_back("word2");
        j["vector"] = v;

        std::unordered_map<std::string, int> m;
        m["value1"] = 32;
        m["value2"] = -5;
        j["map"] = m;

        data = j.dump();
    }

    // parse string to json
    nlohmann::json j = nlohmann::json::parse(data);

    std::cout << j["int"] << " | " << j["string"] << std::endl;

    return 0;
}