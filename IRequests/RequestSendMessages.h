#pragma once
#include <cpprest/json.h>
#include "../MSSQLDatabase.h"
using namespace web;

class RequestSendMessages : public IRequests {
private:
    ISXModel::Message message;
public:
    RequestSendMessages(MSSQLDatabase* db, AnswerContainerInterface* answercontainer, const ISXModel::Message& message);
    void DoRequest();
};

