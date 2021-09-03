cmake_minimum_required(VERSION 3.2)
project(backend)
find_package(cpprestsdk REQUIRED)
set_target_properties(cpprestsdk::cpprestsdk_openssl_internal PROPERTIES
        INTERFACE_COMPILE_DEFINITIONS "CPPREST_NO_SSL_LEAK_SUPPRESS"
        INTERFACE_LINK_LIBRARIES "OpenSSL::SSL"
        )

set(CMAKE_CXX_STANDARD 17)
add_executable(backend main.cpp IRequests.cpp IRequests.h ThreadWorker.cpp ThreadWorker.h)
target_link_libraries(backend PRIVATE cpprestsdk::cpprest)
