#include "Routers.hpp"
#include "UserStorage.hpp"
#include "JsonCast.hpp"
#include "Utility.hpp"

std::optional<User> Auth(const std::string &header)
{
    User user;

    if (header.empty())
        return std::nullopt;

    auto splited_header = utils::Split(header, " ");
    if (splited_header.size() == 2 && splited_header.front() == "Basic")
    {
        auto decode = utils::DecodeBase64(splited_header.back());
        auto splited_auth = utils::Split(decode, ":");

        if (splited_auth.size() != 2)
            throw errors::ErrorException(UNAUTH, 401);

        auto user_opt = UserStorage::Instance().Auth(splited_auth.front(), splited_auth.back());

        if (!user_opt.has_value())
            throw errors::ErrorException(INVALID_AUTH, 401);

        user = user_opt.value();
    }
    else
    {
        throw errors::ErrorException(UNAUTH, 401);
    }

    return user;
}

void route::RegisterResources(hv::HttpService &router)
{
    router.GET("/api/v1/user", [](HttpRequest *req, HttpResponse *resp)
    {
        nlohmann::json response;

        try
        {
            auto current_user = Auth(req->GetHeader("Authorization"));

            if (!current_user.has_value())
            {
                errors::Error err(ACCESS_DENIED);
                resp->SetBody(err.ToJson());
                resp->content_type = APPLICATION_JSON;
                return 403;
            }
            else if (current_user.value().access < User::AccessLevel::moderator)
            {
                errors::Error err(ACCESS_DENIED);
                resp->SetBody(err.ToJson());
                resp->content_type = APPLICATION_JSON;
                return 403;
            }

            response = UserStorage::Instance().GetAll();
        }
        catch(const errors::ErrorException& e)
        {
            errors::Error err(e.error_code());
            resp->SetBody(err.ToJson());
            resp->content_type = APPLICATION_JSON;
            return e.http_code();
        }
        catch(const std::exception& e)
        {
            response["error"] = "Invalid JSON";
            resp->SetBody(response.dump());
            resp->content_type = APPLICATION_JSON;
            return 400;
        }

        resp->SetBody(response.dump());
        resp->content_type = APPLICATION_JSON;

        return 200;
    });

    router.GET("/api/v1/user/{id}", [](HttpRequest *req, HttpResponse *resp)
    {
        auto user_id = req->GetParam("id");
        nlohmann::json response;

        try
        {
            auto current_user = Auth(req->GetHeader("Authorization"));

            if (!current_user.has_value())
            {
                errors::Error err(ACCESS_DENIED);
                resp->SetBody(err.ToJson());
                resp->content_type = APPLICATION_JSON;
                return 403;
            }
            else if (current_user.value().access < User::AccessLevel::moderator && current_user.value().uuid != user_id)
            {
                errors::Error err(ACCESS_DENIED);
                resp->SetBody(err.ToJson());
                resp->content_type = APPLICATION_JSON;
                return 403;
            }

            auto res = UserStorage::Instance().GetByUuid(user_id);

            if (res.has_value())
            {
                response = res.value();
                resp->SetBody(response.dump());
            }
            else
            {
                errors::Error err(UNKNOWN_USER);
                resp->SetBody(err.ToJson());
                resp->content_type = APPLICATION_JSON;
                return 404;
            }
        }
        catch(const errors::ErrorException& e)
        {
            errors::Error err(e.error_code());
            resp->SetBody(err.ToJson());
            resp->content_type = APPLICATION_JSON;
            return e.http_code();
        }
        catch(const std::exception& e)
        {
            response["error"] = "Invalid JSON";
            resp->SetBody(response.dump());
            resp->content_type = APPLICATION_JSON;
            return 400;
        }

        resp->content_type = APPLICATION_JSON;
        return 200;
    });

    router.POST("/api/v1/user", [](HttpRequest *req, HttpResponse *resp)
    {
        nlohmann::json request;
        nlohmann::json response;

        try
        {
            auto current_user = Auth(req->GetHeader("Authorization"));

            User user = nlohmann::json::parse(req->body);

            if (!current_user.has_value() && user.access > User::AccessLevel::user)
            {
                errors::Error err(ACCESS_DENIED);
                resp->SetBody(err.ToJson());
                resp->content_type = APPLICATION_JSON;
                return 403;
            }
            else if (current_user.has_value() && current_user.value().access < user.access)
            {
                errors::Error err(ACCESS_DENIED);
                resp->SetBody(err.ToJson());
                resp->content_type = APPLICATION_JSON;
                return 403;
            }

            auto err = UserStorage::Instance().AddUser(user);

            if (err.has_value())
            {
                resp->SetBody(err.value().ToJson());
                resp->content_type = APPLICATION_JSON;
                return err.value().http_code;
            }
            else
            {
                response = user;
                resp->SetBody(response.dump());
                resp->content_type = APPLICATION_JSON;
            }
        }
        catch(const errors::ErrorException& e)
        {
            errors::Error err(e.error_code(), e.http_code());
            resp->SetBody(err.ToJson());
            resp->content_type = APPLICATION_JSON;
            return e.http_code();
        }
        catch(const std::exception& e)
        {
            response["error"] = "Invalid JSON";
            resp->SetBody(response.dump());
            resp->content_type = APPLICATION_JSON;
            return 400;
        }

        return 200;
    });

    router.PATCH("/api/v1/user/{id}", [](HttpRequest *req, HttpResponse *resp)
    {
        nlohmann::json request;
        nlohmann::json response;

        auto user_id = req->GetParam("id");

        try
        {
            auto current_user = Auth(req->GetHeader("Authorization"));
            if (!current_user.has_value())
            {
                errors::Error err(ACCESS_DENIED);
                resp->SetBody(err.ToJson());
                resp->content_type = APPLICATION_JSON;
                return 403;
            }
            else if (current_user.value().access < User::AccessLevel::admin && current_user.value().uuid != user_id)
            {
                errors::Error err(ACCESS_DENIED);
                resp->SetBody(err.ToJson());
                resp->content_type = APPLICATION_JSON;
                return 403;
            }

            request = nlohmann::json::parse(req->body);

            auto res = UserStorage::Instance().UpdateUser(user_id, request);

            if (std::holds_alternative<errors::Error>(res))
            {
                auto err = std::get<errors::Error>(res);
                resp->SetBody(err.ToJson());
                resp->content_type = APPLICATION_JSON;
                return err.http_code;
            }
            else if (std::holds_alternative<User>(res))
            {
                auto user = std::get<User>(res);
                response = user;
                resp->SetBody(response.dump());
                resp->content_type = APPLICATION_JSON;
            }
        }
        catch(const errors::ErrorException& e)
        {
            errors::Error err(e.error_code());
            resp->SetBody(err.ToJson());
            resp->content_type = APPLICATION_JSON;
            return e.http_code();
        }
        catch(const std::exception& e)
        {
            response["error"] = "Invalid JSON";
            resp->SetBody(response.dump());
            resp->content_type = APPLICATION_JSON;
            return 400;
        }

        return 200;
    });

    router.Delete("/api/v1/user/{id}", [](HttpRequest *req, HttpResponse *resp)
    {
        nlohmann::json response;

        auto user_id = req->GetParam("id");

        try
        {
            auto current_user = Auth(req->GetHeader("Authorization"));
            if (!current_user.has_value())
            {
                errors::Error err(ACCESS_DENIED);
                resp->SetBody(err.ToJson());
                resp->content_type = APPLICATION_JSON;
                return 403;
            }
            else if (current_user.value().access < User::AccessLevel::admin && current_user.value().uuid != user_id)
            {
                errors::Error err(ACCESS_DENIED);
                resp->SetBody(err.ToJson());
                resp->content_type = APPLICATION_JSON;
                return 403;
            }

            auto res = UserStorage::Instance().DeleteUser(user_id);
            if (res.has_value())
            {
                errors::Error err(res.value().err_code);
                resp->SetBody(err.ToJson());
                resp->content_type = APPLICATION_JSON;
                return res.value().http_code;
            }
        }
        catch(const errors::ErrorException& e)
        {
            errors::Error err(e.error_code());
            resp->SetBody(err.ToJson());
            resp->content_type = APPLICATION_JSON;
            return e.http_code();
        }
        catch(const std::exception& e)
        {
            response["error"] = "Invalid JSON";
            resp->SetBody(response.dump());
            resp->content_type = APPLICATION_JSON;
            return 400;
        }

        return 200;
    });
}