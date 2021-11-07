#include "HandlerRequest.h"

void HandlerRequest::_pushRequest(http_request& request, IRequests* irequest)
{
    AnswerContainer* answer = new AnswerContainer(request, irequest);

    irequest->setAnswerContainer(answer);

    if (worker != nullptr)
    {
        LOG_DEBUG("Process with threads");
        worker->PushRequest(answer);
    }
    else
    {
        LOG_DEBUG("Process without threads");
        answer->ProcessRequest();
        answer->RespondOnRequest();
        delete answer;
        answer = nullptr;
    }

}

void HandlerRequest::_pushRequest(const http_request& request, IRequests* irequest)
{
    AnswerContainer* answer = new AnswerContainer(request, irequest);

    irequest->setAnswerContainer(answer);

    if (worker != nullptr)
    {
        LOG_DEBUG("Process with threads");
        worker->PushRequest(answer);
    }
    else
    {
        LOG_DEBUG("Process without threads");
        answer->ProcessRequest();
        answer->RespondOnRequest();
        delete answer;
        answer = nullptr;
    }

}

void HandlerRequest::_groupUser (http_request& request, const std::string urlRequest)
{
    if (urlRequest == "/login")
    {
        LOG_DEBUG("Request /user/login");
        _requestLogin(request);
    }
    else if (urlRequest == "/sign_up")
    {
        LOG_DEBUG("Request /user/sign_up");
        _requestSignUp(request);
    }
    else if (urlRequest == "/change_login")
    {
        LOG_DEBUG("Request /user/change_login");
        _requestChangeLogin(request);
    }
    else if (urlRequest == "/change_password")
    {
        LOG_DEBUG("Request /user/change_password");
        _requestChangePassword(request);
    }
    else if (urlRequest == "/logout")
    {
        LOG_DEBUG("Request /user/logout");
        _requestLogout(request);
    }
    else if (urlRequest == "/find")
    {
        LOG_DEBUG("Request /user/find");
        _requestFindUsers(request);
    }
    else if (urlRequest == "/get_chats")
    {
        LOG_DEBUG("Request /user/get_chats");
        _requestGetUserChats(request);
    }
    else if (urlRequest == "/check_session")
    {
        LOG_DEBUG("Request /user/check_session");
        _requestCheckTimeSession(request);
    }
    else if (urlRequest == "/delete")
    {
        LOG_DEBUG("Request /user/delete");
        _requestDeleteUser(request);
    }
    else if (urlRequest == "/change_immage")
    {
        LOG_DEBUG("Request /user/change_immage");
        _requestUpdateUserImmage(request);
    }

}

void HandlerRequest::_groupChat(const http_request& request, const std::string urlRequest)
{
    if (urlRequest == "/get_participants")
    {
        LOG_DEBUG("Request /chat/get_participants");
        _requestGetChatParticipants(request);
    }
    else if (urlRequest == "/create")
    {
        LOG_DEBUG("Request /chat/create");
        _requestCreateNewChat(request);
    }
    else if (urlRequest == "/add_user")
    {
        LOG_DEBUG("Request /chat/add_user");
        _requestAddUserToChat(request);
    }
    else if (urlRequest == "/leave")
    {
        LOG_DEBUG("Request /chat/leave");
        _requestLeaveChat(request);
    }
    else if (urlRequest == "/change_name")
    {
        LOG_DEBUG("Request /chat/change_name");
        _requestChangeChatName(request);
    }
    else if (urlRequest == "/change_immage")
    {
        LOG_DEBUG("Request /chat/change_immage");
        _requestUpdateChatImmage(request);
    }
}

void HandlerRequest::_groupMessages(const http_request& request, const std::string urlRequest)
{
    if (urlRequest == "/get")
    {
        LOG_DEBUG("Request /messages/get");
        _requestGetMessages(request);
    }
    else if (urlRequest == "/send")
    {
        LOG_DEBUG("Request /messages/send");
        _requestSendMessages(request);
    }
}

void HandlerRequest::_handle_post(http_request &request) {

    std::string urlMain = to_string(request.relative_uri().to_string());
    std::string urlGroup = urlMain.substr( 0, urlMain.rfind("/") );
    std::string urlRequest = urlMain.substr( urlMain.rfind("/") );
    LOG_DEBUG("Your main URL: " + urlMain);

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
    else
    {
        LOG_ERROR("Wrong URL group");
    }

}

void HandlerRequest::AddQueueThread(bool& RunningServer)
{
    if (!RunningServer)
        return;

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