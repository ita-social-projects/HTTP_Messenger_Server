#ifndef HandlerRequest_hpp
#define HandlerRequest_hpp

#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include <map>

#include "Database/MSSQLDatabase.h"

#include "stringtowstring.h"
#include "ThreadWorker.h"

#include "IRequests/IRequests.h"
#include "IRequests/RequestAddUserToTheChat.h"
#include "IRequests/RequestChangeLogin.h"
#include "IRequests/RequestChangePassword.h"
#include "IRequests/RequestCreateNewChat.h"
#include "IRequests/RequestFindUsers.h"
#include "IRequests/RequestGetChatParticipants.h"
#include "IRequests/RequestGetUserChats.h"
#include "IRequests/RequestLeaveChat.h"
#include "IRequests/RequestLogout.h"
#include "IRequests/RequestGetMessages.h"
#include "IRequests/RequestLogin.h"
#include "IRequests/RequestSendMessages.h"
#include "IRequests/RequestSignUp.h"

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
    
    //url groups
    void _groupUser                  (const http_request& request, const std::string urlRequest);
    void _groupChat                  (const http_request& request, const std::string urlRequest);
    void _groupMessages              (const http_request& request, const std::string urlRequest);

    // listener
    void _handle_get                 (http_request request);
    void _handle_post                (http_request request);
    void _handle_put                 (http_request request);
    void _handle_del                 (http_request request);

    // /user/...
    void _requestLogin               (const http_request& request);
    void _requestSignUp              (const http_request& request);
    void _requestChangeLogin         (const http_request& request);
    void _requestChangePassword      (const http_request& request);
    void _requestLogout              (const http_request& request);
    void _requestFindUser            (const http_request& request);

    // /chat/....
    void _requestGetChatParticipants (const http_request& request);
    void _requestGetUserChats        (const http_request& request);
    void _requestCreateNewChat       (const http_request& request);
    void _requestAddUserToChat       (const http_request& request);
    void _requestLeaveChat           (const http_request& request);

    // /messages/...
    void _requestGetMessages         (const http_request& request);
    void _requestSendMessage         (const http_request& request);



public:

    HandlerRequest()
    try : db()
    {

    }
    catch (const std::exception& e)
    {
           std::cerr << e.what() << std::endl;
            exit(EXIT_FAILURE);
    }
    


    void AddQueueThread(bool&);
};

#endif /* HandlerRequest_hpp */
