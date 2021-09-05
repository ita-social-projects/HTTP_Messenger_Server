//
// Created by pavlo on 9/2/21.
//

#ifndef BACKEND_IREQUESTS_H
#define BACKEND_IREQUESTS_H
#pragma once
#include <cpprest/json.h>
#include <MSSQLDatabase.h>
#include "MSSQLDatabase.h"
#include <model/User.h>
#include <model/Message.h>
#include <model/Chat.h>
using namespace web;

class IRequests {
    MSSQLDatabase* db;
public:
    IRequests(MSSQLDatabase *db);
    void DoStuff();
};

class RequestGetChats: public IRequests {
private:
    std::string user_login;
public:
    RequestGetChats(MSSQLDatabase *db, const std::string &userLogin);

    void DoStuff();
};

class RequestGetMessages: public IRequests {
private:
    std::string chat_title;
public:
    RequestGetMessages(MSSQLDatabase *db, const std::string &chatTitle);

    void DoStuff();
};

class RequestSendMessages: public IRequests {
private:
    ISXModel::Message message;
public:
    RequestSendMessages(MSSQLDatabase *db, const ISXModel::Message &message);
    void DoStuff();
};

class RequestSignUp: public IRequests {
private:
    ISXModel::User user;
public:
    RequestSignUp(MSSQLDatabase *db, const ISXModel::User &user);
    void DoStuff();
};

class RequestLogin: public IRequests {
private:
    std::string login,password;
public:
    RequestLogin(MSSQLDatabase *db, const std::string &login, const std::string &password);
    void DoStuff();
};


#endif //BACKEND_IREQUESTS_H
