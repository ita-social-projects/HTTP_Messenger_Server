#include "MSSQLDatabase.h"

int main()
{
	try
	{
		MSSQLDatabase db;

		db.SaveMessageToDB(ISXModel::Message("new message", 3, 2));

		std::vector<ISXModel::Message> messages = db.GetChatMessagesFromDB("chat2");
		for (const ISXModel::Message& message : messages)
			std::cout << message;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
