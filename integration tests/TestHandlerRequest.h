#ifndef CMAKEPROJECT2_TESTHANDLERREQUEST_H
#define CMAKEPROJECT2_TESTHANDLERREQUEST_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <iostream>
#include <map>

#include "../HTTP_Messenger_Server/HandlerRequest.h"

enum RequestType
{
	Login,
	SignUp,
	Delete,
	ChangeLogin,
	RChangeLogin,
	ChangePassword,
	RChangePassword,
	Logout,
	GetChats,
	SendMessages,
	GetMessages,
	CreateChat
};

class TestHandlerRequest : public HandlerRequest
{
private:
	std::string accessToken;
	status_code statusCode;

	json::value _postRequest(json::value requestBody, std::string requestUrl);

	void _doLogin();
	void _doSignUp();
	void _doDelete();
	void _doChangeLogin();
	void _doRChangeLogin();
	void _doChangePassword();
	void _doRChangePassword();
	void _doGetChats();
	void _doLogout();

public:
	

	TestHandlerRequest() : HandlerRequest(false)
	{}

	void SetToken(std::string accessToken);
	std::string GetToken();

	void SetCode(status_code statusCode);
	status_code GetCode();

	json::value GenerateRequetBody(RequestType requestType);
	std::string GenerateRequestURL(RequestType requestType);

	void DoRequest(RequestType doRequest);

};

#endif //CMAKEPROJECT2_TESTHANDLERREQUEST_H