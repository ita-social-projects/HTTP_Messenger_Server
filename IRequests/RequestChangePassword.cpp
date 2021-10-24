#include "RequestChangePassword.h"

RequestChangePassword::RequestChangePassword(IDatabase* db, const std::string& userAccessToken,const std::string& oldPassword, const std::string& newPassword)
    : IRequests(db), 
    user_access_token(userAccessToken), 
    old_password(oldPassword), 
    new_password(newPassword) {}

void RequestChangePassword::DoRequest()
{
    json::value result;
    try {
        if (this->db->UpdateUserPasswordInDB(this->user_access_token,this->old_password,this->new_password)) {
            result[L"status"] = json::value::string(to_wstring("OK"));
            this->answercontainer->SetStatusCode(status_codes::OK);
        }
        else {
            result[L"status"] = json::value::string(to_wstring("Cannot process request."));
            this->answercontainer->SetStatusCode(status_codes::Unauthorized);
        }
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
