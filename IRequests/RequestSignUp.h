#pragma once
#include <cpprest/json.h>
#include "../MSSQLDatabase.h"
using namespace web;

class RequestSignUp : public IRequests {
private:
    ISXModel::User user;
public:
    RequestSignUp(MSSQLDatabase* db, AnswerContainerInterface* answercontainer, const ISXModel::User& user);
    void DoRequest();
};

