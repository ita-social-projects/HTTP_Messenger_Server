#pragma once
#include "IRequests.h"
#include <cpprest/json.h>
using namespace web;

class RequestGetChats : public IRequests {
private:
    std::string user_access_token;
public:
    RequestGetChats(IDatabase* db, const std::string& userAccessToken);
    void DoRequest();
};
