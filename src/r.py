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


while True:
    cons = input("> ")
    if cons == "reg":
        reg("user2","password2")
    elif cons == "login":
        login("user2","password2")
    else:
        print("No succh command")
