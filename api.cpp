#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <pistache/endpoint.h>
#include <pistache/router.h>
#include <sstream>


using namespace Pistache;
using namespace Pistache::Rest; 
using namespace std;



void write_info(string data){
    ofstream file("/Users/ivan/rest_api/data.txt",std::ios::app);
    if(!file.is_open()){
        std::cout << "File wasnt opened" << endl;
    }    
    file << data << "\n";
    file.close();
}

// Обработчик POST-запроса
void handlePost(const Rest::Request& request, Http::ResponseWriter response) {
    std::string body = request.body();  // Получаем тело запроса
    std::cout << "Получено тело: " << body << std::endl;
    write_info(body);
    cout << "Data had been writen" << endl;
    response.send(Http::Code::Ok, "Данные получены");
}


string get_file_data(){
    ifstream file("/Users/ivan/rest_api/data.txt");

    if(!file.is_open()){
        std::cerr << "File wasnt opened" << endl;
        return "No data";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();  // читаем весь файл в буфер

    file.close();
    std::string content = buffer.str(); // содержимое файла
    return content;

}


void handleGet(const Request& request, Http::ResponseWriter response){
    string data = get_file_data();
    response.send(Http::Code::Ok,data);
}





int main() {
    Http::Endpoint server(Address("*:8080")); 
    Rest::Router router;

    Routes::Post(router, "/api/data", Routes::bind(handlePost));//post
    Routes::Get(router,"/api/view",Routes::bind(handleGet));//get
    server.init();
    server.setHandler(router.handler());
    server.serve();
}


