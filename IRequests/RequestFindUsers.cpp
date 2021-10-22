#include "RequestFindUsers.h"

RequestFindUsers::RequestFindUsers(IDatabase* db, const std::string& userAccesToken, const std::string& userLoginPart) : IRequests(db), user_access_token(userAccesToken), user_login_part(userLoginPart) {}

void RequestFindUsers::DoRequest()
{
    json::value result;
    try {
        std::vector<ISXModel::User> userList = db->GetUsersFromDBLike(this->user_access_token,this->user_login_part);
        result[L"size"] = int(userList.size());
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
    this->answercontainer->MakeDone();
}
