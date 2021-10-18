#include "RequestGetChatParticipants.h"

RequestGetChatParticipants::RequestGetChatParticipants(IDatabase* db,const std::string& userToken, const unsigned long& chatId) : IRequests(db), user_token(userToken), chat_id(chatId) {}

void RequestGetChatParticipants::DoRequest()
{
    json::value result;
    try {
        std::vector<ISXModel::User> userList = db->GetChatParticipantsFromDB(this->user_token,this->chat_id);
        result[L"size"] = json::value::Number(userList.size());
        json::value users;
        for (int i = 0; i < userList.size(); i++) {
            json::value current = json::value();
            current[L"token"] = json::value::string(to_wstring(userList[i].get_access_token()));
            current[L"login"] = json::value::string(to_wstring(userList[i].get_login()));
            users[i] = current;
        }
        result[L"users"] = users;
        this->answercontainer->SetStatusCode(status_codes::Accepted);
    }
    catch (const QueryException& e) {
        result[L"what"] = json::value::string(to_wstring(e.what()));
        this->answercontainer->SetStatusCode(status_codes::BadRequest);
    }
    catch (const std::exception& e) {
        result[L"what"] = json::value::string(to_wstring("Database error"));
        this->answercontainer->SetStatusCode(status_codes::InternalError);
    }
    this->answercontainer->SetAnswer(result);
    this->answercontainer->MakeDone();
}
