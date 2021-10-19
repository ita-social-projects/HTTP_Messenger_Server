#pragma once
#include "IRequests.h"
#include "RequestGetUserChats.h"
#include <cpprest/json.h>
#include "../stringtowstring.h"
using namespace web;

RequestCreateNewChat::RequestCreateNewChat(IDatabase* db, const std::string& userAccessToken, std::string& chatTitle) : IRequests(db),user_access_token(userAccessToken),chat_title(chatTitle) {}

void RequestCreateNewChat::DoRequest() {
    json::value result;
    try{

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
