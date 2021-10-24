#pragma once
#include "IRequests.h"
#include "RequestGetUserChats.h"
#include <cpprest/json.h>
#include "../stringtowstring.h"
using namespace web;

RequestGetUserChats::RequestGetUserChats(IDatabase* db, const std::string& userAccessToken) : IRequests(db),user_access_token(userAccessToken) {}

void RequestGetUserChats::DoRequest() {
    json::value result;
    try{
        std::vector<ISXModel::Chat> chatList = db->GetUserChatsFromDB(this->user_access_token);
        result[L"size"] = (int)chatList.size();
		json::value chats = json::value::array();
        for (int i = 0; i < chatList.size(); i++) {
            json::value current = json::value();
            current[L"id"] = (int)chatList[i].get_id();
            current[L"title"] = json::value::string(to_wstring(chatList[i].get_title()));
            chats[i] = current;
        }
        result[L"chats"] = chats; 
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
}
