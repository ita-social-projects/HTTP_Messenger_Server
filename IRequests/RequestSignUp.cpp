#pragma once
#include "IRequests.h"
#include "RequestSignUp.h"
#include <cpprest/json.h>
#include "../StringUtils.h"
using namespace web;

RequestSignUp::RequestSignUp(IDatabase* db, const ISXModel::User& user) : IRequests(db), user(user) {}

void RequestSignUp::DoRequest() {
    json::value result;
    try {
        if (db->SaveUserToDB(this->user)) {
            result[L"status"] = json::value::string(to_wstring("OK"));
            this->answercontainer->SetStatusCode(status_codes::OK);

        }
        else {
            result[L"what"] = json::value::string(to_wstring("Cannot process request"));
            this->answercontainer->SetStatusCode(status_codes::BadRequest);
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
}

