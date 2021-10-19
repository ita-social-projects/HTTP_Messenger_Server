#pragma once
#include <cpprest/json.h>
using namespace web;

class RequestLeaveChat : public IRequests {
private:
    std::string user_access_token, user_login;
    unsigned long chat_id;
public:
    RequestLeaveChat(IDatabase* db, const std::string& userAccessToken,const unsigned long& chatId,const std::string& userLogin);
    void DoRequest();
};
