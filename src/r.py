import requests as r

def reg(username:str,password:str):
    data = {
        "username":username,
        "hash_psw":password
    }
    res = r.post("http://localhost:8000/register",json=data)
    print(f"Status code: {res.status_code}")
    print(f"Text {res.text}")

def login(username:str,password:str):
    data = {
        "username":username,
        "hash_psw":password
    }
    res = r.post("http://localhost:8000/login",json=data)
    print(f"Status code: {res.status_code}")
    print(f"Text {res.text}")
    print(f" Json: {res.json}")
def wr_def(username):
    data = {
        "username":username,
    }
    res = r.post("http://localhost:8000/write_default/posts",json=data)
    print(f"Status code: {res.status_code}")
    print(f"Text {res.text}")
    print(f" Json: {res.json}")
def get_subs_count(username):
    url = "http://localhost:8000/subs/count"
    data = {
        "username":username
    }
    resp = r.post(url,json = data)
    print(f"Status code: {resp.status_code}")
    print(f"Text {resp.text}")
    print(f" Json: {resp.json}")

def get_user_posts(username:str):
    url = "http://0.0.0.0:8000/get/user/posts"
    data = {
        "username":username
    }
    resp = r.post(url,json = data)
    print(f"RESULT: {resp.text}")
    return resp.json()

while True:
    cons = input("> ")
    if cons == "reg":
        reg("user2","password2")
    elif cons == "login":
        login("user2","password2")
    elif cons == "wrp":
        wr_def("test2")
    elif cons == "sbc":
        get_subs_count("user2")
    elif cons == "s":
        get_user_posts("lux23")    
    else:
        print("No succh command")
