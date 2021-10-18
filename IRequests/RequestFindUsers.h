#pragma once
#include "IRequests.h"
#include <cpprest/json.h>
using namespace web;

class RequestFindUsers : public IRequests {
private:
    std::string user_login_part;
    std::string user_token;
public:
    RequestFindUsers(IDatabase* db, const std::string& userToken, const std::string& userLoginPart);
    void DoRequest();
};
