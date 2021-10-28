#pragma once
#include "IRequests.h"
#include <cpprest/json.h>
#include "../StringUtils.h"

using namespace web;

class RequestChangeLogin : public IRequests {
private:
    std::string new_login;
    std::string user_access_token;
public:
    RequestChangeLogin(IDatabase* db, const std::string& userAccessToken, const std::string& newLogin);
    void DoRequest();
};
