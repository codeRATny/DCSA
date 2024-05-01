#ifndef _JSON_CAST_HPP_
#define _JSON_CAST_HPP_

#include "json.hpp"

#include "User.hpp"
#include "GlobalErrors.hpp"

void to_json(nlohmann::json &j, const User &user);
void from_json(const nlohmann::json &j, User &user);

#endif