#pragma once
#include "IRequests.h"
#include "RequestSignUp.h"
#include <cpprest/json.h>
#include "../stringtowstring.h"
using namespace web;

RequestSignUp::RequestSignUp(IDatabase* d,AnswerContainerInterface* answercontainer, const ISXModel::User& user) : IRequests(db, answercontainer), user(user) {}

void RequestSignUp::DoRequest() {
    json::value result;
    try {
        db->SaveUserToDB(this->user);
        ISXModel::User currentUser = db->GetUserFromDB(this->user.get_login());
        result[L"Login"] = json::value::string(to_wstring(currentUser.get_login()));
        result[L"id"] = json::value::Number(currentUser.get_id());
        this->answercontainer->SetStatusCode(status_codes::Accepted);
    }
    catch (std::exception e) {
        result[L"what"] = json::value::string(to_wstring(e.what()));
        this->answercontainer->SetStatusCode(status_codes::BadRequest);
    }
    this->answercontainer->SetAnswer(result);
    this->answercontainer->MakeDone();
}

