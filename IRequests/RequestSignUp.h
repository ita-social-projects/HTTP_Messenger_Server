#pragma once
#include <cpprest/json.h>
using namespace web;

class RequestSignUp : public IRequests {
private:
    ISXModel::User user;
public:
    RequestSignUp(IDatabase* db, AnswerContainerInterface* answercontainer, const ISXModel::User& user);
    void DoRequest();
};

