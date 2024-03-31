#include "Routers.hpp"

#include <thread>

#include <map>


// Для хранения данных пользователей

std::map<int, nlohmann::json> users;


void route::RegisterResources(hv::HttpService &router)

{

router.POST("/user", [](HttpRequest *req, HttpResponse *resp)

{

nlohmann::json request;

nlohmann::json response;



try

{

request = nlohmann::json::parse(req->body);

int id = users.size(); // Просто присваиваем ID на основе размера map

users[id] = request; // Добавляем пользователя в map

response["msg"] = "User added successfully";

response["id"] = id;

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



router.GET("/user/{userId}",[](HttpRequest *req, HttpResponse *resp)

{

nlohmann::json response;

int userId = std::stoi(req->GetParam("userId"));



if (users.find(userId) != users.end())

{

response = users[userId];

}

else

{

response["error"] = "User not found";

resp->SetBody(response.dump());

resp->content_type = APPLICATION_JSON;

return 404;

}



resp->SetBody(response.dump());

resp->content_type = APPLICATION_JSON;



return 200;

});



router.GET("/users",[](HttpRequest *req, HttpResponse *resp)

{

nlohmann::json response = nlohmann::json::array();



for (auto &user : users)

{

nlohmann::json userJson;

userJson["id"] = user.first;

userJson["data"] = user.second;

response.push_back(userJson);

}



resp->SetBody(response.dump());

resp->content_type = APPLICATION_JSON;



return 200;

});



router.Delete("/user/{userId}", [](HttpRequest *req, HttpResponse *resp)

{

nlohmann::json response;

int userId = std::stoi(req->GetParam("userId"));



if (users.find(userId) != users.end())

{

users.erase(userId);

response["msg"] = "User deleted successfully";

}

else

{

response["error"] = "User not found";

resp->SetBody(response.dump());

resp->content_type = APPLICATION_JSON;

return 404;

}



resp->SetBody(response.dump());

resp->content_type = APPLICATION_JSON;



return 200;

});

} 
