#pragma once
#include <cpprest/json.h>
using namespace web;

class RequestJoinToTheChat : public IRequests {
private:
    std::string user_token, chat_title;
public:
    RequestJoinToTheChat(IDatabase* db, const std::string user_token, const std::string chat_title);
    void DoRequest();
};
