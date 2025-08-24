import requests as r
import json




def req_1():
    url = "http://0.0.0.0:8000/load/default/videos"
    data = {
        "username":"ivantest"
    }
    resp = r.post(url,json = data)
    print(f"Satus code : {resp.status_code}")
    print(f"Text : {resp.text}")
def req_2():
    url = "http://0.0.0.0:8000/write/video/user"
    data = {
        "username":"ivantest",
        "code":"1234567",
        "title":"Hello this is a test"
    }
    resp = r.post(url,json = data)
    print(f"Status code : {resp}")
    print(f"Text : {resp.text}")
    print(f"Json : {resp.json}")

while True:
    cmd = input("> ")
    if(cmd == "r1"):
        req_1()
    if(cmd == "r2"):
        req_2()
    else:print("Unknown cmd")      

    