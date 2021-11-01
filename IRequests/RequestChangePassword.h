#pragma once
#include "IRequests.h"
#include "../StringUtils.h"
#include <cpprest/json.h>
using namespace web;

class RequestChangePassword : public IRequests {
private:
    std::string new_password,old_password;
    std::string user_access_token;
public:
    RequestChangePassword(IDatabase* db, const std::string& userAccessToken,const std::string& oldPassword, const std::string& newPassword);
    void DoRequest();
};
