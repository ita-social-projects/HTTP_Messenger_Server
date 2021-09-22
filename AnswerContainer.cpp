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
	this->requestProcessor->DoRequest();
}

void AnswerContainer::RespondOnRequest() {
	this->request.reply(this->status_code, this->answer);
}