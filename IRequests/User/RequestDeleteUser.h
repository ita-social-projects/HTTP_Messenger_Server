#pragma once
#include "../IRequests.h"

class RequestDeleteUser : public IRequests {
private:
    std::string user_access_token;
public:
    RequestDeleteUser(IDatabase* db, const std::string& userAccessToken);
    void DoRequest();
};
