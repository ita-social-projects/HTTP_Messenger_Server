#pragma once
#include "RequestCheckTimeSession.h"

RequestCheckTimeSession::RequestCheckTimeSession(IDatabase* db, const std::string& userAccessToken) : IRequests(db),
user_access_token(userAccessToken) {}

void RequestCheckTimeSession::DoRequest() {
    json::value result;
    try {
        if (this->db->UpdateUserAccessTokenUsedDateInDB(this->user_access_token)) {
            result[L"status"] = json::value::string(L"OK");
            this->answercontainer->SetStatusCode(status_codes::OK);
        }
        else {
            result[L"what"] = json::value::string(to_wstring("Your time is out"));
            this->answercontainer->SetStatusCode(status_codes::Forbidden);
        }
    }
    catch (const QueryException& e) {
        result[L"what"] = json::value::string(to_wstring(e.what()));
        this->answercontainer->SetStatusCode(status_codes::Unauthorized);
    }
    catch (const std::exception& e) {
        result[L"what"] = json::value::string(to_wstring("Database error"));
        this->answercontainer->SetStatusCode(status_codes::InternalError);
    }
    this->answercontainer->SetAnswer(result);
}
