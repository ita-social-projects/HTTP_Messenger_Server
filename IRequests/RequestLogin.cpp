#pragma once
#include "IRequests.h"
#include "RequestLogin.h"
#include <cpprest/json.h>
#include "../stringtowstring.h"
using namespace web;

RequestLogin::RequestLogin(MSSQLDatabase* db, AnswerContainerInterface* answercontainer, const std::string& login, const std::string& password) : IRequests(db, answercontainer),
login(login),
password(password) {}

//Split into files
void RequestLogin::DoRequest() {
    json::value result;
    try {
        ISXModel::User currentUser = db->GetUserFromDB(this->login);
        if (currentUser.get_password() == this->password) {
            result[L"id"] = json::value::Number(currentUser.get_id());
            result[L"Login"] = json::value::string(to_wstring(currentUser.get_login()));
            this->answercontainer->SetStatusCode(status_codes::Accepted);
        }
        else {
            this->answercontainer->SetStatusCode(status_codes::NotFound);
            result[L"what"] = json::value::string(to_wstring("User not found"));
        }
    }catch(std::exception e) {
        result[L"what"] = json::value::string(to_wstring(e.what()));
        this->answercontainer->SetStatusCode(status_codes::BadRequest);
    }
    this->answercontainer->SetAnswer(result);
    this->answercontainer->MakeDone();
    //Hashing
   
    
}
