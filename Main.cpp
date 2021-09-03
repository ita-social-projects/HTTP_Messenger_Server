#include "MSSQLDatabase.h"

int main()
{
	try
	{
		MSSQLDatabase db;

		User test("testlogin", "testpassword");
		db.SaveUserToDB(test);

		User u = db.GetUserFromDB("testlogin");
		std::cout << u << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
