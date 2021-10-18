#pragma once
#include <cpprest/json.h>
using namespace web;

class RequestSendMessages : public IRequests {
private:
    ISXModel::Message message;
    std::string token;
public:
    RequestSendMessages(IDatabase* db, const std::string& token, const ISXModel::Message& message);
    void DoRequest();
};

