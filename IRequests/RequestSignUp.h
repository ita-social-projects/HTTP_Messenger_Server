#pragma once
#include <cpprest/json.h>
using namespace web;

class RequestSignUp : public IRequests {
private:
    ISXModel::User user;
public:
    RequestSignUp(IDatabase* db, const ISXModel::User& user);
    std::pair<json::value, int>  DoRequest();
};

