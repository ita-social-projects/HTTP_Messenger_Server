#pragma once
#include "IRequests.h"
#include "RequestSendMessages.h"
#include <cpprest/json.h>
#include "../stringtowstring.h"
using namespace web;

RequestSendMessages::RequestSendMessages(MSSQLDatabase* db,AnswerContainerInterface* answercontainer, const ISXModel::Message& message) : IRequests(db, answercontainer), message(message)
{

}

void RequestSendMessages::DoRequest() {
    json::value result;
    try {
        db->SaveMessageToDB(this->message);
        result[L"status"] = json::value::string(L"OK");
        this->answercontainer->SetStatusCode(status_codes::Accepted);
    }
    catch(std::exception e){
        result[L"what"] = json::value::string(to_wstring(e.what()));
        this->answercontainer->SetStatusCode(status_codes::BadRequest);
    }
    this->answercontainer->SetAnswer(result);
    this->answercontainer->MakeDone();

    
}


