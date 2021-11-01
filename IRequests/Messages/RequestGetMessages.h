#pragma once
#include "../IRequests.h"

class RequestGetMessages : public IRequests {
private:
    std::string user_access_token;
    unsigned long chat_id, last_message_id;
public:
    RequestGetMessages(IDatabase* db, const std::string& userAccessToken, const unsigned long& chatId, const unsigned long& lastMessageId);

    void DoRequest();
};
