#pragma once
#include <cpprest/json.h>
#include "../Database/IDatabase.h"
#include "../AnswerContainerInterface.h"
#include "../StringUtils.h"

using namespace web;
extern class AnswerContainerInterface;

class IRequests {

protected:
    IDatabase* db;
    
public:
    AnswerContainerInterface* answercontainer;
    IRequests(IDatabase* db);
    virtual void DoRequest() {
        
    }
    void setAnswerContainer(AnswerContainerInterface* answercontainer) {
        this->answercontainer = answercontainer;
    }
};




