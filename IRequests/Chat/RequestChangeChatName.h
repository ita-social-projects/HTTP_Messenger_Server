#pragma once
#include "../IRequests.h"

class RequestChangeChatName : public IRequests {
private:
    std::string user_access_token;
    unsigned long chat_id;
    std::wstring new_title;
public:
    RequestChangeChatName(IDatabase* db, const std::string& userAccessToken, const unsigned long& chat_id, const std::wstring& new_title);
    void DoRequest();
};
