#pragma once
#include "../IRequests.h"

class RequestGetChatParticipants : public IRequests {
private:
    std::string user_access_token;
    unsigned long chat_id;
public:
    RequestGetChatParticipants(IDatabase* db, const std::string& userAccessToken,const unsigned long& chatId );
    void DoRequest();
};
