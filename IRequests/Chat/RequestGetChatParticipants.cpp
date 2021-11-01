#include "RequestGetChatParticipants.h"

RequestGetChatParticipants::RequestGetChatParticipants(IDatabase* db,const std::string& userAccessToken, const unsigned long& chatId) : IRequests(db), user_access_token(userAccessToken), chat_id(chatId) {}

void RequestGetChatParticipants::DoRequest()
{
    json::value result;
    try {
        std::vector<ISXModel::User> userList = db->GetChatParticipantsFromDB(this->user_access_token,this->chat_id);
        result[L"size"] = (int)userList.size();
		json::value users = json::value::array();
        for (int i = 0; i < userList.size(); i++) {
            json::value current = json::value();
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
}
