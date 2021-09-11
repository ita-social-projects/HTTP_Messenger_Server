//
//  HandlerRequest.cpp
//  main.cpp
//
//  Created by Павло Коваль on 31.08.2021.
//

#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include "HandlerRequest.hpp"


using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace concurrency::streams;       // Asynchronous streams

requestStatus  HandlerRequest::_GetRequestResponse()
{
    std::string url_ = "http://www.20min.ch/rss/rss.tmpl?type=channel&get=68";
    http_client client1(utility::conversions::to_string_t(url_));
    http_request request;
    
    request.set_method(methods::GET);
    request.headers().add(U("Host"), U("www.20min.ch"));
    
    client1.request(request).then([] (http_response response)
    {
        return response.status_code();
    }).wait();
    
    // data = text msg, log
    // statusCode ;
    
}

void HandlerRequest::AddQueueThread()
{
    requestStatus statusCode = _GetRequestResponse();
    ThreadWorker  worker;
    
    switch (statusCode)
    {
        case SIGN_UP:
        {
            RequestSignUp temp;
            worker.PushRequest(temp);
        }
            break;
            
        case LOG_IN:
        {
            RequestLogin temp;
            worker.PushRequest(temp);
        }
            break;
            
        case GET_MESSAGE:
        {
            RequestGetMessages temp;
            worker.PushRequest(temp);
        }
            break;
            
        case SEND_MESSAGE:
        {
            RequestSignUp temp;
            worker.PushRequest(temp);
        }
            break;
            
        case GET_CHATS:
        {
            RequestSignUp temp;
            worker.PushRequest(temp);
        }
            break;
            
        default:
            break;
    }
    
    
}

int  HandlerRequest::WhaitForRequest()
{
    
}
