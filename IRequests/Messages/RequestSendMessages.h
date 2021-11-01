#pragma once
#include "../IRequests.h"

class RequestSendMessages : public IRequests {
private:
    ISXModel::Message message;
    std::string user_access_token;
public:
    RequestSendMessages(IDatabase* db, const std::string& userAccessToken, const ISXModel::Message& message);
    void DoRequest();
};

