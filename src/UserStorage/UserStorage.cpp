#include "UserStorage.hpp"

std::optional<errors::Error> UserStorage::AddUser(User &user)
{
    user.uuid = utils::GenerateUUIDv4();

    LockGuard lk(_storage_mtx);

    if (_LoginIsExist(user.login))
        return errors::Error(LOGIN_CONFLICT, 409);

    _storage.emplace(user);

    return std::nullopt;
}

std::optional<errors::Error> UserStorage::DeleteUser(const std::string &uuid)
{
    LockGuard lk(_storage_mtx);

    auto it = _uuid_index.find(uuid);
    if (it == _uuid_index.end())
        return errors::Error(UNKNOWN_USER, 404);

    _uuid_index.erase(it);

    return std::nullopt;
}

std::variant<errors::Error, User> UserStorage::UpdateUser(const std::string &uuid, const nlohmann::json &j)
{
    LockGuard lk(_storage_mtx);

    User user;
    {
        auto it = _uuid_index.find(uuid);

        if (it == _uuid_index.end())
            return errors::Error(UNKNOWN_USER, 404);

        user = it.get_node()->value();
    }

    if (j.contains("password") && j.at("password").is_string())
        user.password = j["password"];

    if (j.contains("login") && j.at("login").is_string())
    {
        std::string value = j["login"];
        if (user.login != value)
        {
            if (_LoginIsExist(value))
                return errors::Error(LOGIN_CONFLICT, 409);

            user.login = value;
        }
    }

    if (j.contains("access") && j.at("access").is_string())
    {
        auto access = StrToAccess(j["access"]);
        if (access == User::AccessLevel::undef)
            return errors::Error(UNKNOWN_ACCESS, 400);

        user.access = access;
    }

    _uuid_index.erase(_uuid_index.find(uuid));
    _storage.insert(user);

    return user;
}

std::optional<User> UserStorage::GetByUuid(const std::string &uuid)
{
    LockGuard lk(_storage_mtx);

    auto it = _uuid_index.find(uuid);

    if (it == _uuid_index.end())
        return std::nullopt;

    auto value = it.get_node()->value();
    return value;
}

std::vector<User> UserStorage::GetAll()
{
    LockGuard lk(_storage_mtx);

    return std::vector<User>(_storage.begin(), _storage.end());
}

std::optional<User> UserStorage::Auth(const std::string &login, const std::string &password)
{
    LockGuard lk(_storage_mtx);

    auto user = _GetByLogin(login);

    if (!user.has_value())
        return std::nullopt;

    if (user.value().password != password)
        return std::nullopt;

    return user.value();
}

bool UserStorage::_LoginIsExist(const std::string &login) NO_THREAD_SAFE
{
    auto it = _login_index.find(login);

    if (it == _login_index.end())
        return false;

    return true;
}

std::optional<User> UserStorage::_GetByLogin(const std::string &login) NO_THREAD_SAFE
{
    auto it = _login_index.find(login);

    if (it == _login_index.end())
        return std::nullopt;

    auto value = it.get_node()->value();
    return value;
}