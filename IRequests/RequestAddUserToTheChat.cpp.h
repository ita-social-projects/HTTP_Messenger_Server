#pragma once
#include <cpprest/json.h>
using namespace web;

class RequestJoinToTheChat : public IRequests {
private:
    std::string user_access_token, user_login;
    unsigned long chat_id;
public:
    RequestJoinToTheChat(IDatabase* db, const std::string& userAccessToken,const unsigned long& chatId,const std::string& userLogin);
    void DoRequest();
};
