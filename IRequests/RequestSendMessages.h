#pragma once
#include <cpprest/json.h>
using namespace web;

class RequestSendMessages : public IRequests {
private:
    ISXModel::Message message;
public:
    RequestSendMessages(IDatabase* db, const ISXModel::Message& message);
    std::pair<json::value, int>  DoRequest();
};

