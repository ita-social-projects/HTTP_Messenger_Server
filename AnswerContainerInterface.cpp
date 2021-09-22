#include "answercontainerinterface.h"
#include "stringtowstring.h"
#include <map>
#include <cpprest/http_listener.h>
#include <cpprest/json.h>
using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;

AnswerContainerInterface::AnswerContainerInterface(http_request request, IRequests* requestProcessor)
{
	this->request = request;
	this->requestProcessor = requestProcessor;
	this->requestProcessor->setAnswerContainer(this);

}

bool AnswerContainerInterface::IsDone()
{
	return this->done;
}

void AnswerContainerInterface::MakeDone()
{
	 this->done = true;
}
