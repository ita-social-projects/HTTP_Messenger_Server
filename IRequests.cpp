//
// Created by pavlo on 9/2/21.
//

#include "IRequests.h"
#include "stringtowstring.h"
#include <vector>
IRequests::IRequests(MSSQLDatabase* db) : db(db) {}

RequestGetChats::RequestGetChats(MSSQLDatabase* db, const std::string& userLogin) : IRequests(db),
user_login(userLogin) {}

json::value RequestGetChats::DoRequest() {
    json::value result;
    std::vector<ISXModel::Chat> chatList = db->GetUserChatsFromDB(this->user_login);
    result[L"size"] = json::value::Number(chatList.size());
    json::value chats;
    for (int i = 0; i < chatList.size(); i++) {
        json::value current = json::value();
        current[L"id"] = json::value::Number(chatList[i].get_id());
        current[L"title"] = json::value::string(to_wstring(chatList[i].get_title()));
        chats[i] = current;
    }
    result[L"chats"] = chats;
    return result;
}

RequestGetMessages::RequestGetMessages(MSSQLDatabase* db, const std::string& chatTitle) : IRequests(db), chat_title(chatTitle) {}

json::value RequestGetMessages::DoRequest() {
    json::value result;
    std::vector<ISXModel::Message> messageList = db->GetChatMessagesFromDB(this->chat_title);
    result[L"size"] = messageList.size();
    json::value messages;
    for (int i = 0; i < messageList.size(); i++) {
        json::value current = json::value();
        current[L"message_id"] = json::value::Number(messageList[i].get_id());
        current[L"sender_id"] = json::value::Number(messageList[i].get_sender_id());
        current[L"chat_id"] = json::value::Number(messageList[i].get_chat_id());
        current[L"text"] = json::value::string(to_wstring(messageList[i].get_content()));
        messages[i] = current;
    }
    result[L"messages"] = messages;
    return result;
}


RequestSendMessages::RequestSendMessages(MSSQLDatabase* db, const ISXModel::Message& message) : IRequests(db)
{

}

json::value RequestSendMessages::DoRequest() {
    json::value result;
    db->SaveMessageToDB(this->message);
    result[L"status"] = json::value::string(L"OK");
    return result;
}                                                                                              


RequestSignUp::RequestSignUp(MSSQLDatabase* db, const ISXModel::User& user) : IRequests(db), user(user) {}

json::value RequestSignUp::DoRequest() {
    json::value result;
    db->SaveUserToDB(this->user);
    ISXModel::User currentUser = db->GetUserFromDB(this->user.get_login());
    result[L"Login"] = json::value::string(to_wstring(currentUser.get_login()));
    result[L"id"] = json::value::Number(currentUser.get_id());
    return result;
}

RequestLogin::RequestLogin(MSSQLDatabase* db, const std::string& login, const std::string& password) : IRequests(db),
login(login),
password(password) {}

//Split into files
json::value RequestLogin::DoRequest() {
    json::value result;
    //Hashing
        ISXModel::User currentUser = db->GetUserFromDB(this->login);
        if (currentUser.get_password() == this->password) {
            result[L"id"] = json::value::Number(currentUser.get_id());
            result[L"Login"] = json::value::string(to_wstring(currentUser.get_login()));
            //Return user and token(mb) 
        }
        else {

        }
        return result;
}
