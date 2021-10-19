#pragma once 
#include "IRequests.h"
#include "RequestGetMessages.h"
#include <cpprest/json.h>
#include "../stringtowstring.h"
using namespace web;
RequestGetMessages::RequestGetMessages(IDatabase* db, const std::string& userAccessToken,const unsigned long& chatId) : IRequests(db), user_access_token(userAccessToken), chat_id(chatId) {}

void RequestGetMessages::DoRequest() {
    json::value result;
    try {
        std::vector<ISXModel::Message> messageList = db->GetChatMessagesFromDB(this->user_access_token,this->chat_id);
        result[L"size"] = (int)messageList.size();
        json::value messages;
        for (int i = 0; i < messageList.size(); i++) {
            json::value current = json::value();
            current[L"message_id"] = (int)messageList[i].get_id();
            current[L"sender"] = json::value::string(to_wstring(messageList[i].get_sender()));
            current[L"chat_id"] = (int)messageList[i].get_chat_id();
            current[L"timestamp"] = json::value::string(to_wstring(messageList[i].get_timestamp()));
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
