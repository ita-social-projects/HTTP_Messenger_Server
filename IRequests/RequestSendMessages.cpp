#pragma once
#include "IRequests.h"
#include "RequestSendMessages.h"
#include <cpprest/json.h>
#include "../stringtowstring.h"
using namespace web;

RequestSendMessages::RequestSendMessages(IDatabase* db, const ISXModel::Message& message) : IRequests(db), message(message)
{

}

void RequestSendMessages::DoRequest() {
    json::value result;
    try {
        db->SaveMessageToDB(this->message);
        result[L"status"] = json::value::string(L"OK");
        this->answercontainer->SetStatusCode(status_codes::Accepted);
    }
    catch (const QueryException& e) {
        result[L"what"] = json::value::string(to_wstring(e.what()));
        this->answercontainer->SetStatusCode(status_codes::BadRequest);
    }
    catch (const std::exception& e) {
        result[L"what"] = json::value::string(to_wstring("Database error"));
        this->answercontainer->SetStatusCode(status_codes::InternalError);
    }
    this->answercontainer->SetAnswer(result);
    this->answercontainer->MakeDone();

    
}


