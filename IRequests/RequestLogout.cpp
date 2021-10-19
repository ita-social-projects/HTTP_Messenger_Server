#pragma once
#include "IRequests.h"
#include "RequestLogout.h"
#include <cpprest/json.h>
#include "../stringtowstring.h"
using namespace web;

RequestLogout::RequestLogout(IDatabase* db, const std::string& userAccessToken) : IRequests(db),
user_access_token(userAccessToken){}

void RequestLogout::DoRequest() {
    json::value result;
    try {
        if (this->db->RemoveUserAccessToken(this->user_access_token)) {
            result[L"status"] = json::value::string(L"OK");
        }
        else {
            result[L"what"] = json::value::string(to_wstring("Cannot leave the app"));
            this->answercontainer->SetStatusCode(status_codes::Forbidden);
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
}
