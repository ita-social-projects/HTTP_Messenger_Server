#pragma once
#include <cpprest/json.h>
#include "../MSSQLDatabase.h"
using namespace web;

class RequestGetMessages : public IRequests {
private:
    std::string chat_title;
public:
    RequestGetMessages(MSSQLDatabase* db, AnswerContainerInterface* answercontainer, const std::string& chatTitle);

    void DoRequest();
};
