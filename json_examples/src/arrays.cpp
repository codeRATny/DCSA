#include <iostream>
#include "json.hpp"

int main()
{
    nlohmann::json arr1 = nlohmann::json::array();

    arr1.push_back({"key", "value1"});
    arr1.push_back({"key", "value2"});
    arr1.push_back({"key", "value3"});

    nlohmann::json arr2 = nlohmann::json::array();

    arr2.push_back("value1");
    arr2.push_back("value2");
    arr2.push_back("value3");

    std::cout << arr1.dump() << std::endl;
    std::cout << arr2.dump() << std::endl;

    return 0;
}