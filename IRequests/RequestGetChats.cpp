#pragma once
#include "IRequests.h"
#include "RequestGetChats.h"
#include <cpprest/json.h>
#include "../stringtowstring.h"
using namespace web;

RequestGetChats::RequestGetChats(IDatabase* db, const std::string& userLogin) : IRequests(db),user_login(userLogin) {}

std::pair<json::value, int>  RequestGetChats::DoRequest() {
    json::value result;
    status_code code;
    try{
        std::vector<ISXModel::Chat> chatList = db->GetUserChatsFromDB(this->user_login);
        result[L"size"] = json::value::Number(chatList.size());
        json::value chats;
        for (int i = 0; i < chatList.size(); i++) {
            json::value current = json::value();
            current[L"id"] = (int)chatList[i].get_id();
            current[L"title"] = json::value::string(to_wstring(chatList[i].get_title()));
            chats[i] = current;
        }
        result[L"chats"] = chats; 
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
    return {result,0};
}
