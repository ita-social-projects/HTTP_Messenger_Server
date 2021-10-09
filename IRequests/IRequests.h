//
// Created by pavlo on 9/2/21.
//

#pragma once
#include <cpprest/json.h>
#include "../Database/IDatabase.h"
#include "../AnswerContainerInterface.h"
using namespace web;
extern class AnswerContainerInterface;

class IRequests {

protected:
    IDatabase* db;
    
public:
    IRequests(IDatabase* db);
    ~IRequests();
    virtual std::pair<json::value,int> DoRequest() = 0;

};




