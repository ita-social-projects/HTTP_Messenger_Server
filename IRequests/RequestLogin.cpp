#pragma once
#include "IRequests.h"
#include "RequestLogin.h"
#include <cpprest/json.h>
#include "../stringtowstring.h"
using namespace web;

RequestLogin::RequestLogin(IDatabase* db, const std::string login, const std::string password) : IRequests(db),
login(login),
password(password) {}

std::pair<json::value, int>  RequestLogin::DoRequest() {
    json::value result;
    status_code code;
    try {
        ISXModel::User currentUser = db->GetUserFromDB(this->login);
        if (currentUser.get_password() == this->password) {
            result[L"id"] = (int)currentUser.get_id();
            result[L"Login"] = json::value::string(to_wstring(currentUser.get_login()));
            code = status_codes::OK;
        }
        else {
            result[L"what"] = json::value::string(to_wstring("No such user"));
            code = status_codes::Unauthorized;
        }
    }
    catch (const QueryException& e) {
        result[L"what"] = json::value::string(to_wstring("No such user"));
        code = status_codes::Unauthorized;
    }
    catch (const std::exception& e) {
        result[L"what"] = json::value::string(to_wstring("Database error"));
        code = status_codes::InternalError;
    }
    return { result,0 };    
}
