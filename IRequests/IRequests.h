//
// Created by pavlo on 9/2/21.
//

#pragma once
#include <cpprest/json.h>
#include "../MSSQLDatabase.h"
#include "../AnswerContainerInterface.h"
using namespace web;
extern class AnswerContainerInterface;

class IRequests {

protected:
    MSSQLDatabase* db;
    AnswerContainerInterface* answercontainer;
public:
    IRequests(MSSQLDatabase* db,AnswerContainerInterface* answercontainer);
    virtual void DoRequest() {
        
    }
};




