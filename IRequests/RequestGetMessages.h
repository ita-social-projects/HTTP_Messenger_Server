#pragma once
#include <cpprest/json.h>
#include "../MSSQLDatabase.h"
#include "../MSSQLDatabase.h"
#include "../model/User.h"
#include "../model/Message.h"
#include "../model/Chat.h"
using namespace web;

class RequestGetMessages : public IRequests {
private:
    std::string chat_title;
public:
    RequestGetMessages(MSSQLDatabase* db, AnswerContainerInterface* answercontainer, const std::string& chatTitle);

    void DoRequest();
};
