#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <pistache/endpoint.h>
#include <pistache/router.h>
#include <sstream>
#include <nlohmann/json.hpp>


using namespace Pistache;
using namespace Pistache::Rest; 
using namespace std;
using json = nlohmann::json;






vector<string> split(string word){
    const long long int space_count = count(word.begin(),word.end(),' ');

    vector<string> res(space_count + 1,"");

    int i = 0;
    int j = 0;
    while(i < word.size() && j < res.size()){
        if(word[i] != ' '){
            res[j] += word[i];
            i++;
        }
        else{
            i++;
            j++;
        }
    }
    return res;

}

void write_info(string data){
    ofstream file("/Users/ivan/rest_api/data.txt",std::ios::app);
    if(!file.is_open()){
        std::cout << "File wasnt opened" << endl;
    }    
    file << data << "\n";
    file.close();
}

void write_user_to_json(string username,string password){
    json data;

    data[username] = password;
    ofstream file("/Users/ivan/rest_api/data/users.json");
    if(file.is_open()){
        file << data.dump(4);

    }
    else{
        cerr << "Error while writing json" << endl;
    }
}

// Обработчик POST-запроса
void handlePost_Ai(const Rest::Request& request, Http::ResponseWriter response) {
    std::string body = request.body();  // Получаем тело запроса
    std::cout << "Получено тело: " << body << std::endl;
    write_info(body);
    cout << "Data had been writen" << endl;
    response.send(Http::Code::Ok, "Данные получены");
}


void register_user(const Rest::Request& request,Http::ResponseWriter response){
    string user_data = request.body();
    vector<string> data = split(user_data);
    string username = data[0];
    string hash_password = data[1];
    write_user_to_json(username,hash_password);
    response.send(Http::Code::Ok,"User had been writen to the database");

    // need to load json FIXME 
}

void show_user_password(const Rest::Request& request, Http::ResponseWriter response){
    ifstream file("/Users/ivan/rest_api/data/users.json");
    if(!file.is_open()){
        std::cerr << "Error while opening file" << endl;
        return;

    }

    json user_data;

    file >> user_data;

    string username = request.body();
    bool is_username = false;
    try{
        string password = user_data[username];
        is_username = true;
        response.send(Http::Code::Ok,password);
    }catch(exception& e){
        response.send(Http::Code::Ok,"No such user");
    }    
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
    Routes::Post(router, "/api/data", Routes::bind(handlePost_Ai));//post
    Routes::Get(router,"/api/view",Routes::bind(handleGet));//get
    Routes::Post(router,"/api/write",Routes::bind(register_user));
    Routes::Post(router,"/api/getpassword",Routes::bind(show_user_password));
    server.init();
    server.setHandler(router.handler());
    server.serve();
}


