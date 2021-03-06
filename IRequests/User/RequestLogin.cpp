#pragma once
#include "RequestLogin.h"

RequestLogin::RequestLogin(IDatabase* db, const std::string& login, const std::string& password) : IRequests(db),
login(login),
password(password) {}

void RequestLogin::DoRequest() {
    json::value result;
    try {
        std::string user_access_token = db->GetUserAccessToken(this->login,this->password);
        ISXModel::User user_data = db->GetUserFromDB(user_access_token);

        result[L"token"] = json::value::string(to_wstring(user_access_token));
        result[L"login"] = json::value::string(to_wstring(this->login));
        result[L"image"] = json::value::string(to_wstring(user_data.get_image()));

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
