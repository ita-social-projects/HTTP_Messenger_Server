//
//  HandlerRequest.hpp
//  main.cpp
//
//  Created by Павло Коваль on 31.08.2021.
//

#ifndef HandlerRequest_hpp
#define HandlerRequest_hpp

#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include <map>

#include "ThreadWorker.h"
#include "./IRequests/IRequests.h"
#include "./IRequests/RequestGetChats.h"
#include "./IRequests/RequestGetMessages.h"
#include "./IRequests/RequestLogin.h"
#include "./IRequests/RequestSignUp.h"
#include "./IRequests/RequestSendMessages.h"

#include "./Database/IDatabase.h"
using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace web::http::experimental::listener;
using namespace concurrency::streams;       // Asynchronous streams

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
    IDatabase db;
    ThreadWorker  worker;
    
    void _handle_get (http_request request);
    void _handle_post(http_request request);
    void _handle_put  (http_request request);
    void _handle_del  (http_request request);
    
public:
    
    void AddQueueThread(IDatabase &db);
    int  WhaitForRequest();
};


#endif /* HandlerRequest_hpp */
