#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <pistache/endpoint.h>
#include <pistache/router.h>
#include <sstream>
#include <nlohmann/json.hpp>
#include <ctime>
#include <chrono>


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
void write_logs(string log) {
    ifstream file("/Users/ivan/rest_api/data/logs.txt");
    if (!file.is_open()) {
        std::cerr << "Error while opening logs" << endl;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    string logs = buffer.str();
    logs += "\n";
    logs += log + "\n";
    ofstream exit_file("/Users/ivan/rest_api/data/logs.txt");
    if (!exit_file.is_open()) {
        std::cerr << "Error while writing changes" << endl;
    }
    exit_file << logs;
    exit_file.close();
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
    write_logs(body);
    std::cout << "Получено тело: " << body << std::endl;
    write_info(body);
    cout << "Data had been writen" << endl;
    response.send(Http::Code::Ok, "Данные получены");
}


void register_user(const Rest::Request& request,Http::ResponseWriter response){
    string user_data = request.body();
    write_logs(user_data);
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
        try {
            ifstream file("/Users/ivan/rest_api/data/user_messages.json");
            if (!file.is_open()) {
                response.send(Http::Code::Not_Found,"State wasnt changed");
            }
            json state_data;
            file >> state_data;
            file.close();
            state_data[username] = true;

            ofstream exit("/Users/ivan/rest_api/data/user_messages.json");
            if (!exit.is_open()) {
                response.send(Http::Code::Bad_Request,"Error while writing");
            }
            else {
                exit << state_data.dump(4);
                exit.close();
                response.send(Http::Code::Ok,"Success");
            }
            
        }catch (exception& e) {
            response.send(Http::Code::Bad_Request,e.what());
        }
        response.send(Http::Code::Ok,"User had been writen to the database"); 
    }

    
}

void check_user_validation(const Rest::Request& request,Http::ResponseWriter response){
    string user_data  = request.body();
    vector<string> d = split(user_data);
    write_logs(user_data);
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
    write_logs(username);
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
    write_logs(username);
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
    write_logs(send_data[0]);
    string username = send_data[0]; // username
    string messages_to_write = send_data[1]; // user chat hsitory with ai (form fronend)
    write_logs(send_data[1]);

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
    write_logs(username);
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
    write_logs(username);
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

void delete_from_specail_json(string path,string username) {
    json data;
    ifstream file(path);
    if(!file.is_open()) {
        std::cerr << "Error while opening file" << endl;
        
    }
    else {
        file >> data;
        file.close();
    }
    if (!data.contains(username)) {
        std::cerr << "No such user" << endl;
        return;
    }
    for (auto it = data.begin();it != data.end(); ) {
        if ((*it)["username"] == username) {
            it = data.erase(it);
        }
        else {
            ++it;
        }
    }
    ofstream exit(path);
    if (!exit.is_open()) {
        std::cerr << "Error while writing result" << endl;
    }
    else {
        exit << data.dump(4);
        exit.close();
    }
}

void change_user_state(const Rest::Request& request, Http::ResponseWriter response) {
    try {
        string body = request.body();
        auto data = json::parse(body);
        string username = data["username"];
        bool state = data["state"];
        ifstream file("/Users/ivan/rest_api/data/loged_in.json");
        if (!file.is_open()) {
            response.send(Http::Code::Bad_Request,"Error while opening file");
        }
        json main_data;
        file >> main_data;
        file.close();
        if (main_data.contains(username)) {
            try {
                main_data[username] = state;
            }catch(exception& e) {
                response.send(Http::Code::Bad_Request,e.what());
            }
        }
        else {
            response.send(Http::Code::Not_Found,"User not found");
        }

        ofstream exit_file("/Users/ivan/rest_api/data/loged_in.json");
        if (!exit_file.is_open()) {
            response.send(Http::Code::Bad_Request,"Error while writing file");
        }
        else {
            exit_file << main_data.dump(4);
            exit_file.close();
            response.send(Http::Code::Ok,"Success");
        }
    }catch(exception& e) {
        response.send(Http::Code::Bad_Request,e.what());
    }
}

void change_password(const::Rest::Request& request, Http::ResponseWriter response) {
    try {
        string body = request.body();
        auto main_data = json::parse(body);
        const string username = main_data["username"];
        const string password = main_data["password"];
        const string new_password = main_data["new_password"];

        ifstream file("/Users/ivan/rest_api/data/users.json");
        if (!file.is_open()) {
            response.send(Http::Code::Bad_Request,"Error while opening file");
        }
        json database;
        file >> database;
        file.close();

        if (!database.contains(username)) {
            response.send(Http::Code::Not_Found,"User not found");
        }
        try {
            if (database[username] == password) {
                database[username] = new_password;
                ofstream exit("/Users/ivan/rest_api/data/users.json");
                if (!exit.is_open()) {
                    response.send(Http::Code::Bad_Request,"Error while writing file");
                }
                else {
                    exit << database.dump(4);
                    exit.close();
                    response.send(Http::Code::Ok,"Success");
                    
                }
            }
            else {
                response.send(Http::Code::Not_Found,"User password is incorrect");
            }
        }catch(exception& e) {
            response.send(Http::Code::Bad_Request,e.what());
        }

    }catch (exception& e) {
        response.send(Http::Code::Bad_Request,e.what());
    }
}

void check_loged_in(const Rest::Request& request,Http::ResponseWriter response) {
    auto username_opt = request.query().get("username");
    if (!username_opt.has_value()) {
        response.send(Http::Code::Bad_Request,"Empty username");
        
    }
    json data;
    ifstream file("/Users/ivan/rest_api/data/loged_in.json");
    if (!file.is_open()) {
        response.send(Http::Code::Bad_Request,"Error while opening file");
    }
    else {
        file >> data;
        file.close();
    }
    string username = username_opt.value();
    if (data.contains(username)) {
        try {
            response.send(Http::Code::Ok,data[username]);
        }catch (exception& e) {
            response.send(Http::Code::Bad_Request,e.what());
        }
    }
    else {
        response.send(Http::Code::Not_Found,"User not found");
    }
    
}


void delete_user_data(const::Rest::Request& request, Http::ResponseWriter response) {
    string username = request.body();
    write_logs(username);
    //deleting from reqiester database
    ifstream file("/Users/ivan/rest_api/data/users.json");
    if (!file.is_open()) {
        response.send(Http::Code::Bad_Request,"File wasnt opened");
        return;
    }
    json data1;
    file >> data1;
    file.close();

    if (data1.contains(username)) {
        data1.erase(username);
        ofstream exit_username_file("/Users/ivan/rest_api/data/users.json");
        if (exit_username_file.is_open()) {
            exit_username_file << data1.dump(4);
            exit_username_file.close();
            response.send(Http::Code::Ok,"User was deleted from users.json");
        }
        else {
            response.send(Http::Code::Bad_Request,"Error while writing changes");
        }
    }
    else {
        response.send(Http::Code::Not_Found,"User not found");
    }

    //deleting from history
    ifstream file_history("/Users/ivan/rest_api/data/history.json");
    if (!file_history.is_open()) {
        response.send(Http::Code::Bad_Request," History File wasnt opened");
    }
    json data_history;
    file_history >> data_history;
    file_history.close();
    for (auto it = data_history.begin(); it != data_history.end(); ) {
        if ((*it)["username"] == username) {
            it = data_history.erase(it);
        }
        else {
            ++it;
        }
    }
    ofstream file_exit_history("/Users/ivan/rest_api/data/history.json");
    if (!file_exit_history.is_open()) {
        response.send(Http::Code::Bad_Request,"Error while deleting history data");
        
    }
    else {
        file_exit_history << data_history.dump(4);
        file_exit_history.close();
        response.send(Http::Code::Ok,"Deleted data from history");

    }
    try {
        delete_from_specail_json("/Users/ivan/rest_api/data/user_messages.json",username);
        response.send(Http::Code::Ok,"deleted");
    }catch (exception& e) {
        response.send(Http::Code::Bad_Request,e.what());
    }
    
}



void handleGet(const Request& request, Http::ResponseWriter response){
    string data = get_file_data();
    write_logs(data);
    response.send(Http::Code::Ok,data);
}

//get функция именно для записы default user messages (только сообщения пользователя)
void write_default_user_messages_history(const::Rest::Request& request,Http::ResponseWriter response) {
    
    auto username_opt = request.query().get("username");
    if (!username_opt.has_value()) {
        response.send(Http::Code::Bad_Request,"Empty username");
        return;
    }
    string username = username_opt.value();
    ifstream file("/Users/ivan/rest_api/data/user_messages.json");
    json data;
    write_logs(username);
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

void write_user_message(const Rest::Request& request,Http::ResponseWriter response) {
    ifstream file("/Users/ivan/rest_api/data/user_messages.json");
    json data;
    vector<string> sended_data = split(request.body());
    string username = sended_data[0];
    string data_to_write = sended_data[1];
    write_logs(username);
    write_logs(data_to_write);
    if(file.is_open()) {
        file >> data;
        file.close();
    }
    else {
        response.send(Http::Code::Bad_Request,"Error while opening");
        return;
    }
    bool user_ex = false;
    for (auto& user:data) {
        if (user["username"] == username) {
            user["messages"].push_back(data_to_write);
            user_ex = true;
        }
    }
    if (!user_ex) {
        response.send(Http::Code::Not_Found,"User not found");
        return;
    }
    else{
        ofstream exit_file("/Users/ivan/rest_api/data/user_messages.json");
        if (exit_file.is_open()) {
            exit_file << data.dump(4);
            exit_file.close();
            response.send(Http::Code::Ok,"Data loaded");
        }
        else {
            response.send(Http::Code::Bad_Request,"Error while writing data");
        }
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
    Routes::Get(router, "/api/get_history", Routes::bind(get_user_history_getrequest));
    Routes::Get(router,"/api/def_user_messages",Routes::bind(write_default_user_messages_history));
    Routes::Post(router,"/api/wrhmessagehistory",Routes::bind(write_user_message));
    Routes::Post(router,"/api/delete_user",Routes::bind(delete_user_data));
    Routes::Get(router,"/api/check_loged_in",Routes::bind(check_loged_in));
    Routes::Post(router,"/api/change_state",Routes::bind(change_user_state));
    Routes::Post(router,"/api/change_password",Routes::bind(change_password));
    server.init();
    server.setHandler(router.handler());
    server.serve();
}


