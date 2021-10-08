#pragma once
#include "IRequests.h"
#include "RequestLogin.h"
#include <cpprest/json.h>
#include "../stringtowstring.h"
using namespace web;

RequestLogin::RequestLogin(IDatabase* db, const std::string login, const std::string password) : IRequests(db),
login(login),
password(password) {}

//Split into files
void RequestLogin::DoRequest() {
    json::value result;
    try {
        ISXModel::User currentUser = db->GetUserFromDB(this->login);
        if (currentUser.get_password() == this->password) {
            result[L"id"] = (int)currentUser.get_id();
            result[L"Login"] = json::value::string(to_wstring(currentUser.get_login()));
            this->answercontainer->SetStatusCode(status_codes::OK);
        }
        else {
            result[L"what"] = json::value::string(to_wstring("No such user"));
            this->answercontainer->SetStatusCode(status_codes::Unauthorized);
        }
    }
    catch (const QueryException& e) {
        result[L"what"] = json::value::string(to_wstring("No such user"));
        this->answercontainer->SetStatusCode(status_codes::Unauthorized);
    }
    catch (const std::exception& e) {
        result[L"what"] = json::value::string(to_wstring("Database error"));
        this->answercontainer->SetStatusCode(status_codes::InternalError);
    }


    this->answercontainer->SetAnswer(result);
    this->answercontainer->MakeDone();



    //Hashing
   
    
}
