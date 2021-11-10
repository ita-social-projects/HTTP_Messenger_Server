cmake_minimum_required (VERSION 3.8)
project (GoogleTest)

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

set(CMAKE_CXX_STANDARD 17)

configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/googletest-download"
)
execute_process(COMMAND "${CMAKE_COMMAND}" --build .
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/googletest-download"
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)


add_subdirectory("${CMAKE_BINARY_DIR}/googletest-src"
                 "${CMAKE_BINARY_DIR}/googletest-build"
)

if(CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include"
                        "${gmock_SOURCE_DIR}/include"
    )
endif()

include_directories(out/build/x64-Debug/googletest-src/googlemock/include)
include_directories(out/build/x64-Debug/googletest-src/googletest/include)


set(SOURCE_FILES main.cpp)
add_executable (GoogleTest "main.cpp" "TestHandlerRequest.h" "TestHandlerRequest.cpp" "TESTS.h")

target_link_libraries(GoogleTest PRIVATE cpprestsdk::cpprest OpenSSL::SSL OpenSSL::Crypto odbc32 ${CMAKE_CURRENT_SOURCE_DIR}/Logger/Logger.lib gtest gtest_main ${CMAKE_CURRENT_SOURCE_DIR}/HTTP_Messenger_Server.lib)


