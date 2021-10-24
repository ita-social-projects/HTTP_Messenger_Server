#pragma once
#include "IRequests.h"
#include "RequestSendMessages.h"
#include <cpprest/json.h>
#include "../stringtowstring.h"
using namespace web;

RequestSendMessages::RequestSendMessages(IDatabase* db, const std::string& userAccessToken, const ISXModel::Message& message) : 
    IRequests(db), user_access_token(userAccessToken), message(message)
{}

void RequestSendMessages::DoRequest() {
    json::value result;
    try {
        unsigned long new_message_id = this->db->SaveMessageToDB(this->user_access_token, this->message);
        ISXModel::Message new_message = this->db->GetMessageFromDB(this->user_access_token, new_message_id);
        result[L"id"] = (int)new_message.get_id();
        result[L"timestamp"] = json::value::string(to_wstring(new_message.get_timestamp()));
        result[L"content"] = json::value::string(to_wstring(new_message.get_content()));
        result[L"sender"] = json::value::string(to_wstring(new_message.get_sender()));
		this->answercontainer->SetStatusCode(status_codes::OK);
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


