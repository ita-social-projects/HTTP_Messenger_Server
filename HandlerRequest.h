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

#include "stringtowstring.h"
#include "ThreadWorker.h"
#include "IRequests/IRequests.h"
#include "IRequests/RequestGetChats.h"
#include "IRequests/RequestGetMessages.h"
#include "IRequests/RequestLogin.h"
#include "IRequests/RequestSendMessages.h"
#include "IRequests/RequestSignUp.h"
#include "Database/MSSQLDatabase.h"
#include "Database/MSSQLDatabase.h"

using namespace utility;
using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace web::http::experimental::listener;
using namespace concurrency::streams;

class HandlerRequest
{
private:
    MSSQLDatabase db;
    ThreadWorker  worker;
    
    void _handle_get  (http_request request);
    void _handle_post (http_request request);
    void _handle_put  (http_request request);
    void _handle_del  (http_request request);
    
public:
    
    void AddQueueThread(bool&);
};


#endif /* HandlerRequest_hpp */
