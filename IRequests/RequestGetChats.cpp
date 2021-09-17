#pragma once
#include "IRequests.h"
#include "RequestGetChats.h"
#include <cpprest/json.h>
#include "../stringtowstring.h"
using namespace web;

RequestGetChats::RequestGetChats(MSSQLDatabase* db, answercontainerinterface* answercontainer, const std::string& userLogin) : IRequests(db, answercontainer),
user_login(userLogin) {}

json::value RequestGetChats::DoRequest() {
    json::value result;
    std::vector<ISXModel::Chat> chatList = db->GetUserChatsFromDB(this->user_login);
    result[L"size"] = json::value::Number(chatList.size());
    json::value chats;
    for (int i = 0; i < chatList.size(); i++) {
        json::value current = json::value();
        current[L"id"] = json::value::Number(chatList[i].get_id());
        current[L"title"] = json::value::string(to_wstring(chatList[i].get_title()));
        chats[i] = current;
    }
    result[L"chats"] = chats;
    return result;
}
