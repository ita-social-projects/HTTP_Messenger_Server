#include "HandlerRequest.h"

void HandlerRequest::_pushRequest(const http_request& request, IRequests* irequest)
{
    AnswerContainer* answer = new AnswerContainer(request, irequest);

    irequest->setAnswerContainer(answer);

    worker.PushRequest(answer);
}

void HandlerRequest::_groupUser (const http_request& request, const std::string urlRequest)
{
    if (urlRequest == "/login")
    {
        _requestLogin(request);
    }
    else if (urlRequest == "/sign_up")
    {
        _requestSignUp(request);
    }
    else if (urlRequest == "/change_login")
    {
        _requestChangeLogin(request);
    }
    else if (urlRequest == "/change_password")
    {
        _requestChangePassword(request);
    }
    else if (urlRequest == "/logout")
    {
        _requestLogout(request);
    }
    else if (urlRequest == "/find")
    {
        _requestFindUsers(request);
    }
    else if (urlRequest == "/get_chats")
    {
        _requestGetUserChats(request);
    }
    else if (urlRequest == "/check_session")
    {
        _requestCheckTimeSession(request);
    }
    else if (urlRequest == "/delete")
    {
        _requestDeleteUser(request);
    }

}
void HandlerRequest::_groupChat(const http_request& request, const std::string urlRequest)
{
    if (urlRequest == "/get_participants")
    {
        _requestGetChatParticipants(request);
    }
    else if (urlRequest == "/create")
    {
        _requestCreateNewChat(request);
    }
    else if (urlRequest == "/add_user")
    {
        _requestAddUserToChat(request);
    }
    else if (urlRequest == "/leave")
    {
        _requestLeaveChat(request);
    }
    else if (urlRequest == "/change_name")
    {
        _requestChangeChatName(request);
    }
}
void HandlerRequest::_groupMessages(const http_request& request, const std::string urlRequest)
{
    if (urlRequest == "/get")
    {
        _requestGetMessages(request);
    }
    else if (urlRequest == "/send")
    {
        _requestSendMessages(request);
    }
}

void HandlerRequest::_handle_post(http_request request) {

    std::string urlMain = to_string(request.relative_uri().to_string());
    std::string urlGroup = urlMain.substr( 0, urlMain.rfind("/") );
    std::string urlRequest = urlMain.substr( urlMain.rfind("/") );
    LOG_DEBUG("Processing URL");

    if (urlGroup == "/user")
    {
        _groupUser(request, urlRequest);
    }
    else if (urlGroup == "/chat")
    {
        _groupChat(request, urlRequest);
    }
    else if (urlGroup == "/messages")
    {
        _groupMessages(request, urlRequest);
    }

    LOG_DEBUG("Request put");
}

void HandlerRequest::AddQueueThread(bool& RunningServer)
{
    const std::string link = "http://localhost:8080/api";

    std::cout << "Your server address: " << link << std::endl;

    http_listener listener(to_wstring(link));
    
    listener.support(methods::POST, std::bind(&HandlerRequest::_handle_post, this, std::placeholders::_1));
    try
    {
        listener
                .open()
                .then([&listener](){std::cout<<"Starting..."<<std::endl;})
                .wait();
            while (RunningServer);
    }
    catch (std::exception const & e)
    {
         std::cout << e.what() << std::endl;
    }
    
} 