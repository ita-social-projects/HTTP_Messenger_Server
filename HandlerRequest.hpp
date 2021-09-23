//
//  HandlerRequest.hpp
//  main.cpp
//
//  Created by Павло Коваль on 31.08.2021.
//

#ifndef HandlerRequest_hpp
#define HandlerRequest_hpp

#include "ThreadWorker.hpp"

enum requestStatus
{
    SIGN_UP      = 1,
    LOG_IN       = 2,
    GET_MESSAGE  = 3,
    SEND_MESSAGE = 4,
    GET_CHATS    = 5
};

class HandlerRequest
{
private:
    std::unique_ptr<IRequests> request;
    
    requestStatus  _GetRequestResponse();
    
public:
    
    void AddQueueThread();
    int  WhaitForRequest();
};


#endif /* HandlerRequest_hpp */
