#pragma once
#include "../IRequests.h"

class RequestLogin : public IRequests {
private:
    std::string login, password;
public:
    RequestLogin(IDatabase* db, const std::string& login, const std::string& password);
    void DoRequest();
};
