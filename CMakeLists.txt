cmake_minimum_required(VERSION 3.2)
project(HTTP_Messenger_Server)

if (WIN32)
	set(vcpkg_installed_x64_DIR vcpkg\\installed\\x64-windows)
	set(cpprestsdk_DIR ${CMAKE_CURRENT_SOURCE_DIR}\\${vcpkg_installed_x64_DIR}\\share\\cpprestsdk)
	set(ZLIB_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}\\${vcpkg_installed_x64_DIR}\\include)
	set(ZLIB_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}\\${vcpkg_installed_x64_DIR}\\lib\\zlib.lib)
	set(unofficial-brotli_DIR ${CMAKE_CURRENT_SOURCE_DIR}\\${vcpkg_installed_x64_DIR}\\share\\unofficial-brotli)
endif ()

find_package(cpprestsdk REQUIRED)
find_package(ODBC REQUIRED)

if (APPLE)
	set_target_properties(cpprestsdk::cpprestsdk_openssl_internal PROPERTIES
			INTERFACE_COMPILE_DEFINITIONS "CPPREST_NO_SSL_LEAK_SUPPRESS"
			INTERFACE_LINK_LIBRARIES "OpenSSL::SSL")
endif ()

set(CMAKE_CXX_STANDARD 17)

add_executable(HTTP_Messenger_Server main.cpp MSSQLDatabase.cpp model/User.cpp model/Message.cpp model/Chat.cpp AnswerContainer.cpp stringtowstring.cpp IRequests/IRequests.cpp IRequests/RequestGetChats.cpp IRequests/RequestGetMessages.cpp IRequests/RequestSendMessages.cpp IRequests/RequestLogin.cpp IRequests/RequestSignUp.cpp AnswerContainerInterface.cpp )
target_link_libraries(HTTP_Messenger_Server PRIVATE cpprestsdk::cpprest odbc32)
