#pragma once
#include "../IRequests.h"

class RequestFindUsers : public IRequests {
private:
    std::string user_login_part;
    std::string user_access_token;
public:
    RequestFindUsers(IDatabase* db, const std::string& userAccessToken, const std::string& userLoginPart);
    void DoRequest();
};
