#pragma once
#include "../IRequests.h"

class RequestUpdateChatImage : public IRequests {
private:
    std::string user_access_token;
    unsigned long chat_id;
    std::string image;
public:
    RequestUpdateChatImage(IDatabase* db, const std::string& userAccessToken, const unsigned long& chat_id, const std::string& image);
    void DoRequest();
};
