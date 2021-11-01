#pragma once
#include "IRequests.h"
#include "RequestLogin.h"
#include <cpprest/json.h>
#include "../StringUtils.h"
using namespace web;

RequestLogin::RequestLogin(IDatabase* db, const std::string& login, const std::string& password) : IRequests(db),
login(login),
password(password) {}


void RequestLogin::DoRequest() {
    json::value result;
    try {
        std::string user_access_token = db->GenerateUserAccessToken(this->login,this->password);
        result[L"token"] = json::value::string(to_wstring(user_access_token));
        result[L"login"] = json::value::string(to_wstring(this->login));
        this->answercontainer->SetStatusCode(status_codes::OK);
    }
    catch (const QueryException& e) {
        result[L"what"] = json::value::string(to_wstring("Invalid login or password"));
        this->answercontainer->SetStatusCode(status_codes::Unauthorized);
    }
    catch (const std::exception& e) {
        result[L"what"] = json::value::string(to_wstring("Database error"));
        this->answercontainer->SetStatusCode(status_codes::InternalError);
    }
    this->answercontainer->SetAnswer(result);
}
