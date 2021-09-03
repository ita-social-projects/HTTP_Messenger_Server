#include "MSSQLDatabase.h"

int main()
{
	try
	{
		MSSQLDatabase db;

		User u = db.GetUserFromDB("admin");
		std::cout << u << std::endl;

		User u2 = db.GetUserFromDB("user2");
		std::cout << u2 << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
