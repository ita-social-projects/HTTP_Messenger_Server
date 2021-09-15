#include <iostream>
#include <cpprest/http_listener.h>
#include <cpprest/json.h>

using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;
#include <map>


void handle_get(http_request request) {
    if (request.relative_uri().to_string() == L"/login")
    {
        json::value answer1;
        answer1[L"temp"] = json::value(1);
        json::value answer;
        answer[0] = answer1;
        json::value wrapper;
        wrapper[L"list"] = answer;
        request.reply(status_codes::OK, wrapper);
        std::cout << "Replied\n";
    }
    request.reply(status_codes::OK, "Nothing special");
}


void handle_post(http_request request) {
    if (request.relative_uri().to_string() == L"/user/login")
    {
        json::value answer = request.extract_json().get();
        std::wstring val = answer[L"OK"].as_string();
        std::cout << std::string(val.begin(), val.end())<<"\n";
        request.reply(status_codes::OK, answer);
    }
    else
        if (request.relative_uri().to_string() == L"/user/register")
        {
            json::value answer;
            answer[L"OK"] = json::value::string(L"YES");
            request.reply(status_codes::OK, answer);
        }
    std::cout << "Handling post!\n";
}

void handle_put(http_request request) {
    std::cout << "Handling put!\n";
}

void handle_del(http_request request) {
    std::cout << "Handling delete!\n";
}

int main() {
    http_listener listener(L"http://localhost:8080/restdemo");
    listener.support(methods::GET, handle_get);
    listener.support(methods::POST, handle_post);
    listener.support(methods::PUT, handle_put);
    listener.support(methods::DEL, handle_del);
    try
    {
        listener
            .open()
            .then([&listener]() {std::cout << "Starting..." << std::endl; })
            .wait();
        while (true);
    }
    catch (std::exception const& e)
    {
        std::cout << e.what() << std::endl;
    }
}

