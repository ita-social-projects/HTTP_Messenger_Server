#pragma once
#include "IRequests.h"
#include "RequestJoinToTheChat.h"
#include <cpprest/json.h>
#include "../stringtowstring.h"
using namespace web;

RequestJoinToTheChat::RequestJoinToTheChat(IDatabase* db, const std::string userLogin, const std::string chatTitle) : IRequests(db),
user_token(userLogin),
chat_title(chatTitle) {}

void RequestJoinToTheChat::DoRequest() {
    json::value result;
    try {
        if (this->db->AddUserToChat(this->user_token, this->chat_title)) {
            result[L"status"] = json::value::string(L"OK");
        }
        else {
            result[L"what"] = json::value::string(to_wstring("Cannot join to this chat"));
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
