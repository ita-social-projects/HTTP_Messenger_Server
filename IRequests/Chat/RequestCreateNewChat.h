#pragma once
#include "../IRequests.h"

class RequestCreateNewChat : public IRequests {
private:
    std::string user_access_token;
    std::wstring chat_title;
public:
    RequestCreateNewChat(IDatabase* db, const std::string& userAccessToken, const std::wstring& chatTitle);
    void DoRequest();
};
