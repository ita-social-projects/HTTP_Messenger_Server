#pragma once
#include <cpprest/json.h>
using namespace web;

class RequestLogin : public IRequests {
private:
    std::string login, password;
public:
    RequestLogin(IDatabase* db, const std::string login, const std::string password);
    void DoRequest();
};
