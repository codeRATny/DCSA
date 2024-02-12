#include "crow_all.h"

int main()
{
    crow::SimpleApp app;

    CROW_ROUTE(app, "/hello")([]()
    {
        crow::response resp;
        resp.body = crow::json::wvalue({{"msg", "Hello world"}}).dump();
        resp.code = 200;
        resp.add_header("Content-Type", "application/json");
        return resp;
    });

    CROW_ROUTE(app, "/hello").methods("POST"_method)([](const crow::request& req) 
    {
        crow::response resp;
        auto request = crow::json::load(req.body);
        if (!request)
        {
            resp.body = crow::json::wvalue({{"error", "Invalid JSON"}}).dump();
            resp.code = 400;
            resp.add_header("Content-Type", "application/json");
            return resp;
        }

        resp.body = req.body;
        resp.code = 200;
        resp.add_header("Content-Type", "application/json");
        return resp;
    });

    app.port(7777).multithreaded().run();

    return 0;
}