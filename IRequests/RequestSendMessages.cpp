#pragma once
#include "IRequests.h"
#include "RequestSendMessages.h"
#include <cpprest/json.h>
#include "../stringtowstring.h"
using namespace web;

RequestSendMessages::RequestSendMessages(MSSQLDatabase* db, const ISXModel::Message& message) : IRequests(db)
{

}

json::value RequestSendMessages::DoRequest() {
    json::value result;
    db->SaveMessageToDB(this->message);
    result[L"status"] = json::value::string(L"OK");
    return result;
}


