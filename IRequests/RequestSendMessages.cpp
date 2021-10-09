#pragma once
#include "IRequests.h"
#include "RequestSendMessages.h"
#include <cpprest/json.h>
#include "../stringtowstring.h"
using namespace web;

RequestSendMessages::RequestSendMessages(IDatabase* db, const ISXModel::Message& message) : IRequests(db), message(message)
{

}

std::pair<json::value, int>  RequestSendMessages::DoRequest() {
    json::value result;
    status_code code;
    try {
        db->SaveMessageToDB(this->message);
        result[L"status"] = json::value::string(L"OK");
        code = status_codes::Accepted;
    }
    catch (const QueryException& e) {
        result[L"what"] = json::value::string(to_wstring(e.what()));
        code = status_codes::BadRequest;
    }
    catch (const std::exception& e) {
        result[L"what"] = json::value::string(to_wstring("Database error"));
        code = status_codes::BadRequest;
    }
    return {result,0};
}


