#ifndef _HTTP_SERVER_HPP_
#define _HTTP_SERVER_HPP_

#include "HttpServer.h"
#include "HttpService.h"

#include "Routers.hpp"

class HttpServer final
{
public:
using UPtr = std::unique_ptr<HttpServer>;

HttpServer();
HttpServer(const HttpServer &) = delete;
HttpServer(HttpServer &&) = delete;
~HttpServer();

void Start(int port);

private:

std::unique_ptr<hv::HttpServer> _server;
HttpService _router;
};

#endif
