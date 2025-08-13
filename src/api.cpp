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





//split function
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
    ofstream file("/Users/ivan/rest_api/data/data.txt",std::ios::app);
    if(!file.is_open()){
        std::cout << "File wasnt opened" << endl;
    }    
    file << data << "\n";
    file.close();
}

void write_user_to_json(string username,string password){
    json data;
    ifstream file("/Users/ivan/rest_api/data/users.json");
    if(file.is_open()){
        file >> data;
        file.close();
    }
    data[username] = password;

    ofstream out_file("/Users/ivan/rest_api/data/users.json");
    if(out_file.is_open()){
        out_file << data.dump(4); 
        out_file.close();
    }
    else{
        std::cerr << "file wasnt opened" << endl;
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

    ifstream file("/Users/ivan/rest_api/data/users.json");

    //checking if username is already taken by other user
    json dt;
    if(file.is_open()){
        file >> dt;
    }
    if(dt.contains(username)){
        response.send(Http::Code::Bad_Request,"Username is already taken");

    }
    else{
        write_user_to_json(username,hash_password);
        response.send(Http::Code::Ok,"User had been writen to the database"); 
    }

    
}

void check_user_validation(const Rest::Request& request,Http::ResponseWriter response){
    string user_data  = request.body();
    vector<string> d = split(user_data);

    string username = d[0];
    string password = d[1];

    ifstream file("/Users/ivan/rest_api/data/users.json");

    if (!file.is_open())
    {
        std::cerr << "Error opening file" << endl;
        response.send(Http::Code::Not_Found,"File wasnt opened");

    }
    else{
        json data;

        file >> data;

        if(data.contains(username)){
            try{
                string true_password = data[username];
                if(true_password == password){
                    response.send(Http::Code::Ok,"True");
                
                }
                else{
                    response.send(Http::Code::Bad_Request,"False");
                }
            }catch(exception& e){
                response.send(Http::Code::Not_Found,e.what());
            }

        }
        else{
            response.send(Http::Code::Not_Found,"Username doesnt exist");
        }

    }
}

//если делать парсинг бд на стороне API ,то это POST запрос
void get_user_history(const Rest::Request& request, Http::ResponseWriter response){
    json data;
    ifstream file("/Users/ivan/rest_api/data/history.json");

    string username = request.body();

    if(!file.is_open()){
        std::cerr << "File wasnt opened" << endl;
        response.send(Http::Code::Bad_Request,"File wasnt opened");

    }
    else{
        file >> data;

        try{
            for(const auto& user:data){
                if(user["username"] == username){
                    response.send(Http::Code::Ok,user["username"]);
                }
            }
        }catch(exception& e){
            response.send(Http::Code::Bad_Request,"User not found");
        }
    }
}

//get запрос к базе для выдачи истории пользователя

void get_user_history_getrequest(const Rest::Request& request,Http::ResponseWriter response){
    auto username_opt = request.query().get("username");
    if(!username_opt.has_value()){
        response.send(Http::Code::Not_Acceptable,"Empty username");
        return;
    }
    string username = username_opt.value();
    json main_data;

    ifstream file("/Users/ivan/rest_api/data/history.json");
    bool found = false;
    if(!file.is_open()){
        response.send(Http::Code::Bad_Request,"Database wasnt opened");
        return;
    }
    else{
        file >> main_data;
        try{
            for(const auto& user:main_data){
                if(user["username"] == username){
                    response.send(Http::Code::Ok,user["messages"]);
                    found = true;
                }
            }
            if(!found){
                response.send(Http::Code::Ok,"User wanst found");
            }

        }catch(exception& e){
            response.send(Http::Code::Ok,e.what());
            return;
        }
    }

}

void write_data_to_user_history(const Rest::Request& request,Http::ResponseWriter response){
    json main_data;

    vector<string> send_data = split(request.body());

    string username = send_data[0]; // username
    string messages_to_write = send_data[1]; // user chat hsitory with ai (form fronend)


    ifstream file("/Users/ivan/rest_api/data/history.json");

    if(file.is_open()){
        file >> main_data;
        file.close();
    }
    else{
        response.send(Http::Code::Not_Found,"File wasnt opened");
    }


    try{
        for(auto& user : main_data){
            if(user["username"] == username){
                try{
                    user["messages"].push_back(messages_to_write);
                }catch(exception& e){
                    response.send(Http::Code::Bad_Request,e.what());
                }
            }

        }
    }catch(exception& e){
        response.send(Http::Code::Not_Found,e.what());
        std::cerr << e.what() << endl;
    }

    ofstream out_file("/Users/ivan/rest_api/data/history.json");

    if(out_file.is_open()){
        out_file << main_data.dump(4);
        out_file.close();
        response.send(Http::Code::Ok,"Success");

    }
    else{
        response.send(Http::Code::Bad_Request,"Data wasnt writen 2");
    }


}

void write_default_history(const Rest::Request& request,Http::ResponseWriter response){
    string username = request.body();

    json data;

    ifstream file("/Users/ivan/rest_api/data/history.json");

    if(file.is_open()){
        file >> data;
        file.close();
    }
    else{
        std::cerr << "Error while opening 1" << endl;
        response.send(Http::Code::Bad_Request,"Error while opening 1");

    }

    json new_user = {
        {"username",username},
        {"messages",json::array()}
    };

    data.push_back(new_user);
    ofstream out_put_file("/Users/ivan/rest_api/data/history.json");
    if(out_put_file.is_open()){
        out_put_file << data.dump(4);
        out_put_file.close();
        response.send(Http::Code::Ok,"Success");
    }
    else{
        std::cerr << "Error while opening file 2" << endl;
        response.send(Http::Code::Bad_Request,"Error while opening file 2");
    }

}


void show_user_password(const Rest::Request& request, Http::ResponseWriter response){
    ifstream file("/Users/ivan/rest_api/data/history.json");
    if(!file.is_open()){
        std::cerr << "Error while opening file" << endl;
        return;

    }

    json user_data;

    file >> user_data;

    string username = request.body();
    if(user_data.contains(username)){
        bool is_username = false;
        try{
            string password = user_data[username];
            is_username = true;
            response.send(Http::Code::Ok,password);
        }catch(exception& e){
            response.send(Http::Code::Ok,e.what());
        }    
    }
    else{
        response.send(Http::Code::Not_Found,"User not found");
    }    
}



string get_file_data(){
    ifstream file("/Users/ivan/rest_api/data/data.txt");

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


void write_default_user_messages_history(const::Rest::Request& request,Http::ResponseWriter response) {
    
    auto username_opt = request.query().get("username");
    if (!username_opt.has_value()) {
        response.send(Http::Code::Bad_Request,"Empty username");
        return;
    }
    string username = username_opt.value();
    ifstream file("/Users/ivan/rest_api/data/user_messages.json");
    json data;
    if(file.is_open()) {
        file >> data;
        file.close();
    }
    else {
        response.send(Http::Code::Bad_Request,"Error while openeing file1");
        return;
    }
    json new_user = {
        {"username",username},
        {"messages",json::array()}
    };
    data.push_back(new_user);

    ofstream exit_file("/Users/ivan/rest_api/data/user_messages.json");
    if (exit_file.is_open()) {
        exit_file << data.dump(4);
        exit_file.close();
        response.send(Http::Code::Ok,"Data loaded");
    }
    else {
        response.send(Http::Code::Bad_Request,"Error while writing exit data tofile2");
    }
}



int main() {
    Http::Endpoint server(Address("*:8080")); 
    Rest::Router router;
    Routes::Post(router, "/api/data", Routes::bind(handlePost_Ai));//post
    Routes::Get(router,"/api/view",Routes::bind(handleGet));//get
    Routes::Post(router,"/api/write",Routes::bind(register_user));
    Routes::Post(router,"/api/getpassword",Routes::bind(show_user_password));
    Routes::Post(router,"/api/validate",Routes::bind(check_user_validation));
    Routes::Post(router,"/api/history",Routes::bind(get_user_history));
    Routes::Post(router,"/api/defhistory",Routes::bind(write_default_history));
    Routes::Post(router,"/api/wrhistory",Routes::bind(write_data_to_user_history));
    Routes::Get(router, "/get_history", Routes::bind(get_user_history_getrequest));
    server.init();
    server.setHandler(router.handler());
    server.serve();
}


