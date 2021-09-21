#pragma once
#include "IRequests.h"
#include <cpprest/json.h>
using namespace web;

class RequestGetChats : public IRequests {
private:
    std::string user_login;
public:
    RequestGetChats(IDatabase* db, AnswerContainerInterface* answercontainer, const std::string& userLogin);
    void DoRequest();
};
