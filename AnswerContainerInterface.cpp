#include "answercontainerinterface.h"
#include "StringUtils.h"
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

}

AnswerContainerInterface::~AnswerContainerInterface()
{
	delete requestProcessor;
}


