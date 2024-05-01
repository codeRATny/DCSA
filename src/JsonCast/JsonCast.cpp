#include "JsonCast.hpp"

void to_json(nlohmann::json &j, const User &user)
{
    j = nlohmann::json
    {
        {"id", user.uuid},
        {"access", AccessToStr(user.access)}
    };
}

void from_json(const nlohmann::json &j, User &user)
{
    j.at("login").get_to(user.login);
    j.at("password").get_to(user.password);

    auto access = StrToAccess(j.at("access"));
    if (access == User::AccessLevel::undef)
        throw errors::ErrorException(UNKNOWN_ACCESS, 400);

    user.access = access;
}