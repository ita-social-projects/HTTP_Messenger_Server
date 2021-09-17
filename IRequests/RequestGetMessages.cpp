#pragma once 
#include "IRequests.h"
#include "RequestGetMessages.h"
#include <cpprest/json.h>
#include "../stringtowstring.h"
using namespace web;
RequestGetMessages::RequestGetMessages(MSSQLDatabase* db, answercontainerinterface* answercontainer, const std::string& chatTitle) : IRequests(db, answercontainer), chat_title(chatTitle) {}

json::value RequestGetMessages::DoRequest() {
    json::value result;
    std::vector<ISXModel::Message> messageList = db->GetChatMessagesFromDB(this->chat_title);
    result[L"size"] = messageList.size();
    json::value messages;
    for (int i = 0; i < messageList.size(); i++) {
        json::value current = json::value();
        current[L"message_id"] = json::value::Number(messageList[i].get_id());
        current[L"sender_id"] = json::value::Number(messageList[i].get_sender_id());
        current[L"chat_id"] = json::value::Number(messageList[i].get_chat_id());
        current[L"text"] = json::value::string(to_wstring(messageList[i].get_content()));
        messages[i] = current;
    }
    result[L"messages"] = messages;
    return result;
}
