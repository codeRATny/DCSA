#ifndef _USER_STORAGE_HPP_
#define _USER_STORAGE_HPP_

#include <optional>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/hashed_index.hpp>

#include "json.hpp"

#include "Utility.hpp"
#include "User.hpp"
#include "GlobalDeffs.hpp"
#include "GlobalErrors.hpp"

using Users = boost::multi_index::multi_index_container<
        User,
        boost::multi_index::indexed_by<
            boost::multi_index::hashed_unique<
                boost::multi_index::tag<struct User::GetByUuid>,
                boost::multi_index::member<User, decltype(User::uuid), &User::uuid>
            >,
            boost::multi_index::hashed_unique<
                boost::multi_index::tag<struct User::GetByLogin>,
                boost::multi_index::member<User, decltype(User::login), &User::login>
            >
        >
    >;

class UserStorage final
{
public:
    static UserStorage &Instance()
    {
        static UserStorage inst;
        return inst;
    }

    std::optional<errors::Error> AddUser(User &user);
    std::optional<errors::Error> DeleteUser(const std::string &uuid);
    std::variant<errors::Error, User> UpdateUser(const std::string &uuid, const nlohmann::json &j);

    std::optional<User> GetByUuid(const std::string &uuid);

    std::vector<User> GetAll();

    std::optional<User> Auth(const std::string &login, const std::string &password);

private:
    UserStorage() : _login_index(_storage.get<User::GetByLogin>()),
                    _uuid_index(_storage.get<User::GetByUuid>())
    {

        User default_admin;
        default_admin.uuid = utils::GenerateUUIDv4();
        default_admin.login = "admin";
        default_admin.password = "admin";
        default_admin.access = User::AccessLevel::admin;

        _storage.emplace(default_admin);
    }

    ~UserStorage() = default;

    bool _LoginIsExist(const std::string &login);
    std::optional<User> _GetByLogin(const std::string &login);

    Mutex _storage_mtx;
    Users _storage GUARDED_BY(_storage_mtx);
    Users::index<User::GetByLogin>::type &_login_index GUARDED_BY(_storage_mtx);
    Users::index<User::GetByUuid>::type &_uuid_index GUARDED_BY(_storage_mtx);

};

#endif