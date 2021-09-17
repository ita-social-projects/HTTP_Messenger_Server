#pragma once
#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include "IRequests/IRequests.h"
#include "MSSQLDatabase.h"
using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;

class answercontainer {
private:
	http_request request;
	json::value value;
	IRequests* worker;
public:
	bool done = false;
	answercontainer(http_request request,IRequests* worker);
	void processrequest();
};