#pragma once
#include "IRequests.h"
#include "RequestLogin.h"
#include <cpprest/json.h>
#include "../stringtowstring.h"
using namespace web;

RequestLogin::RequestLogin(MSSQLDatabase* db, answercontainerinterface* answercontainer, const std::string& login, const std::string& password) : IRequests(db, answercontainer),
login(login),
password(password) {}

//Split into files
json::value RequestLogin::DoRequest() {
    json::value result;
    //Hashing
    ISXModel::User currentUser = db->GetUserFromDB(this->login);
    if (currentUser.get_password() == this->password) {
        result[L"id"] = json::value::Number(currentUser.get_id());
        result[L"Login"] = json::value::string(to_wstring(currentUser.get_login()));
        //Return user and token(mb) 
    }
    else {

    }
    return result;
}
