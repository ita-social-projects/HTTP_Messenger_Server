#include "answercontainer.h"
#include "MSSQLDatabase.h"
#include "stringtowstring.h"
#include <map>
#include <cpprest/http_listener.h>
#include <cpprest/json.h>
using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;
answercontainer::answercontainer(http_request request, IRequests* worker )
{
	this->request = request;
	this->value = request.extract_json().wait();
	this->worker = worker;
}

void answercontainer::processrequest()
{
	try {
		this->request.reply(status_codes::Accepted, this->worker->DoRequest());
	}
	catch(std::exception e){
		json::value answer;
		//answer[L"what"] = json::value::string(to_wstring(std::string(e.what())));
		this->request.reply(status_codes::BadRequest, answer);
	}
}

