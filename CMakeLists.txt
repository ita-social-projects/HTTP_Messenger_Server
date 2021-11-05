cmake_minimum_required(VERSION 3.2)
project(HTTP_Messenger_Server)

if (WIN32)
	set(vcpkg_installed_x64_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-windows)
	set(cpprestsdk_DIR ${vcpkg_installed_x64_DIR}/share/cpprestsdk)
	set(ZLIB_INCLUDE_DIR ${vcpkg_installed_x64_DIR}/include)
	set(ZLIB_LIBRARY ${vcpkg_installed_x64_DIR}/lib/zlib.lib)
	set(unofficial-brotli_DIR ${vcpkg_installed_x64_DIR}/share/unofficial-brotli)
	set(OPENSSL_ROOT_DIR ${vcpkg_installed_x64_DIR})
	set(OPENSSL_CRYPTO_LIBRARY ${vcpkg_installed_x64_DIR}/lib)
	set(OPENSSL_INCLUDE_DIR ${vcpkg_installed_x64_DIR}/include)
endif ()

find_package(cpprestsdk REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(ODBC REQUIRED)

if (APPLE)
	set_target_properties(cpprestsdk::cpprestsdk_openssl_internal PROPERTIES
			INTERFACE_COMPILE_DEFINITIONS "CPPREST_NO_SSL_LEAK_SUPPRESS"
			INTERFACE_LINK_LIBRARIES "OpenSSL::SSL")
endif ()

set(CMAKE_CXX_STANDARD 17)

set (EXCLUDE_DIR "/out/")
file (GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS "*.cpp")
foreach (TMP_PATH ${PROJECT_SOURCES})
    string (FIND ${TMP_PATH} ${EXCLUDE_DIR} EXCLUDE_DIR_FOUND)
    if (NOT ${EXCLUDE_DIR_FOUND} EQUAL -1)
        list (REMOVE_ITEM PROJECT_SOURCES ${TMP_PATH})
    endif ()
endforeach(TMP_PATH)

add_library("HTTP_Messenger_Server" STATIC ${PROJECT_SOURCES})
#add_executable("HTTP_Messenger_Server" ${PROJECT_SOURCES})

target_link_libraries(HTTP_Messenger_Server PRIVATE cpprestsdk::cpprest OpenSSL::SSL OpenSSL::Crypto odbc32 ${CMAKE_CURRENT_SOURCE_DIR}/Logger/Logger.lib)
