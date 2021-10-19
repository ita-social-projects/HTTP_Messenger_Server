#pragma once
#include <cpprest/json.h>
using namespace web;

class RequestGetMessages : public IRequests {
private:
    std::string user_access_token;
    unsigned long chat_id;
public:
    RequestGetMessages(IDatabase* db, const std::string& userAccessToken, const unsigned long& chatId);

    void DoRequest();
};
