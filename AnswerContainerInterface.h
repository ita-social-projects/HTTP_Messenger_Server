#pragma once
#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include "IRequests/IRequests.h"
using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;
extern class IRequests;

class AnswerContainerInterface {
protected:
	http_request request;

	bool done = false;

public:
	json::value answer;
	status_code code;
	IRequests* requestProcessor;
	AnswerContainerInterface(http_request request, IRequests* requestProcessor);
	virtual void ProcessRequest(){}
	virtual void RespondOnRequest(){}
	virtual void SetAnswer(json::value answer) = 0;
	virtual void SetStatusCode(status_code code) = 0;
	bool IsDone();
	void MakeDone();

};