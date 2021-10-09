#pragma once
#include "IRequests.h"
#include <cpprest/json.h>
using namespace web;

class RequestGetChats : public IRequests {
private:
    std::string user_login;
public:
    RequestGetChats(IDatabase* db, const std::string& userLogin);
    std::pair<json::value, int>  DoRequest();
};
