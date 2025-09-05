import requests as r

def reg(username:str,password:str):
    data = {
        "username":username,
        "hash_psw":password
    }
    res = r.post("http://localhost:8000/register",json=data)
    print(f"Status code: {res.status_code}")
    print(f"Text {res.text}")

while True:
    cons = input("> ")
    if cons == "reg":
        reg("user1","password1")
    else:
        print("No succh command")
