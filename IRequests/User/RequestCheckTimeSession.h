#pragma once
#include "../IRequests.h"

class RequestCheckTimeSession : public IRequests {
private:
    std::string user_access_token;
public:
    RequestCheckTimeSession(IDatabase* db, const std::string& userAccessToken);
    void DoRequest();
};
