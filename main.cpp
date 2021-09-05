#include <iostream>
#include <cpprest/http_listener.h>
#include <cpprest/json.h>

using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;
#include <map>


void handle_get(http_request request){
	if (request.relative_uri().to_string() == "/login")
	{
		json::value answer;
		answer["OK"] = json::value::string("YES");
		request.reply(status_codes::OK,answer);
	}
	request.reply(status_codes::OK, "Nothing special");
}


void handle_post(http_request request){
    if (request.relative_uri().to_string() == "/user/login")
	{
		json::value answer;
		answer["OK"] = json::value::string("YES");
		request.reply(status_codes::OK,answer);
	}else
    if (request.relative_uri().to_string() == "/user/register")
	{
		json::value answer;
		answer["OK"] = json::value::string("YES");
		request.reply(status_codes::OK,answer);
	}
    std::cout<<"Handling put!\n";
}
void handle_put(http_request request){
    std::cout<<"Handling put!\n";
}
void handle_del(http_request request){
    std::cout<<"Handling delete!\n";
}

int main() {
	http_listener listener("http://localhost:8080/restdemo");
    listener.support(methods::GET, handle_get);
    listener.support(methods::POST, handle_post);
    listener.support(methods::PUT, handle_put);
    listener.support(methods::DEL, handle_del);
    try
    {
        listener
                .open()
				.then([&listener](){std::cout<<"Starting..."<<std::endl;})
                .wait();
        while (true);
    }
    catch (std::exception const & e)
    {
        std::cout << e.what() << std::endl;
    }

}
