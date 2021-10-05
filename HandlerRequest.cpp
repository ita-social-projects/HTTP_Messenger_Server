//
//  HandlerRequest.cpp
//  main.cpp
//
//  Created by Павло Коваль on 31.08.2021.
//


#include "HandlerRequest.h"

void HandlerRequest::_handle_get(http_request request) {
    
    if (request.relative_uri().to_string() == L"/get_message")
    {
        const std::string FUTURE_CHAT_TITLE = "none";
        
        RequestGetMessages temp(&db, FUTURE_CHAT_TITLE);
        AnswerContainer t1(request, &temp);
        worker.PushRequest(&t1);
        
    } else if (request.relative_uri().to_string() == L"/get_chats")
    {
        const std::string FUTURE_USER_JSON = "none";
        
        RequestGetChats temp(&db, FUTURE_USER_JSON);
        AnswerContainer t1(request, &temp);
        worker.PushRequest(&t1);

    }
    
}

void HandlerRequest::_handle_post(http_request request) {
    if (request.relative_uri().to_string() == L"/user/login")
    {
        json::value *value = new json::value ( request.extract_json().wait() );


        RequestLogin *temp = new RequestLogin( &db, "admin54t", "admin");


        AnswerContainer *t1 = new AnswerContainer(request, temp);
        temp->setAnswerContainer(t1);
       
        worker.PushRequest(t1);

    } else if (request.relative_uri().to_string() == L"/user/register")
    {
        const std::string FUTURE_USER_JSON = "none";
        const std::string FUTURE_PASS_USER_JSON = "none";
        
        RequestLogin temp(&db, FUTURE_USER_JSON, FUTURE_PASS_USER_JSON);
        AnswerContainer t1(request, &temp);
        worker.PushRequest(&t1);
    }
    
    std::cout << "Handling post!\n";
}

void HandlerRequest::_handle_put(http_request request) {
    if (request.relative_uri().to_string() == L"/user/send_message")
    {
        const std::string FUTURE_USER_MESSAGE = "none";
        const unsigned long FUTURE_SENDER       = 0;
        const unsigned long FUTURE_CHAT_ID      = 0;
     
        RequestSendMessages temp(&db, ISXModel::Message(FUTURE_USER_MESSAGE, FUTURE_SENDER, FUTURE_CHAT_ID));
        AnswerContainer t1(request, &temp);
        worker.PushRequest(&t1);
    }
}

void HandlerRequest::_handle_del(http_request request) {
    std::cout << "Handling delete!\n";
}

void HandlerRequest::AddQueueThread()
{
        http_listener listener(L"http://localhost:8080/restdemo");
    
        listener.support(methods::GET,  std::bind(&HandlerRequest::_handle_get, this, std::placeholders::_1));
        listener.support(methods::POST, std::bind(&HandlerRequest::_handle_post, this, std::placeholders::_1));
        listener.support(methods::PUT,  std::bind(&HandlerRequest::_handle_put, this, std::placeholders::_1));
        listener.support(methods::DEL,  std::bind(&HandlerRequest::_handle_del, this, std::placeholders::_1));
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
    return 0;
}
