#include "answercontainer.h"
#include "StringUtils.h"
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
	this->requestProcessor->DoRequest();
}

void AnswerContainer::RespondOnRequest() {
	this->request.reply(this->requestProcessor->answercontainer->code, this->requestProcessor->answercontainer->answer);
}

void AnswerContainer::SetAnswer(json::value answer)
{
	this->answer = answer;
}

void AnswerContainer::SetStatusCode(status_code code)
{
	this->code = code;
}
