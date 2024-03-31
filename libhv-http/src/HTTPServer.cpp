#include "HTTPServer.hpp"

#include <thread>

HttpServer::HttpServer()
{
_server = std::make_unique<hv::HttpServer>();
route::RegisterResources(_router);
_server->registerHttpService(&_router);
}

void HttpServer::Start(int port)
{
_server->setPort(port);
_server->setThreadNum(std::thread::hardware_concurrency());
_server->run();
}

HttpServer::~HttpServer()
{
_server->stop();
}
