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
        json::value value = request.extract_json().get();

        const utility::string_t chat_title = value[L"chat"].to_string();

        const std::string CHAT_TITLE = to_string(chat_title.substr(1, chat_title.length() - 2));
        
        RequestGetMessages* temp = new  RequestGetMessages(&db, CHAT_TITLE);
        AnswerContainer* t1 = new AnswerContainer(request, temp);

        worker.PushRequest(t1);
    }
    else if (request.relative_uri().to_string() == L"/get_chats")
    {
        json::value value = request.extract_json().get();

        const utility::string_t login = value[L"login"].to_string();

        const std::string USER_LOGIN = to_string( login.substr(1, login.length() - 2) );
        
        RequestGetChats* temp = new  RequestGetChats(&db, USER_LOGIN);
        AnswerContainer* t1 = new AnswerContainer(request, temp);

        worker.PushRequest(t1);
    }
    
}

void HandlerRequest::_handle_post(http_request request) {
    if (request.relative_uri().to_string() == L"/user/login")
    {
        json::value value = request.extract_json().get();

        const utility::string_t login = value[L"login"].to_string();
        const utility::string_t pass  = value[L"pass"].to_string();

        const std::string LOGIN_USER     = to_string(login.substr(1, login.length() - 2));
        const std::string PASS_USER      = to_string(pass.substr(1, pass.length() - 2));

        RequestLogin *temp = new RequestLogin( &db, LOGIN_USER, PASS_USER);
        AnswerContainer *t1 = new AnswerContainer(request, temp);
       
        worker.PushRequest(t1);
    }
    else if (request.relative_uri().to_string() == L"/user/sign_up")
    {
        json::value value = request.extract_json().get();

        const utility::string_t login = value[L"login"].to_string();
        const utility::string_t pass  = value[L"pass"].to_string();

        const std::string LOGIN_USER  = to_string(login.substr(1, login.length() - 2));
        const std::string PASS_USER   = to_string(pass.substr(1, pass.length() - 2));
        
        RequestSignUp*   temp = new RequestSignUp( &db, ISXModel::User(LOGIN_USER, PASS_USER) );
        AnswerContainer* t1 =   new AnswerContainer(request, temp);

        worker.PushRequest(t1);
    }
    
    std::cout << "Handling post!\n";
}

void HandlerRequest::_handle_put(http_request request) {
    if (request.relative_uri().to_string() == L"/user/send_message")
    {
        json::value value = request.extract_json().get();

        const utility::string_t msg = value[L"login"].to_string();

        const std::string   USER_MESSAGE    = to_string(msg.substr(1, msg.length() - 2));
        const unsigned long SENDER_ID       = value[L"sender"].as_integer();
        const unsigned long CHAT_ID         = value[L"chat"].as_integer();
     
        RequestSendMessages* temp = new RequestSendMessages(&db, ISXModel::Message(USER_MESSAGE, SENDER_ID, CHAT_ID));
        AnswerContainer* t1 = new AnswerContainer(request, temp);

        worker.PushRequest(t1);
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