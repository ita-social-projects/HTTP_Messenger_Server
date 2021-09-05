//
// Created by pavlo on 9/2/21.
//

#include "IRequests.h"
#include <vector>
IRequests::IRequests(MSSQLDatabase *db) : db(db) {}

RequestGetChats::RequestGetChats(MSSQLDatabase *db, const std::string &userLogin) : IRequests(db),
                                                                                    user_login(userLogin) {}

void RequestGetChats::DoStuff(){
    try {
        std::vector<ISXModel::Chat> chatList = db.GetUserChatsFromDB(this->user_login);
    }catch(...){

    }
}

RequestGetMessages::RequestGetMessages(MSSQLDatabase *db, const std::string &chatTitle) : IRequests(db),
                                                                                          chat_title(chatTitle) {}
void RequestGetMessages::DoStuff(){
    try {
        std::vector<ISXModel::Message> messageList = db.SetChatMessagesFromDB(this->chat_title);
    }catch(...){

    }
}


RequestSendMessages::RequestSendMessages(MSSQLDatabase *db, const ISXModel::Message &message) : IRequests(db),

void RequestSendMessages::DoStuff(){
    try {
        db.SaveMessageToDB(this->message);
    }
    catch(...){

    }
}                                                                                              message(message) {}


RequestSignUp::RequestSignUp(MSSQLDatabase *db, const ISXModel::User &user) : IRequests(db), user(user) {}

void RequestSignUp::DoStuff(){
    try{
        db.SaveUserToDB(this->User);
    }
    catch(...) {

    }
}

RequestLogin::RequestLogin(MSSQLDatabase *db, const std::string &login, const std::string &password) : IRequests(db),
                                                                                                       login(login),
                                                                                                       password(password) {}


void RequestLogin::DoStuff(){
    try{
        ISXModel::User currentUser = db.GetUserFromDB(this->login);
        if(currentUser.get_password() == this->password){

        }else{

        }
    }
}

