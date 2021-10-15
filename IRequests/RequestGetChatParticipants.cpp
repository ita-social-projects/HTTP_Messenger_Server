#include "RequestGetChatParticipants.h"

RequestGetChatParticipants::RequestGetChatParticipants(IDatabase* db, const std::string& chatTitle) : IRequests(db), chat_title(chatTitle) {}

void RequestGetChatParticipants::DoRequest()
{
    json::value result;
    try {
        std::vector<ISXModel::User> userList = db->GetChatParticipantsFromDB(this->chat);
        result[L"size"] = json::value::Number(userList.size());
        json::value users;
        for (int i = 0; i < chatList.size(); i++) {
            json::value current = json::value();
            current[L"id"] = (int)userList[i].get_id();
            current[L"login"] = json::value::string(to_wstring(userList[i].get_login()));
            chats[i] = current;
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
