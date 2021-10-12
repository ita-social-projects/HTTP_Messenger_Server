#pragma once
#include <cpprest/json.h>
using namespace web;

class RequestLogin : public IRequests {
private:
    std::string user_login, chat_title;
public:
    RequestLogin(IDatabase* db, const std::string user_login, const std::string chat_title);
    void DoRequest();
};
