#pragma once 
#include "IRequests.h"
#include "RequestGetMessages.h"
#include <cpprest/json.h>
#include "../stringtowstring.h"
using namespace web;
RequestGetMessages::RequestGetMessages(IDatabase* db, const std::string& chatTitle) : IRequests(db), chat_title(chatTitle) {}

std::pair<json::value, int>  RequestGetMessages::DoRequest() {
    json::value result;
    status_code code;
    try {
        std::vector<ISXModel::Message> messageList = db->GetChatMessagesFromDB(this->chat_title);
        result[L"size"] = messageList.size();
        json::value messages;
        for (int i = 0; i < messageList.size(); i++) {
            json::value current = json::value();
            current[L"message_id"] = (int)messageList[i].get_id();
            current[L"sender_id"] = (int)messageList[i].get_sender_id();
            current[L"chat_id"] = (int)messageList[i].get_chat_id();
            current[L"text"] = json::value::string(to_wstring(messageList[i].get_content()));
            messages[i] = current;
        }
        result[L"messages"] = messages;
        code = status_codes::Accepted;
    }
    catch (const QueryException& e) {
        result[L"what"] = json::value::string(to_wstring(e.what()));
        code = status_codes::Accepted;
    }
    catch (const std::exception& e) {
        result[L"what"] = json::value::string(to_wstring("Database error"));
        code = status_codes::InternalError;
    }
    return { result,0 };
}
