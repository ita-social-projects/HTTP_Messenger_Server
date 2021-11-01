#pragma once
#include "RequestChangeChatName.h"

RequestChangeChatName::RequestChangeChatName(IDatabase* db, const std::string& userAccessToken, const unsigned long& chat_id, const std::wstring& new_title) : IRequests(db),
chat_id(chat_id),
new_title(new_title),
user_access_token(userAccessToken) {}

void RequestChangeChatName::DoRequest() {
    json::value result;
    try {
        if (this->db->UpdateChatTitleInDB(this->user_access_token, this->chat_id, this->new_title)) {
            result[L"status"] = json::value::string(L"OK");
            this->answercontainer->SetStatusCode(status_codes::OK);
        }
        else {
            result[L"what"] = json::value::string(to_wstring("Cannot delete user"));
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
