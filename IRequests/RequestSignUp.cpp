#pragma once
#include "IRequests.h"
#include "RequestSignUp.h"
#include <cpprest/json.h>
#include "../stringtowstring.h"
using namespace web;

RequestSignUp::RequestSignUp(IDatabase* db, const ISXModel::User& user) : IRequests(db), user(user) {}

std::pair<json::value, int> RequestSignUp::DoRequest() {
    json::value result;
    status_code code;
    try {
        db->SaveUserToDB(this->user);
        ISXModel::User currentUser = db->GetUserFromDB(this->user.get_login());
        result[L"Login"] = json::value::string(to_wstring(currentUser.get_login()));
        result[L"id"] = (int)currentUser.get_id();
        code = status_codes::Accepted;
    }
    catch (const QueryException& e) {
        result[L"what"] = json::value::string(to_wstring(e.what()));
        code = status_codes::BadRequest;
    }
    catch (const std::exception& e) {
        result[L"what"] = json::value::string(to_wstring("Database error"));
        code = status_codes::InternalError;
    }
    return { result,0 };
}

