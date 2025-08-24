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

req_1()

    