#pragma once
#include "IRequests.h"
#include <cpprest/json.h>
using namespace web;

class RequestGetUserChats : public IRequests {
private:
    std::string user_access_token;
public:
    RequestGetUserChats(IDatabase* db, const std::string& userAccessToken);
    void DoRequest();
};
