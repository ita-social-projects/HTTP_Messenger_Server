#pragma once
#include "RequestUpdateChatImage.h"

RequestUpdateChatImage::RequestUpdateChatImage(IDatabase* db, const std::string& userAccessToken, const unsigned long& chat_id, const std::string& image) : IRequests(db),
user_access_token(userAccessToken),chat_id(chat_id) ,image(image) {}

void RequestUpdateChatImage::DoRequest() {
    json::value result;
    try {
        if (this->db->UpdateChatImageInDB(user_access_token, chat_id, image)) {
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
