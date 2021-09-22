#pragma once
#include <cpprest/json.h>
using namespace web;

class RequestGetMessages : public IRequests {
private:
    std::string chat_title;
public:
    RequestGetMessages(IDatabase* db, const std::string& chatTitle);

    void DoRequest();
};
