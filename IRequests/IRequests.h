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
#include "../AnswerContainerInterface.h"
using namespace web;
extern class answercontainerinterface;

class IRequests {

protected:
    MSSQLDatabase* db;
    answercontainerinterface* answercontainer;
public:
    IRequests(MSSQLDatabase* db,answercontainerinterface* answercontainer);
    virtual json::value DoRequest() {
        return json::value();
    }
};




