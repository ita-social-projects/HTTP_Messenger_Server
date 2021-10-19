#pragma once

#include <vector>

#include "Models/User.h"
#include "Models/Message.h"
#include "Models/Chat.h"
#include "Exception/QueryException.h"
#include "../Logger/Logger.h"

class IDatabase
{
public:
	virtual ISXModel::User GetUserFromDB(const std::string& user_access_token, const unsigned long& user_id) = 0;
	virtual std::vector<ISXModel::User> GetUsersFromDBLike(const std::string& user_access_token, const std::string& search_string) = 0;
	virtual std::vector<ISXModel::User> GetChatParticipantsFromDB(const std::string& user_access_token, const unsigned long& chat_id) = 0;
	virtual std::string GenerateUserAccessToken(const std::string& user_login, const std::string& user_password) = 0;
	virtual unsigned long SaveUserToDB(const ISXModel::User& user) = 0;
	virtual bool UpdateUserLoginInDB(const std::string& user_access_token, const std::string& user_login) = 0;
	virtual bool UpdateUserPasswordInDB(const std::string& user_access_token, const std::string& old_password, const std::string& new_password) = 0;
	virtual bool AddUserToChat(const std::string& user_access_token, const std::string& user_login, const unsigned long& chat_id) = 0;
	virtual bool RemoveUserFromChat(const std::string& user_access_token, const std::string& user_login, const unsigned long& chat_id) = 0;
	virtual bool RemoveUserAccessToken(const std::string& user_access_token) = 0;
	virtual bool RemoveUserFromDB(const std::string& user_access_token) = 0;

	virtual ISXModel::Message GetMessageFromDB(const std::string& user_access_token, const unsigned long& message_id) = 0;
	virtual std::vector<ISXModel::Message> GetChatMessagesFromDB(const std::string& user_access_token, const unsigned long& chat_id, const unsigned long& last_message_id) = 0;
	virtual unsigned long SaveMessageToDB(const std::string& user_access_token, const ISXModel::Message& message) = 0;
	virtual bool RemoveMessageFromDB(const std::string& user_access_token, const unsigned long& message_id) = 0;

	virtual ISXModel::Chat GetChatFromDB(const std::string& user_access_token, const unsigned long& chat_id) = 0;
	virtual std::vector<ISXModel::Chat> GetUserChatsFromDB(const std::string& user_access_token) = 0;
	virtual unsigned long SaveChatToDB(const std::string& user_access_token, const ISXModel::Chat& chat) = 0;
	virtual bool RemoveChatFromDB(const std::string& user_access_token, const unsigned long& chat_id) = 0;
};
