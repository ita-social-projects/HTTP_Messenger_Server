#pragma once

#include <vector>

#include "Models/User.h"
#include "Models/Message.h"
#include "Models/Chat.h"
#include "Exception/QueryException.h"

class IDatabase
{
public:
	virtual ISXModel::User GetUserFromDB(const unsigned long& user_id) = 0;
	virtual std::vector<ISXModel::User> GetUsersFromDBLike(const std::string& search_string) = 0;
	virtual std::vector<ISXModel::User> GetChatParticipantsFromDB(const unsigned long& chat_id) = 0;
	virtual std::string GenerateUserAccessToken(const std::string& user_login, const std::string& user_password) = 0;
	virtual bool SaveUserToDB(const ISXModel::User& user) = 0;
	virtual bool UpdateUserLoginInDB(const std::string& user_access_token, const std::string& user_login) = 0;
	virtual bool UpdateUserPasswordInDB(const std::string& user_access_token, const std::string& user_password) = 0;
	virtual bool AddUserToChat(const unsigned long& user_id, const unsigned long& chat_id) = 0;
	virtual bool RemoveUserFromChat(const unsigned long& user_id, const unsigned long& chat_id) = 0;
	virtual bool RemoveUserAccessToken(const std::string& user_access_token) = 0;
	virtual bool RemoveUserFromDB(const std::string& user_access_token) = 0;

	virtual ISXModel::Message GetMessageFromDB(const unsigned long& message_id) = 0;
	virtual std::vector<ISXModel::Message> GetChatMessagesFromDB(const unsigned long& chat_id) = 0;
	virtual bool SaveMessageToDB(const std::string& user_access_token, const ISXModel::Message& message) = 0;
	virtual bool RemoveMessageFromDB(const unsigned long& message_id) = 0;

	virtual ISXModel::Chat GetChatFromDB(const unsigned long& chat_id) = 0;
	virtual std::vector<ISXModel::Chat> GetUserChatsFromDB(const std::string& user_access_token) = 0;
	virtual bool SaveChatToDB(const std::string& user_access_token, const ISXModel::Chat& chat) = 0;
	virtual bool RemoveChatFromDB(const unsigned long& chat_id) = 0;
};
