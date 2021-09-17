#pragma once
#include <cpprest/json.h>
#include "../MSSQLDatabase.h"
#include "../MSSQLDatabase.h"
#include "../model/User.h"
#include "../model/Message.h"
#include "../model/Chat.h"
using namespace web;

class RequestSignUp : public IRequests {
private:
    ISXModel::User user;
public:
    RequestSignUp(MSSQLDatabase* db, const ISXModel::User& user);
    json::value DoRequest();
};

