from fastapi import FastAPI
from pydantic import BaseModel
import json

app = FastAPI()


@app.get("/")
def main():
    return "Alexandria videos API"


def contains_username(username:str) -> bool:
    with open("data/videos.json","r") as file:
        data = json.load(file)


    for user in data:
        if user["username"] == username:
            return True

    return False        

def base_videos_data(username:str):
    with open("data/videos.json","r") as file:
        data = json.load(file)




