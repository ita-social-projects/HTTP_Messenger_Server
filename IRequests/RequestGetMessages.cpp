#pragma once 
#include "IRequests.h"
#include "RequestGetMessages.h"
#include <cpprest/json.h>
#include "../stringtowstring.h"
using namespace web;
RequestGetMessages::RequestGetMessages(IDatabase* db, const std::string& chatTitle) : IRequests(db), chat_title(chatTitle) {}

void RequestGetMessages::DoRequest() {
    json::value result;
    try {
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
