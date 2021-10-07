#pragma once
#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include "IRequests/IRequests.h"
#include "AnswerContainerInterface.h"
using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;

class AnswerContainer: public AnswerContainerInterface{
public:
	AnswerContainer(http_request request,IRequests* requestProcessor);
	void ProcessRequest();
	void RespondOnRequest();
	void SetAnswer(json::value answer) override;
	void SetStatusCode(status_code code) override;
	status_code GetStatusCode();

};