import requests as r
import os



def pos_request():
    url = "http://localhost:8080/api/data"
    text_data = "Это просто обычный текст без JSON"
    headers = {"Content-Type": "text/plain"}

    response = r.post(url, data=text_data, headers=headers)

    print("Status code:", response.status_code)
    print("Response:", response.text)


def get_request():
    url = "http://localhost:8080/api/view"

    try:
        response = r.get(url)
        try:
            print("####### STATUS CODE ####### ")
            print(response.status_code)
        except Exception as e:
            print(f"No staus code :{e}")    
        return response.text
    except Exception as e:
        print(f"Error {e}")
        return ""

def write_default_user_history():
    url = "http://localhost:8080/api/defhistory";
    text_data = "ivan";
    headers = {"Content-Type": "text/plain"}

    try:
        res = r.post(url,data = text_data,headers=headers)
        print(f"Status code : {res.status_code}")
        print(f"Text: {res.text}")
    except Exception as e:
        print(f"Exception : {e}")    


def write_user():
    url  = "http://localhost:8080/api/write";
    text_data = "test_user 123456";
    headers = {"Content-Type": "text/plain"}
    try:
        response = r.post(url,data = text_data,headers= headers)
        print(f"Status Code: {response.status_code}")
        print(f"Text: {response.text}")
    except Exception as e:
        print(f"Exception as {e}")
        return;    

def get_username():
    url  = "http://localhost:8080/api/getpassword";
    text_data = "ivan";
    headers = {"Content-Type": "text/plain"}

    try:
        res = r.post(url,data = text_data,headers= headers)
        print(f"Status code : {res.status_code}")
        print(f"Password : {res.text}")
    except Exception as e:
        print(f"Exception as e: {e}")    


def validate_username_post():
    url  = "http://localhost:8080/api/validate"
    text_data = "ivan 81"
    headers = {"Content-Type": "text/plain"}

    try:
        res = r.post(url,data = text_data,headers=headers)
        print(f"Status code: {res.status_code}")
        print(f"Text: {res.text}")
    except Exception as e:
        print(f"Exception : {e}")    


def write_history_data():
    url  = "http://localhost:8080/api/wrhistory"
    text_data = "ivan hellothisisatest"
    headers = {"Content-Type": "text/plain"}
    try:
        res = r.post(url,data = text_data,headers=headers)
        print(f"Status code: {res.status_code}")
        print(f"Text: {res.text}")
    except Exception as e:
        print(f"Exception : {e}")

def delete_user():
    url = "http://localhost:8080/api/delete_user";
    data = "ivan"
    headers = {"Content-Type": "text/plain"}
    try:
        res = r.post(url,data = data,headers=headers)
        print(f"Status code: {res.status_code}")
        print(f"Text: {res.text}")
    except Exception as e:
        print(f"Exception : {e}")


def post_title():
    url = "http://localhost:8080/api/title";
    data = "ivan"
    headers = {"Content-Type": "text/plain"}
    try:
        res = r.post(url,data = data,headers=headers)
        print(f"Status code: {res.status_code}")
        print(f"Text: {res.text}")
    except Exception as e:
        print(f"Exception : {e}")





while True:
    user_search = input(">")

    if user_search == "write data":
        print(pos_request())

    if user_search == "get data":
        print(get_request())

    if user_search == "login":
        print(write_user())

    if user_search == "get pass":
        print(get_username())

    if user_search == "val":
        validate_username_post()

    if user_search == "dfh":
        write_default_user_history()

    if user_search == "wrh":
        write_history_data()

    if user_search == "delete user":
        delete_user()

    if user_search == "title":
        post_title()    
    if user_search == "exit":
        break


    if user_search == "clear":
        os.system("clear")                
