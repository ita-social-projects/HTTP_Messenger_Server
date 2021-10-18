#pragma once
#include <cpprest/json.h>
using namespace web;

class RequestGetMessages : public IRequests {
private:
    std::string chat_title,user_token;
    unsigned long chat_id;
public:
    RequestGetMessages(IDatabase* db, const std::string& userToken, const unsigned long& chatId);

    void DoRequest();
};
