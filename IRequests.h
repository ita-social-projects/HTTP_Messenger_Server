//
// Created by pavlo on 9/2/21.
//

#ifndef BACKEND_IREQUESTS_H
#define BACKEND_IREQUESTS_H
#pragma once
#include <cpprest/json.h>
#include "MSSQLDatabase.h"
#include "MSSQLDatabase.h"
#include "model/User.h"
#include "model/Message.h"
#include "model/Chat.h"
using namespace web;

class IRequests {

protected:
    MSSQLDatabase* db;
public:
    IRequests(MSSQLDatabase* db);
    virtual json::value DoRequest() {
        return json::value();
    }
};

class RequestGetChats : public IRequests {
private:
    std::string user_login;
public:
    RequestGetChats(MSSQLDatabase* db, const std::string& userLogin);

    json::value DoRequest();
};

class RequestGetMessages : public IRequests {
private:
    std::string chat_title;
public:
    RequestGetMessages(MSSQLDatabase* db, const std::string& chatTitle);

    json::value DoRequest();
};

class RequestSendMessages : public IRequests {
private:
    ISXModel::Message message;
public:
    RequestSendMessages(MSSQLDatabase* db, const ISXModel::Message& message);
    json::value DoRequest();
};

class RequestSignUp : public IRequests {
private:
    ISXModel::User user;
public:
    RequestSignUp(MSSQLDatabase* db, const ISXModel::User& user);
    json::value DoRequest();
};

class RequestLogin : public IRequests {
private:
    std::string login, password;
public:
    RequestLogin(MSSQLDatabase* db, const std::string& login, const std::string& password);
    json::value DoRequest();
};


#endif //BACKEND_IREQUESTS_H