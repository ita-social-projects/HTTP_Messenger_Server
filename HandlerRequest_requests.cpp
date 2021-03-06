#include "HandlerRequest.h"


// /user/...
void HandlerRequest::_requestLogin               (const http_request& request)
{
    json::value value = request.extract_json().get();

    const json::value login = value[L"login"];
    const json::value pass = value[L"pass"];

    if (login.is_null() || pass.is_null())
    {
        LOG_ERROR("Inalid JSON");
        request.reply(status_codes::BadRequest, "Invalid JSON");

        return;
    }

    const std::string USER_LOGIN = to_string(login.as_string());
    const std::string USER_PASS = to_string(pass.as_string());

    IRequests* irequest = new RequestLogin(&db, USER_LOGIN, USER_PASS);
    _pushRequest(request, irequest);

    LOG_DEBUG("Login DONE");
}

void HandlerRequest::_requestSignUp              (const http_request& request)
{
    json::value value = request.extract_json().get();

    const json::value login = value[L"login"];
    const json::value pass = value[L"pass"];

    if (login.is_null() || pass.is_null())
    {
        LOG_ERROR("Inalid JSON");
        request.reply(status_codes::BadRequest, "Invalid JSON");

        return;
    }

    const std::string USER_LOGIN = to_string(login.as_string());
    const std::string USER_PASS = to_string(pass.as_string());

    IRequests* irequest = new RequestSignUp( &db, ISXModel::User(USER_LOGIN, USER_PASS) );
    _pushRequest(request, irequest);

}

void HandlerRequest::_requestChangeLogin         (const http_request& request)
{
    json::value value = request.extract_json().get();

    const json::value token = value[L"token"];
    const json::value new_login = value[L"login"];

    if (token.is_null() || new_login.is_null())
    {
        LOG_ERROR("Inalid JSON");
        request.reply(status_codes::BadRequest, "Invalid JSON");

        return;
    }

    const std::string TOKEN          = to_string(token.as_string());
    const std::string USER_NEW_LOGIN = to_string(new_login.as_string());

    IRequests* irequest = new RequestChangeLogin( &db, TOKEN, USER_NEW_LOGIN );
    _pushRequest(request, irequest);

}

void HandlerRequest::_requestChangePassword      (const http_request& request)
{
    json::value value = request.extract_json().get();

    const json::value token = value[L"token"];
    const json::value old_password = value[L"pass"];
    const json::value new_password = value[L"new_pass"];

    if ( token.is_null() || old_password.is_null() || new_password.is_null() )
    {
        LOG_ERROR("Inalid JSON");
        request.reply(status_codes::BadRequest, "Invalid JSON");

        return;
    }

    const std::string TOKEN = to_string(token.as_string());
    const std::string USER_OLD_PASSWORD = to_string(old_password.as_string());
    const std::string USER_NEW_PASSWORD = to_string(new_password.as_string());

    IRequests* irequest = new RequestChangePassword( &db, TOKEN, USER_OLD_PASSWORD, USER_NEW_PASSWORD );
    _pushRequest(request, irequest);

}

void HandlerRequest::_requestLogout              (const http_request& request)
{
    json::value value = request.extract_json().get();

    const json::value token = value[L"token"];

    if (token.is_null())
    {
        LOG_ERROR("Inalid JSON");
        request.reply(status_codes::BadRequest, "Invalid JSON");

        return;
    }

    const std::string TOKEN = to_string( token.as_string() );

    IRequests* irequest = new RequestLogout( &db, TOKEN );
    _pushRequest(request, irequest);

}

void HandlerRequest::_requestFindUsers           (const http_request& request)
{
    json::value value = request.extract_json().get();

    const json::value token = value[L"token"];
    const json::value search_string = value[L"search_string"];

    if ( token.is_null() || search_string.is_null() )
    {
        LOG_ERROR("Inalid JSON");
        request.reply(status_codes::BadRequest, "Invalid JSON");

        return;
    }

    const std::string TOKEN = to_string(token.as_string());
    const std::string SEARCH_STRING = to_string(search_string.as_string());

    IRequests* irequest = new RequestFindUsers(&db, TOKEN, SEARCH_STRING);
    _pushRequest(request, irequest);

}

void HandlerRequest::_requestGetUserChats        (const http_request& request)
{
    json::value value = request.extract_json().get();

    const json::value token = value[L"token"];

    if (token.is_null())
    {
        LOG_ERROR("Inalid JSON");
        request.reply(status_codes::BadRequest, "Invalid JSON");

        return;
    }

    const std::string TOKEN = to_string(token.as_string());

    IRequests* irequest = new RequestGetUserChats(&db, TOKEN);
    _pushRequest(request, irequest);

}

void HandlerRequest::_requestCheckTimeSession    (const http_request& request)
{
    json::value value = request.extract_json().get();

    const json::value token = value[L"token"];

    if (token.is_null())
    {
        LOG_ERROR("Inalid JSON");
        request.reply(status_codes::BadRequest, "Invalid JSON");

        return;
    }

    const std::string TOKEN = to_string(token.as_string());

    IRequests* irequest = new RequestCheckTimeSession(&db, TOKEN);
    _pushRequest(request, irequest);
}

void HandlerRequest::_requestDeleteUser          (const http_request& request)
{
    json::value value = request.extract_json().get();

    const json::value token = value[L"token"];

    if (token.is_null())
    {
        LOG_ERROR("Inalid JSON");
        request.reply(status_codes::BadRequest, "Invalid JSON");

        return;
    }

    const std::string TOKEN = to_string(token.as_string());

    IRequests* irequest = new RequestDeleteUser(&db, TOKEN);
    _pushRequest(request, irequest);
}

void HandlerRequest::_requestUpdateUserImmage(const http_request& request)
{
    json::value value = request.extract_json().get();

    const json::value token = value[L"token"];
    const json::value image = value[L"image"];

    if (token.is_null(), image.is_null())
    {
        LOG_ERROR("Inalid JSON");
        request.reply(status_codes::BadRequest, "Invalid JSON");

        return;
    }

    const std::string TOKEN = to_string(token.as_string());
    const std::string IMAGE = to_string(image.as_string());

    IRequests* irequest = new RequestUpdateUserImage(&db, TOKEN, IMAGE);
    _pushRequest(request, irequest);
}


// /chat/....
void HandlerRequest::_requestGetChatParticipants (const http_request& request)
{
    json::value value = request.extract_json().get();

    const json::value token = value[L"token"];
    const json::value chat_id = value[L"chat_id"];

    if (token.is_null() || chat_id.is_null())
    {
        LOG_ERROR("Inalid JSON");
        request.reply(status_codes::BadRequest, "Invalid JSON");

        return;
    }

    const std::string TOKEN = to_string(token.as_string());
    const unsigned long CHAT_ID = (chat_id.as_number().to_uint32());

    IRequests* irequest = new RequestGetChatParticipants(&db, TOKEN, CHAT_ID);
    _pushRequest(request, irequest);

}

void HandlerRequest::_requestCreateNewChat       (const http_request& request) {
    json::value value = request.extract_json().get();

    const json::value token = value[L"token"];
    const json::value chat_title = value[L"chat_title"];

    if (token.is_null() || chat_title.is_null())
    {
        LOG_ERROR("Inalid JSON");
        request.reply(status_codes::BadRequest, "Invalid JSON");

        return;
    }

    const std::string TOKEN = to_string(token.as_string());
    const std::wstring CHAT_TITLE = chat_title.as_string();

    IRequests* irequest = new RequestCreateNewChat(&db, TOKEN, CHAT_TITLE);
    _pushRequest(request, irequest);

}

void HandlerRequest::_requestAddUserToChat       (const http_request& request)
{
    json::value value = request.extract_json().get();

    const json::value token = value[L"token"];
    const json::value chat_id = value[L"chat_id"];
    const json::value user_login = value[L"login"];

    if (token.is_null() || chat_id.is_null() || user_login.is_null())
    {
        LOG_ERROR("Inalid JSON");
        request.reply(status_codes::BadRequest, "Invalid JSON");

        return;
    }

    const std::string TOKEN = to_string(token.as_string());
    const unsigned long CHAT_ID = (chat_id.as_number().to_uint32());
    const std::string USER_LOGIN = to_string(user_login.as_string());

    IRequests* irequest = new RequestAddUserToTheChat(&db, TOKEN, CHAT_ID, USER_LOGIN);
    _pushRequest(request, irequest);

}

void HandlerRequest::_requestLeaveChat           (const http_request& request)
{
    json::value value = request.extract_json().get();

    const json::value token = value[L"token"];
    const json::value chat_id = value[L"chat_id"];
    const json::value user_login = value[L"login"];


    if (token.is_null() || chat_id.is_null() || user_login.is_null())
    {
        LOG_ERROR("Inalid JSON");
        request.reply(status_codes::BadRequest, "Invalid JSON");

        return;
    }

    const std::string TOKEN = to_string(token.as_string());
    const unsigned long CHAT_ID = (chat_id.as_number().to_uint32());
    const std::string USER_LOGIN = to_string(user_login.as_string());

    IRequests* irequest = new RequestLeaveChat(&db, TOKEN, CHAT_ID, USER_LOGIN);
    _pushRequest(request, irequest);

}

void HandlerRequest::_requestChangeChatName      (const http_request& request)
{
    json::value value = request.extract_json().get();

    const json::value token = value[L"token"];
    const json::value chat_id = value[L"chat_id"];
    const json::value title = value[L"chat_title"];

    if (token.is_null() || title.is_null() || chat_id.is_null())
    {
        LOG_ERROR("Inalid JSON");
        request.reply(status_codes::BadRequest, "Invalid JSON");

        return;
    }

    const std::string TOKEN = to_string(token.as_string());
    const unsigned long CHAT_ID = (chat_id.as_number().to_uint32());
    const std::wstring TITLE = title.as_string();

    IRequests* irequest = new RequestChangeChatName(&db, TOKEN, CHAT_ID, TITLE);
    _pushRequest(request, irequest);
}

void HandlerRequest::_requestUpdateChatImmage    (const http_request& request)
{
    json::value value = request.extract_json().get();

    const json::value token = value[L"token"];
    const json::value chat_id = value[L"chat_id"];
    const json::value image = value[L"image"];

    if (token.is_null(), image.is_null(), chat_id.is_null(), chat_id.is_null())
    {
        LOG_ERROR("Inalid JSON");
        request.reply(status_codes::BadRequest, "Invalid JSON");

        return;
    }

    const std::string TOKEN = to_string(token.as_string());
    const unsigned long CHAT_ID = (chat_id.as_number().to_uint32());
    const std::string IMAGE = to_string(image.as_string());

    IRequests* irequest = new RequestUpdateChatImage(&db, TOKEN, CHAT_ID, IMAGE);
    _pushRequest(request, irequest);
}



// /messages/...
void HandlerRequest::_requestSendMessages         (const http_request& request)
{
    json::value value = request.extract_json().get();

    const json::value token = value[L"token"];
    const json::value message = value[L"message"];
    const json::value chat_id = value[L"chat_id"];

    if (token.is_null() || message.is_null() || chat_id.is_null())
    {
        LOG_ERROR("Inalid JSON");
        request.reply(status_codes::BadRequest, "Invalid JSON");

        return;
    }

    const std::string TOKEN = to_string(token.as_string());
    const std::wstring MESSAGE = message.as_string();
    const unsigned long CHAT_ID = (chat_id.as_number().to_uint32());

    IRequests* irequest = new RequestSendMessages(&db, TOKEN, ISXModel::Message(MESSAGE, CHAT_ID));
    _pushRequest(request, irequest);

}

void HandlerRequest::_requestGetMessages         (const http_request& request)
{
    json::value value = request.extract_json().get();

    const json::value token = value[L"token"];
    const json::value chat_id = value[L"chat_id"];
    const json::value last_message_id = value[L"last_message_id"];

    if (token.is_null() ||   chat_id.is_null() || last_message_id.is_null())
    {
        LOG_ERROR("Inalid JSON");
        request.reply(status_codes::BadRequest, "Invalid JSON");

        return;
    }

    const std::string TOKEN = to_string(token.as_string());
    const unsigned long LAST_MESSAGE_ID = (last_message_id.as_number().to_uint32());
    const unsigned long CHAT_ID = (chat_id.as_number().to_uint32());

    IRequests* irequest = new RequestGetMessages(&db, TOKEN, CHAT_ID, LAST_MESSAGE_ID);
    _pushRequest(request, irequest);

}