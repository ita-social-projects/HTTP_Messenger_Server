#pragma once
#include "IRequests.h"
#include <cpprest/json.h>
using namespace web;

class RequestGetChats : public IRequests {
private:
    std::string user_login;
public:
    RequestGetChats(MSSQLDatabase* db, answercontainerinterface* answercontainer, const std::string& userLogin);
    json::value DoRequest();
};
