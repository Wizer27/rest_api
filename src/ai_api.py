from fastapi import FastAPI,HTTPException
import json
from pydantic import BaseModel
import hashlib

app = FastAPI()

@app.get("/")
async def main():
    return "AI urist API"

def has_key(key:str,data) -> bool:
    return key in data

def hash_username(username:str):
    byt = username.encode("utf-8")
    hashed = hashlib.sha256(byt).hexdigest()
    return hashed
#id:str = Field(default_factory=lambda: str(uuid.uuid4()))

class UserReg(BaseModel):
    username:str
    hash_password:str


@app.get("/user/register")
async def requester_(request:UserReg):
    if request.username == "" or (not request.username):
        raise HTTPException(status_code=400,detail="Username is not valid")
    with open("/Users/ivan/rest_api/data/users.json","r") as file:
        data = json.load(file)

    if has_key(request.username,data) or has_key(hash_username(request.username),data):
        raise HTTPException(status_code=400,detail="User already exists")
    else:
        data[hash_username(request.username)] = request.hash_password


    with open("/Users/ivan/rest_api/data/users.json","r") as file:
        json.dump(data,file,indent=2)    

      
    