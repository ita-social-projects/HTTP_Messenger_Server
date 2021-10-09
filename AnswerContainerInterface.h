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
	virtual ~AnswerContainerInterface();
	virtual void ProcessRequest(){}
	virtual void RespondOnRequest(){}
	bool IsDone();
	void MakeDone();

};