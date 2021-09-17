#pragma once
#include <cpprest/json.h>
#include "../MSSQLDatabase.h"
#include "../MSSQLDatabase.h"
#include "../model/User.h"
#include "../model/Message.h"
#include "../model/Chat.h"
using namespace web;

class RequestSendMessages : public IRequests {
private:
    ISXModel::Message message;
public:
    RequestSendMessages(MSSQLDatabase* db, answercontainerinterface* answercontainer, const ISXModel::Message& message);
    json::value DoRequest();
};

