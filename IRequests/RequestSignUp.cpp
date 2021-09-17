#pragma once
#include "IRequests.h"
#include "RequestSignUp.h"
#include <cpprest/json.h>
#include "../stringtowstring.h"
using namespace web;

RequestSignUp::RequestSignUp(MSSQLDatabase* d,AnswerContainerInterface* answercontainer, const ISXModel::User& user) : IRequests(db, answercontainer), user(user) {}

json::value RequestSignUp::DoRequest() {
    json::value result;
    db->SaveUserToDB(this->user);
    ISXModel::User currentUser = db->GetUserFromDB(this->user.get_login());
    result[L"Login"] = json::value::string(to_wstring(currentUser.get_login()));
    result[L"id"] = json::value::Number(currentUser.get_id());
    return result;
}

