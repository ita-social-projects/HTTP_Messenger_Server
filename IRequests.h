//
// Created by pavlo on 9/2/21.
//

#ifndef BACKEND_IREQUESTS_H
#define BACKEND_IREQUESTS_H
#pragma once
#include <cpprest/json.h>

using namespace web;

class IRequests {
public:
    void DoStuff();
};

class RequestGetChats: public IRequests {
public:
    void DoStuff();
};

class RequestGetMessages: public IRequests {
public:
    void DoStuff();
};

class RequestSendMessages: public IRequests {
public:
    void DoStuff();
};

class RequestSignUp: public IRequests {
public:
    void DoStuff();
};

class RequestLogin: public IRequests {
public:
    void DoStuff();
};


#endif //BACKEND_IREQUESTS_H
