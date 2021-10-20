#pragma once
#include "IRequests.h"
#include "RequestCreateNewChat.h"
#include <cpprest/json.h>
#include "../stringtowstring.h"
using namespace web;

RequestCreateNewChat::RequestCreateNewChat(IDatabase* db, const std::string& userAccessToken,const std::string& chatTitle) : IRequests(db),user_access_token(userAccessToken),chat_title(chatTitle) {}

void RequestCreateNewChat::DoRequest() {
    json::value result;
    try{
        ISXModel::Chat new_chat(chat_title);
        unsigned long new_chat_id = this->db->SaveChatToDB(this->user_access_token, new_chat);
        new_chat = this->db->GetChatFromDB(this->user_access_token, new_chat_id);
        result[L"id"] = (int)new_chat.get_id();
        result[L"title"] = json::value::string(to_wstring(new_chat.get_title()));
        this->answercontainer->SetStatusCode(status_codes::Accepted);
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
    this->answercontainer->MakeDone();
}
