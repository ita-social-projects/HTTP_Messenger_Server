#include "HandlerRequest.h"

void HandlerRequest::_requestLogin           (const http_request& request)
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

    IRequests* temp = new RequestLogin(&db, USER_LOGIN, USER_PASS);
    AnswerContainer* t1 = new AnswerContainer(request, temp);

    temp->setAnswerContainer(t1);

    worker.PushRequest(t1);
}

void HandlerRequest::_requestSignUp          (const http_request& request)
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

    IRequests* temp = new RequestSignUp( &db, ISXModel::User(USER_LOGIN, USER_PASS) );
    AnswerContainer* t1 = new AnswerContainer(request, temp);

    temp->setAnswerContainer(t1);

    worker.PushRequest(t1);
}

void HandlerRequest::_requestChangeLogin     (const http_request& request)
{
    json::value value = request.extract_json().get();

    const json::value token = value[L"token"];
    const json::value new_login = value[L"new_login"];

    if (token.is_null() || new_login.is_null())
    {
        LOG_ERROR("Inalid JSON");
        request.reply(status_codes::BadRequest, "Invalid JSON");

        return;
    }

    const std::string TOKEN          = to_string(token.as_string());
    const std::string USER_NEW_LOGIN = to_string(new_login.as_string());

    IRequests* temp = new RequestChangeLogin( &db, TOKEN, USER_NEW_LOGIN );
    AnswerContainer* t1 = new AnswerContainer(request, temp);

    temp->setAnswerContainer(t1);

    worker.PushRequest(t1);
}

void HandlerRequest::_requestChangePassword  (const http_request& request)
{
    json::value value = request.extract_json().get();

    const json::value token = value[L"token"];
    const json::value old_password = value[L"old_password"];
    const json::value new_password = value[L"new_password"];

    if ( token.is_null() || old_password.is_null() || new_password.is_null() )
    {
        LOG_ERROR("Inalid JSON");
        request.reply(status_codes::BadRequest, "Invalid JSON");

        return;
    }

    const std::string TOKEN = to_string(token.as_string());
    const std::string USER_OLD_PASSWORD = to_string(old_password.as_string());
    const std::string USER_NEW_PASSWORD = to_string(new_password.as_string());

    IRequests* temp = new RequestChangePassword( &db, TOKEN, USER_OLD_PASSWORD, USER_NEW_PASSWORD );
    AnswerContainer* t1 = new AnswerContainer( request, temp );

    temp->setAnswerContainer(t1);

    worker.PushRequest(t1);
}

void HandlerRequest::_requestLogout          (const http_request& request)
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

    IRequests* temp = new RequestLogout( &db, TOKEN );
    AnswerContainer* t1 = new AnswerContainer(request, temp);

    temp->setAnswerContainer(t1);

    worker.PushRequest(t1);
}

void HandlerRequest::_requestFindUser        (const http_request& request)
{
    json::value value = request.extract_json().get();

    const json::value token = value[L"token"];
    const json::value find_user = value[L"find_user"];

    if ( token.is_null() || find_user.is_null() )
    {
        LOG_ERROR("Inalid JSON");
        request.reply(status_codes::BadRequest, "Invalid JSON");

        return;
    }

    const std::string TOKEN = to_string(token.as_string());
    const std::string FIND_USER = to_string(find_user.as_string());

    IRequests* temp = new RequestFindUsers(&db, TOKEN, FIND_USER);
    AnswerContainer* t1 = new AnswerContainer(request, temp);

    temp->setAnswerContainer(t1);

    worker.PushRequest(t1);
}
