#pragma once
#include "IRequests.h"
#include <cpprest/json.h>
using namespace web;

class RequestGetChatParticipants : public IRequests {
private:
    std::string chat_title;
public:
    RequestGetChatParticipants(IDatabase* db, const std::string& chatTitle);
    void DoRequest();
};
