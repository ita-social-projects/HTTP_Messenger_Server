#pragma once
#include "RequestUpdateUserImage.h"

RequestUpdateUserImage::RequestUpdateUserImage(IDatabase* db, const std::string& userAccessToken, const std::string& image) : IRequests(db),
user_access_token(userAccessToken), image(image) {}

void RequestUpdateUserImage::DoRequest() {
    json::value result;
    try {
        if (this->db->UpdateUserImageInDB(user_access_token, image)) {
            result[L"status"] = json::value::string(L"OK");
            this->answercontainer->SetStatusCode(status_codes::OK);
        }
        else {
            result[L"what"] = json::value::string(to_wstring("Cannot update an immage"));
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
