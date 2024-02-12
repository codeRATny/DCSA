#include "Routers.hpp"

void route::RegisterResources(hv::HttpService &router)
{
    router.GET("/hello", [](HttpRequest *req, HttpResponse *resp)
    {
        nlohmann::json response
        {
            {"msg", "Hello world"}
        };

        resp->SetBody(response.dump());
        resp->content_type = APPLICATION_JSON;

        return 200;
    });

    router.POST("/hello", [](HttpRequest *req, HttpResponse *resp)
    {
        nlohmann::json request;
        nlohmann::json response;

        try
        {
            request = nlohmann::json::parse(req->body);
        }
        catch(const std::exception& e)
        {
            response["error"] = "Invalid JSON";
            resp->SetBody(response.dump());
            resp->content_type = APPLICATION_JSON;
            return 400;
        }
        
        resp->SetBody(request.dump());
        resp->content_type = APPLICATION_JSON;

        return 200;
    });

}