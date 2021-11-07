#pragma once
#include "../IRequests.h"

class RequestChangeLogin : public IRequests {
private:
    std::string new_login;
    std::string user_access_token;
public:
    RequestChangeLogin(IDatabase* db, const std::string& userAccessToken, const std::string& newLogin);
    void DoRequest();
};
