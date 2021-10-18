#pragma once
#include "IRequests.h"
#include <cpprest/json.h>
#include "../stringtowstring.h"

using namespace web;

class RequestChangeLogin : public IRequests {
private:
    std::string new_login;
    unsigned long id;
    std::string access_token;
public:
    RequestChangeLogin(IDatabase* db, const std::string& accessToken,const unsigned long& id, const std::string& newLogin);
    void DoRequest();
};
