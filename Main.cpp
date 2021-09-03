#include "MSSQLDatabase.h"

int main()
{
	try
	{
		MSSQLDatabase db;

		std::vector<ISXModel::Message> messages = db.GetChatMessagesFromDB("test chat");
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
