#pragma once
#include "IRequests.h"
#include "RequestSendMessages.h"
#include <cpprest/json.h>
#include "../stringtowstring.h"
using namespace web;

RequestSendMessages::RequestSendMessages(MSSQLDatabase* db,answercontainerinterface* answercontainer, const ISXModel::Message& message) : IRequests(db, answercontainer), message(message)
{

}

json::value RequestSendMessages::DoRequest() {
    json::value result;
    db->SaveMessageToDB(this->message);
    result[L"status"] = json::value::string(L"OK");
    return result;
}


