#pragma once
#include <cpprest/json.h>
#include "../MSSQLDatabase.h"
#include "../MSSQLDatabase.h"
#include "../model/User.h"
#include "../model/Message.h"
#include "../model/Chat.h"
using namespace web;

class RequestLogin : public IRequests {
private:
    std::string login, password;
public:
    RequestLogin(MSSQLDatabase* db, AnswerContainerInterface* answercontainer, const std::string& login, const std::string& password);
    void DoRequest();
};