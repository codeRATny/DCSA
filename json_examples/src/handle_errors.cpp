#include <iostream>
#include "json.hpp"

int main()
{
    std::string data = "{\"bool\":false,\"float\":3.14,\"int\":123,\"map\":{\"value1\":32,\"value2\":-5},\"string\":\"test\",\"vector\":[\"word1\",\"word2\"]}";
    std::string data_invalid = "{\"bool\":false,\"map\"\"value1\":32,\"value2\":-5},\"string\":\"test\",\"vector\":[\"word1\",\"word2\"]}";

    try
    {
        nlohmann::json j = nlohmann::json::parse(data_invalid);
    }
    catch(const nlohmann::json::parse_error& e)
    {
        std::cout << "Handle parse error" << std::endl;
    }
    
    nlohmann::json j = nlohmann::json::parse(data);

    try
    {
        int i = j["string"];
        (void)i;
    }
    catch(const nlohmann::json::type_error& e)
    {
        
        std::cout << "Handle type error" << std::endl;
    }

    try
    {
        int i = j.at("a");
        (void)i;
    }
    catch(const nlohmann::json::out_of_range& e)
    {
        
        std::cout << "Handle missing field" << std::endl;
    }

    return 0;
}