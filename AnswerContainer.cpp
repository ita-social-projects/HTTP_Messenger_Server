#include "answercontainer.h"
#include "MSSQLDatabase.h"
#include "stringtowstring.h"
#include <map>
#include <cpprest/http_listener.h>
#include <cpprest/json.h>
using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;

answercontainer::answercontainer(http_request request, IRequests* requestProcessor) :answercontainerinterface(request, requestProcessor) {

}

void answercontainer::processrequest()
{
	try {
		this->answer = this->requestProcessor->DoRequest();
		this->status_code = status_codes::Accepted;
	}
	catch(std::exception e){
		json::value answer;
		answer[L"what"] = json::value::string(to_wstring(std::string(e.what())));
		this->status_code = status_codes::BadRequest;
	}
	this->done = true;
}

void answercontainer::respondonrequest() {
	this->request.reply(this->status_code, this->answer);
}