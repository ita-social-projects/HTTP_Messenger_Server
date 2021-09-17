#pragma once
#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include "IRequests/IRequests.h"
#include "MSSQLDatabase.h"
#include "AnswerContainerInterface.h"
using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;

class answercontainer: public answercontainerinterface{
private:
	status_code status_code;
	json::value answer;
public:
	answercontainer(http_request request,IRequests* requestProcessor);
	void processrequest();
	void respondonrequest();
};