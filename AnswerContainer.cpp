#include "answercontainer.h"
#include "stringtowstring.h"
#include <map>
#include <cpprest/http_listener.h>
#include <cpprest/json.h>
using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;

AnswerContainer::AnswerContainer(http_request request, IRequests* requestProcessor) :AnswerContainerInterface(request, requestProcessor) {
}

void AnswerContainer::ProcessRequest()
{
	std::pair<json::value,int> result = this->requestProcessor->DoRequest();
	this->answer = result.first;
}

void AnswerContainer::RespondOnRequest() {
	this->request.reply(status_codes::Accepted, this->answer);
}


status_code AnswerContainer::GetStatusCode()
{
	return code;
}