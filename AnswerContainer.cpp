#include "answercontainer.h"
#include "stringtowstring.h"
#include <map>
#include <cpprest/http_listener.h>
#include <cpprest/json.h>
using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;

AnswerContainer::AnswerContainer(http_request request, IRequests* requestProcessor) :AnswerContainerInterface(request, requestProcessor) {
	LOG_DEBUG("Creating answercontainer");
}

void AnswerContainer::ProcessRequest()
{
	this->requestProcessor->DoRequest();
	LOG_DEBUG("Making request");
}

void AnswerContainer::RespondOnRequest() {
	LOG_DEBUG("Making reply with status_code:"+std::to_string(this->code));
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
