#pragma once
#include "../IRequests.h"

class RequestUpdateUserImage : public IRequests {
private:
    std::string user_access_token;
    std::string image;
public:
    RequestUpdateUserImage(IDatabase* db, const std::string& userAccessToken, const std::string& image);
    void DoRequest();
};
