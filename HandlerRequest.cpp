//
//  HandlerRequest.cpp
//  main.cpp
//
//  Created by Павло Коваль on 31.08.2021.
//


#include "HandlerRequest.h"

void HandlerRequest::_handle_get(http_request request) {
    
    if (request.relative_uri().to_string() == "/get_message")
    {
        const std::string FUTURE_CHAT_TITLE = "none";
        
        RequestGetMessages temp(&db, FUTURE_CHAT_TITLE);
        worker.PushRequest(temp);
        
    } else if (request.relative_uri().to_string() == "/get_chats")
    {
        const std::string FUTURE_USER_JSON = "none";
        
        RequestGetMessages temp(&db, FUTURE_USER_JSON);
        worker.PushRequest(temp);
    }
    
    request.reply(status_codes::OK, "OK");
}

void HandlerRequest::_handle_post(http_request request) {
    if (request.relative_uri().to_string() == "/login")
    {
        const std::string FUTURE_USER_JSON = "none";
        const std::string FUTURE_PASS_USER_JSON = "none";
        
        RequestLogin temp(&db, FUTURE_USER_JSON, FUTURE_PASS_USER_JSON);
        worker.PushRequest(temp);
        
    } else if (request.relative_uri().to_string() == "/user/register")
    {
        const std::string FUTURE_USER_JSON = "none";
        const std::string FUTURE_PASS_USER_JSON = "none";
        
        RequestSignUp temp(&db, ISXModel::User(FUTURE_USER_JSON, FUTURE_PASS_USER_JSON));
        worker.PushRequest(temp);
    }
    
    std::cout << "Handling put!\n";
}

void HandlerRequest::_handle_put(http_request request) {
    if (request.relative_uri().to_string() == "/user/send_message")
    {
        const std::string FUTURE_USER_MESSAGE = "none";
        const unsigned long FUTURE_SENDER       = 0;
        const unsigned long FUTURE_CHAT_ID      = 0;
        
        RequestSendMessages temp(&db, ISXModel::Message(FUTURE_USER_MESSAGE, FUTURE_SENDER, FUTURE_CHAT_ID));
        worker.PushRequest(temp);
    }
}

void HandlerRequest::_handle_del(http_request request) {
    std::cout << "Handling delete!\n";
}

void HandlerRequest::AddQueueThread(MSSQLDatabase &db)
{
        http_listener listener("http://localhost:8080/restdemo");
    
        listener.support(methods::GET,  _handle_get);
        listener.support(methods::POST, _handle_post);
        listener.support(methods::PUT,  _handle_put);
        listener.support(methods::DEL,  _handle_del);
        try
        {
            listener
                    .open()
                    .then([&listener](){std::cout<<"Starting..."<<std::endl;})
                    .wait();
            while (true);
        }
        catch (std::exception const & e)
        {
            std::cout << e.what() << std::endl;
        }
    
}

int  HandlerRequest::WhaitForRequest()
{
    
}
