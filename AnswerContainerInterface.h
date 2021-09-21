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
	IRequests* requestProcessor;
	json::value answer;
	status_code code;
	bool done = false;
public:
	AnswerContainerInterface(http_request request, IRequests* requestProcessor);
	virtual void ProcessRequest(){}
	virtual void RespondOnRequest(){}
	virtual void SetAnswer(json::value answer) {}
	virtual void SetStatusCode(status_code code) {}
	bool IsDone();
	void MakeDone();

};