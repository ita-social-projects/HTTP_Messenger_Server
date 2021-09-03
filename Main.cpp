#include "MSSQLDatabase.h"

int main()
{
	try
	{
		MSSQLDatabase db;

		db.AddUserToChat("user2", "test chat");
		db.AddUserToChat("user2", "chat2");

		std::vector<Chat> chats = db.GetUserChatsFromDB("user2");
		for (const Chat& chat : chats)
			std::cout << chat;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
