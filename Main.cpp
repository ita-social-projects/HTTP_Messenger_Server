#include "MSSQLDatabase.h"

int main()
{
	try
	{
		MSSQLDatabase db;
		db.ExecuteQuery("select @@version");
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
