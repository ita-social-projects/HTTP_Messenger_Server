#ifndef HandlerRequest_hpp
#define HandlerRequest_hpp

#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include <map>

#include "Database/MSSQLDatabase.h"

#include "StringUtils.h"
#include "ThreadWorker.h"

#include "IRequests/IRequests.h"

#include "IRequests/User/RequestLogin.h"
#include "IRequests/User/RequestSignUp.h"
#include "IRequests/User/RequestChangeLogin.h"
#include "IRequests/User/RequestChangePassword.h"
#include "IRequests/User/RequestLogout.h"
#include "IRequests/User/RequestFindUsers.h"
#include "IRequests/User/RequestGetUserChats.h"
#include "IRequests/User/RequestDeleteUser.h"
#include "IRequests/User/RequestCheckTimeSession.h"

#include "IRequests/Chat/RequestGetChatParticipants.h"
#include "IRequests/Chat/RequestCreateNewChat.h"
#include "IRequests/Chat/RequestAddUserToTheChat.h"
#include "IRequests/Chat/RequestLeaveChat.h"
#include "IRequests/Chat/RequestChangeChatName.h"

#include "IRequests/Messages/RequestGetMessages.h"
#include "IRequests/Messages/RequestSendMessages.h"

using namespace utility;
using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace web::http::experimental::listener;
using namespace concurrency::streams;

class HandlerRequest
{
protected:
    MSSQLDatabase db;
    std::unique_ptr<ThreadWorker> worker;

    void _pushRequest(http_request& request, IRequests* irequest);
    void _pushRequest(const http_request& request, IRequests* irequest);
    
    //url groups
    void _groupUser                  (http_request& request, const std::string urlRequest);
    void _groupChat                  (const http_request& request, const std::string urlRequest);
    void _groupMessages              (const http_request& request, const std::string urlRequest);

    // /user/...
    void _requestLogin               (http_request& request);
    void _requestSignUp              (const http_request& request);
    void _requestChangeLogin         (const http_request& request);
    void _requestChangePassword      (const http_request& request);
    void _requestLogout              (const http_request& request);
    void _requestFindUsers           (const http_request& request);
    void _requestGetUserChats        (const http_request& request);
    void _requestCheckTimeSession    (const http_request& request);
    void _requestDeleteUser          (const http_request& request);

    // /chat/....
    void _requestGetChatParticipants (const http_request& request);
    void _requestCreateNewChat       (const http_request& request);
    void _requestAddUserToChat       (const http_request& request);
    void _requestLeaveChat           (const http_request& request);
    void _requestChangeChatName      (const http_request& request);

    // /messages/...
    void _requestGetMessages         (const http_request& request);
    void _requestSendMessages        (const http_request& request);

    // listener
    void _handle_post(http_request& request);

public:

    HandlerRequest(bool MultiThreading)
    try : db(), worker(!MultiThreading ? nullptr : std::make_unique<ThreadWorker>())
    {
        LOG_DEBUG("Created HandlerRequest");
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }

 


    void AddQueueThread(bool& RunningServer);
};

#endif /* HandlerRequest_hpp */
