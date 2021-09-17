//
// Created by pavlo on 9/2/21.
//

#pragma once
#include <cpprest/json.h>
#include "../MSSQLDatabase.h"
#include "../MSSQLDatabase.h"
#include "../model/User.h"
#include "../model/Message.h"
#include "../model/Chat.h"
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




