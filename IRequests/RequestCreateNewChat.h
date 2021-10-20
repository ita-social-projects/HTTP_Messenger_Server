#pragma once
#include "IRequests.h"
#include <cpprest/json.h>
using namespace web;

class RequestCreateNewChat : public IRequests {
private:
    std::string user_access_token, chat_title;
public:
    RequestCreateNewChat(IDatabase* db, const std::string& userAccessToken, const std::string& chatTitle);
    void DoRequest();
};
