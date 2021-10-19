#pragma once
#include "IRequests.h"
#include "RequestSendMessages.h"
#include <cpprest/json.h>
#include "../stringtowstring.h"
using namespace web;

RequestSendMessages::RequestSendMessages(IDatabase* db, const std::string& userAccessToken, const ISXModel::Message& message) : IRequests(db), user_access_token(userAccessToken), message(message)
{

}

void RequestSendMessages::DoRequest() {
    json::value result;
    try {
        if(db->SaveMessageToDB(this->user_access_token,this->message)){
            result[L"status"] = json::value::string(L"OK");
            this->answercontainer->SetStatusCode(status_codes::Accepted);
        }
        else {
            result[L"what"] = json::value::string(L"Cannot send message");
            this->answercontainer->SetStatusCode(status_codes::Forbidden);
        }
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


