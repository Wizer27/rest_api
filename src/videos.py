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

def base_videos_data(username:str) -> bool:
    with open("data/videos.json","r") as file:
        data = json.load(file)

    if not contains_username(username):
        data.append({
            "username":username,
            "videos":{}
        }) 
        with open("data/videos.json","w") as file:
            json.dump(data,file,indent=2)
            return True
    else:
        print("User already exists")
        return False
        
def test():
    pass

class Username(BaseModel):
    username :str


@app.post("/load/default/videos")
async def load_def(request:Username):
    base_videos_data(request.username)


class JsonDataUser(BaseModel):
    username:str
    code:str
    title:str

@app.post("/write/video/user")

async def write_video_to_user(request:JsonDataUser):
    with open("data/videos.json","r") as file:
        data = json.load(file)
    for user in data:
        if user["username"]  == request.username:
            try:
                user["videos"][request.title] = request.code  
            except Exception as e:
                print(f"Expceptin : {e}")    
    with open("data/videos.json","w") as file:
        json.dump(data,file,indent=2)
        print("Success")           


