#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <pistache/endpoint.h>
#include <pistache/router.h>


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





int main() {
    Http::Endpoint server(Address("*:8080")); 
    Rest::Router router;

    Routes::Post(router, "/api/data", Routes::bind(handlePost));

    server.init();
    server.setHandler(router.handler());
    server.serve();
}


