#ifndef _USER_HPP_
#define _USER_HPP_

#include <string>

struct User final
{
    enum AccessLevel
    {
        undef = -1,
        user,
        moderator,
        admin
    };

    std::string uuid;
    std::string login;
    std::string password;
    AccessLevel access;

    struct GetByUuid;
    struct GetByLogin;
};

[[maybe_unused]] static std::string AccessToStr(User::AccessLevel lvl)
{
    switch (lvl)
    {
    case User::AccessLevel::user:
        return "user";
    case User::AccessLevel::moderator:
        return "moderator";
    case User::AccessLevel::admin:
        return "admin";
    case User::AccessLevel::undef:
        return "undef";
    }

#if __GNUC__ || __clang__
    __builtin_unreachable();
#endif
}

[[maybe_unused]] static User::AccessLevel StrToAccess(const std::string &lvl_str)
{
    if (lvl_str == "user")
        return User::AccessLevel::user;
    else if (lvl_str == "moderator")
        return User::AccessLevel::moderator;
    else if (lvl_str == "admin")
        return User::AccessLevel::admin;

    return User::AccessLevel::undef;
}

#endif