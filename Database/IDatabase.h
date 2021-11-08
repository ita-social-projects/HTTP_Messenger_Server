#pragma once

#include <vector>
#include <tuple>

#include "Models/User.h"
#include "Models/Message.h"
#include "Models/Chat.h"
#include "Exception/QueryException.h"
#include "../Logger/Logger.h"

struct DatabaseConnectionSettings
{
	std::string driver;
	std::string server;
	std::string database;
	std::string uid;
	std::string pwd;

	bool operator==(const DatabaseConnectionSettings& obj) const
	{
		return std::tie(this->driver, this->server, this->database, this->uid, this->pwd)
			== std::tie(obj.driver, obj.server, obj.database, obj.uid, obj.pwd);
	}
};

class IDatabase
{
public:
	virtual void Connect() = 0;
	virtual void Disconnect() = 0;

	virtual ISXModel::User GetUserFromDB(const std::string& user_access_token) = 0;
	virtual std::vector<ISXModel::User> GetUsersFromDBLike(const std::string& user_access_token, const std::string& search_string) = 0;
	virtual std::vector<ISXModel::User> GetChatParticipantsFromDB(const std::string& user_access_token, const unsigned long& chat_id) = 0;
	virtual std::string GetUserAccessToken(const std::string& user_login, const std::string& user_password) = 0;
	virtual unsigned long SaveUserToDB(const ISXModel::User& user) = 0;
	virtual bool UpdateUserAccessTokenUsedDateInDB(const std::string& user_access_token) = 0;
	virtual bool UpdateUserLoginInDB(const std::string& user_access_token, const std::string& new_login) = 0;
	virtual bool UpdateUserPasswordInDB(const std::string& user_access_token, const std::string& old_password, const std::string& new_password) = 0;
	virtual bool UpdateUserImageInDB(const std::string& user_access_token, const std::string& new_image_str) = 0;
	virtual bool AddUserToChat(const std::string& user_access_token, const std::string& user_login, const unsigned long& chat_id) = 0;
	virtual bool RemoveUserFromChat(const std::string& user_access_token, const std::string& user_login, const unsigned long& chat_id) = 0;
	virtual bool RemoveUserAccessToken(const std::string& user_access_token) = 0;
	virtual bool RemoveUserFromDB(const std::string& user_access_token) = 0;

	virtual ISXModel::Message GetMessageFromDB(const std::string& user_access_token, const unsigned long& message_id) = 0;
	virtual std::vector<ISXModel::Message> GetChatMessagesFromDB(const std::string& user_access_token, const unsigned long& chat_id,
																 const unsigned long& last_message_id) = 0;
	virtual unsigned long SaveMessageToDB(const std::string& user_access_token, const ISXModel::Message& message) = 0;
	virtual bool UpdateMessageContentInDB(const std::string& user_access_token, const unsigned long& message_id, const std::wstring& new_content) = 0;
	virtual bool RemoveMessageFromDB(const std::string& user_access_token, const unsigned long& message_id) = 0;

	virtual ISXModel::Chat GetChatFromDB(const std::string& user_access_token, const unsigned long& chat_id) = 0;
	virtual std::vector<ISXModel::Chat> GetUserChatsFromDB(const std::string& user_access_token) = 0;
	virtual unsigned long SaveChatToDB(const std::string& user_access_token, const ISXModel::Chat& chat) = 0;
	virtual bool UpdateChatTitleInDB(const std::string& user_access_token, const unsigned long& chat_id, const std::wstring& new_title) = 0;
	virtual bool UpdateChatImageInDB(const std::string& user_access_token, const unsigned long& chat_id, const std::string& new_image_str) = 0;
	virtual bool RemoveChatFromDB(const std::string& user_access_token, const unsigned long& chat_id) = 0;
};
