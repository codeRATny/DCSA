#include "HTTPServer.hpp"

int main()
{
HttpServer::UPtr server = std::make_unique<HttpServer>();

server->Start(7778);

return 0;
} 
