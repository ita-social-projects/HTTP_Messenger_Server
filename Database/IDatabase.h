#pragma once

#include <vector>
#include "./Exception/QueryException.h"
#include "Models/User.h"
#include "Models/Message.h"
#include "Models/Chat.h"

class IDatabase
{
public:
	virtual ISXModel::User GetUserFromDB(const std::string& user_login) = 0;
	virtual std::vector<ISXModel::User> GetChatParticipantsFromDB(const std::string& chat_title) = 0;
	virtual bool CheckUser(const ISXModel::User& user) = 0;
	virtual bool SaveUserToDB(const ISXModel::User& user) = 0;
	virtual bool AddUserToChat(const std::string& user_login, const std::string& chat_title) = 0;
	virtual bool RemoveUserFromChat(const std::string& user_login, const std::string& chat_title) = 0;
	virtual bool RemoveUserFromDB(const std::string& user_login) = 0;

	virtual ISXModel::Message GetMessageFromDB(const unsigned long& message_id) = 0;
	virtual std::vector<ISXModel::Message> GetChatMessagesFromDB(const std::string& chat_title) = 0;
	virtual bool SaveMessageToDB(const ISXModel::Message& message) = 0;
	virtual bool RemoveMessageFromDB(const unsigned long& message_id) = 0;

	virtual ISXModel::Chat GetChatFromDB(const std::string& chat_title) = 0;
	virtual std::vector<ISXModel::Chat> GetUserChatsFromDB(const std::string& user_login) = 0;
	virtual bool SaveChatToDB(const ISXModel::Chat& chat) = 0;
	virtual bool RemoveChatFromDB(const std::string& chat_title) = 0;
};
