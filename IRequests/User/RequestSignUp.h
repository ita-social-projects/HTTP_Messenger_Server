#pragma once
#include "../IRequests.h"

class RequestSignUp : public IRequests {
private:
    ISXModel::User user;
public:
    RequestSignUp(IDatabase* db, const ISXModel::User& user);
    void DoRequest();
};

