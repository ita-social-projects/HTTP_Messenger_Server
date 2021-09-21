#pragma once

#include <vector>

#include "Models/User.h"
#include "Models/Message.h"
#include "Models/Chat.h"

class IDatabase
{
public:
	virtual ISXModel::User GetUserFromDB(const std::string& user_login);
	virtual std::vector<ISXModel::User> GetChatParticipantsFromDB(const std::string& chat_title);
	virtual bool CheckUser(const ISXModel::User& user);
	virtual bool SaveUserToDB(const ISXModel::User& user);
	virtual bool AddUserToChat(const std::string& user_login, const std::string& chat_title);
	virtual bool RemoveUserFromChat(const std::string& user_login, const std::string& chat_title);
	virtual bool RemoveUserFromDB(const std::string& user_login);

	virtual ISXModel::Message GetMessageFromDB(const unsigned long& message_id);
	virtual std::vector<ISXModel::Message> GetChatMessagesFromDB(const std::string& chat_title);
	virtual bool SaveMessageToDB(const ISXModel::Message& message);
	virtual bool RemoveMessageFromDB(const unsigned long& message_id);

	virtual ISXModel::Chat GetChatFromDB(const std::string& chat_title);
	virtual std::vector<ISXModel::Chat> GetUserChatsFromDB(const std::string& user_login);
	virtual bool SaveChatToDB(const ISXModel::Chat& chat);
	virtual bool RemoveChatFromDB(const std::string& chat_title);
};
